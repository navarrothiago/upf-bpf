#include "SessionManager.h"
#include <SessionProgram.h>
#include <SessionProgramManager.h>
#include <UPFProgram.h>
#include <bits/stdc++.h> //sort
#include <interfaces/ForwardingActionRules.h>
#include <interfaces/PacketDetectionRules.h>
#include <interfaces/SessionBpf.h>
#include <pfcp/pfcp_session.h>
#include <utils/LogDefines.h>
#include <wrappers/BPFMaps.h>

#include <next_prog_rule_key.h>
// #include "common_defs.h"
// #include "logger.hpp"
// #include "msg_pfcp.hpp"
// #include "pfcp_session.hpp"
// #include "thread_sched.hpp"
// #include "uint_generator.hpp"

SessionManager::SessionManager() { LOG_FUNC(); }

SessionManager::~SessionManager() { LOG_FUNC(); }

void SessionManager::createSession(std::shared_ptr<SessionBpf> pSession)
{
  LOG_FUNC();
  SessionProgramManager::getInstance().create(pSession->getSeid());
  LOG_DBG("Session {} has been cretead", pSession->getSeid());
}

void SessionManager::createBPFSession(std::shared_ptr<pfcp::pfcp_session> pSession)
{
  LOG_FUNC();
  LOG_DBG("Session {} received... Lets prepare the UP", pSession->get_up_seid());

  // mSeidToSession.push_back(pSession->get_up_seid());

  LOG_DBG("Find the PDR with highest precedence");
  // The lower precedence values indicate higher precedence of the PDR, and the
  // higher precedence values indicate lower precedence of the PDR when matching
  // a packet.
  std::sort(pSession->pdrs.begin(), pSession->pdrs.end(), SessionManager::comparePDR);

  LOG_DBG("Extract the key (PDI) from the highest priority PDR");
  auto pUPFProgram = UserPlaneComponent::getInstance().getUPFProgram();

  pfcp::pdi pdi;
  pfcp::fteid_t fteid;
  pfcp::ue_ip_address_t ueIpAddress;
  pfcp::source_interface_t sourceInterface;

  auto pdrHighPriority = pSession->pdrs[0];
  if(!pdrHighPriority->get(pdi) && !pdi.get(fteid) && !pdi.get(sourceInterface) && !pdi.get(ueIpAddress)) {
    throw std::runtime_error("No fields available");
  }

  // pUPFProgram->getNextProgRuleMap()->update(&next_rule_prog_index_key)
  LOG_DBG("Extract FAR from the highest priority PDR");
  std::shared_ptr<pfcp::pfcp_far> pFar;
  pfcp::far_id_t farId;

  if (!pdrHighPriority->get(farId) && !pSession->get(farId.far_id, pFar)){
    throw std::runtime_error("No fields available");
  }

  SessionProgramManager::getInstance().create(fteid.teid, sourceInterface.interface_value, ueIpAddress.ipv4_address.s_addr, pFar);

}

bool SessionManager::comparePDR(const std::shared_ptr<pfcp::pfcp_pdr> &pFirst,
                                const std::shared_ptr<pfcp::pfcp_pdr> &pSecond)
{
  LOG_FUNC();
  pfcp::precedence_t precedenceFirst, precedenceSecond;
  // navarrothiago - Check if exists.
  pFirst->get(precedenceFirst);
  pSecond->get(precedenceSecond);
  return precedenceFirst.precedence < precedenceSecond.precedence;
}

void SessionManager::removeSession(uint64_t seid)
{
  LOG_FUNC();
  SessionProgramManager::getInstance().remove(seid);
  LOG_DBG("Session {} has been removed", seid);
}

// TODO navarrothiago - how can we do atomically?
void SessionManager::addFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar)
{
  LOG_FUNC();
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  if(!pSessionProgram) {
    LOG_ERROR("Session {} not found", seid);
    throw std::runtime_error("The FAR cannot be added in the session");
  }

  auto far = pFar->getData();
  auto far_id = pFar->getFARId().far_id;
  pSessionProgram->getFARMap()->update(far_id, far, BPF_NOEXIST);
}

void SessionManager::addPDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr)
{
  LOG_FUNC();
  // TODO navarrothiago - Check if it is good to encapsulate in UserPlaneComponent like UPFProgram.
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  if(!pSessionProgram) {
    LOG_ERROR("Session {} not found", seid);
    throw std::runtime_error("The PDR cannot be added in the session");
  }

  auto pUPFProgram = UserPlaneComponent::getInstance().getUPFProgram();
  auto pdr = pPdr->getData();

  // TODO navarrothiago - Check the pdr counter.

  // Suppose that the other parameters was already check.
  // TODO navarrothiago - check parameters.
  // TODO navarrothiago - check the order, maybe it is better to fill the program map in the end.
  auto source_interface = pdr.pdi.source_interface.interface_value;
  auto teid = pdr.pdi.fteid.teid;
  uint32_t ueIp = pdr.pdi.ue_ip_address.ipv4_address;
  uint32_t programIdkey = static_cast<uint32_t>((ueIp * 0x80008001) >> 16);
  s32 fd;

  switch(source_interface) {
  case INTERFACE_VALUE_ACCESS:
    pSessionProgram->getUplinkPDRsMap()->update(teid, pdr, BPF_ANY);
    fd = pSessionProgram->getUplinkFileDescriptor();
    pUPFProgram->getTeidSessionMap()->update(teid, fd, BPF_ANY);
    break;
  // TODO navarrothiago - change to INTERFACE_VALUE_SGI_LAN_N6_LAN
  case INTERFACE_VALUE_CORE:
    pSessionProgram->getDownlinkPDRsMap()->update(ueIp, pdr, BPF_ANY);
    fd = pSessionProgram->getDownlinkFileDescriptor();
    pUPFProgram->getUeIpSessionMap()->update(programIdkey, fd, BPF_ANY);
    break;
  default:
    LOG_ERROR("Source interface {} not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }

  // TODO navarrothiago - REMOVED IT.
  pSessionProgram->getPDRMap()->update(pdr.pdr_id.rule_id, pdr, BPF_ANY);
}

std::shared_ptr<PacketDetectionRules> SessionManager::lookupPDR(uint64_t seid, uint16_t ruleId)
{
  LOG_FUNC();
  std::shared_ptr<PacketDetectionRules> pPdr;
  pfcp_pdr_t_ pPdrFound;

  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  if(!pSessionProgram) {
    LOG_ERROR("Session {} not found", seid);
    throw std::runtime_error("The PDR cannot be lookup in the session");
  }

  // Check if the PDR was found.
  if(pSessionProgram->getPDRMap()->lookup(ruleId, &pPdrFound) != 0) {
    LOG_WARN("PDR {} not found in program addr {} in map {}", ruleId, static_cast<void *>(pSessionProgram.get()),
             pSessionProgram->getPDRMap()->getName());
    return pPdr;
  }

  LOG_DBG("PDR {} found", ruleId);

  auto pUtils = UserPlaneComponent::getInstance().getRulesUtilities();
  pPdr = pUtils->createPDR(&pPdrFound);

  return pPdr;
}

std::shared_ptr<ForwardingActionRules> SessionManager::lookupFAR(uint64_t seid, uint32_t farId)
{
  LOG_FUNC();

  std::shared_ptr<ForwardingActionRules> pFar;
  pfcp_far_t_ pFarFound;

  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  if(!pSessionProgram) {
    LOG_ERROR("Session {} not found", seid);
    throw std::runtime_error("The FAR cannot be lookup in the session");
  }

  // Check if the PDR was found.
  if(pSessionProgram->getFARMap()->lookup(farId, &pFarFound) != 0) {
    LOG_WARN("FAR {} not found in program addr {} in map {}", farId, static_cast<void *>(pSessionProgram.get()),
             pSessionProgram->getFARMap()->getName());
    return pFar;
  }

  LOG_DBG("FAR {} found", farId);

  auto pUtils = UserPlaneComponent::getInstance().getRulesUtilities();
  pFar = pUtils->createFAR(&pFarFound);

  return pFar;
}

void SessionManager::updateFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar)
{
  LOG_FUNC();
  // TODO navarrothiago - Check if it is good to encapsulate in UserPlaneComponent like UPFProgram.
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  // Check if there is the session.
  if(!pSessionProgram) {
    LOG_ERROR("Session {} not found", seid);
    throw std::runtime_error("The FAR cannot be updated in the session");
  }

  auto pFarFound = lookupFAR(seid, pFar->getFARId().far_id);
  if(!pFarFound) {
    LOG_ERROR("FAR {} not found", pFar->getFARId().far_id);
    throw std::runtime_error("FAR not found");
  }
  // Check if the PDR was found.
  auto far = pFar->getData();
  pSessionProgram->getFARMap()->update(far.far_id.far_id, far, BPF_ANY);
}

void SessionManager::updatePDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr)
{
  LOG_FUNC();
  // TODO navarrothiago - Check if it is good to encapsulate in UserPlaneComponent like UPFProgram.
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  // Check if there is the session.
  if(!pSessionProgram) {
    LOG_ERROR("Session {} not found", seid);
    throw std::runtime_error("The PDR cannot be updated in the session");
  }

  auto pUPFProgram = UserPlaneComponent::getInstance().getUPFProgram();
  auto pUplinkPDRsMap = pSessionProgram->getUplinkPDRsMap();
  auto pPdrFound = lookupPDR(seid, pPdr->getPdrId().rule_id);

  // Check if the PDR was found.
  if(!pPdrFound) {
    LOG_ERROR("PDR {} not found", pPdr->getPdrId().rule_id);
    throw std::runtime_error("PDR not found");
  }

  auto pdrFound = pPdrFound->getData();
  auto pdr = pPdr->getData();

  // TODO (navarrothiago) check parameters.
  auto pUtil = UserPlaneComponent::getInstance().getRulesUtilities();
  auto source_interface = pdr.pdi.source_interface.interface_value;
  auto teid = pdr.pdi.fteid.teid;
  auto oldTeid = pdrFound.pdi.fteid.teid;

  switch(source_interface) {
  case INTERFACE_VALUE_ACCESS:
    // We are supposing 1x1 (TEIDxPDR)
    // Remove the old entry PDR from TEID->PDR map.
    pUplinkPDRsMap->remove(oldTeid);
    // Add the new entry PDR from TEID->PDR map.
    pUplinkPDRsMap->update(teid, pdr, BPF_NOEXIST);
    break;
  case INTERFACE_VALUE_CORE:
    // Remove the old entry PDR from TEID->PDR map.
    // removePDR(pdr.pdi.fteid.teid, pdrFound, mpDownlinkPDRsMap);
    // Add the new entry PDR from TEID->PDR map.
    // addPDR(pdr.pdi.fteid.teid, pdr, mpDownlinkPDRsMap);
    break;
  default:
    LOG_ERROR("Source interface {} not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }

  // TODO navarrothiago - check if TEID can change in update procedures.
  s32 fd = pSessionProgram->getUplinkFileDescriptor();
  pUPFProgram->getTeidSessionMap()->remove(oldTeid);
  pUPFProgram->getTeidSessionMap()->update(teid, fd, BPF_ANY);
  pSessionProgram->getPDRMap()->update(pdr.pdr_id.rule_id, pdr, BPF_ANY);
}

void SessionManager::removeFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar)
{
  LOG_FUNC();
  // TODO navarrothiago - Check if it is good to encapsulate in UserPlaneComponent like UPFProgram.
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  // Check if there is the session.
  if(!pSessionProgram) {
    LOG_ERROR("Session {} not found", seid);
    throw std::runtime_error("The FAR cannot be removed in the session");
  }

  // Check if the PDR was found.
  auto far = pFar->getData();
  pSessionProgram->getFARMap()->remove(far.far_id.far_id);
}

void SessionManager::removePDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr)
{
  LOG_FUNC();
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  if(!pSessionProgram) {
    LOG_ERROR("Session {} not found", seid);
    throw std::runtime_error("The PDR cannot be removed in the session");
  }
  auto pUPFProgram = UserPlaneComponent::getInstance().getUPFProgram();
  auto pUplinkPDRsMap = pSessionProgram->getUplinkPDRsMap();
  auto pdr = pPdr->getData();

  // Suppose that the other parameters was already check.
  // TODO navarrothiago - check parameters.
  // TODO navarrothiago - check the order, maybe it is better to fill the program map in the end.
  // Remove PDR from PDR maps (TEID/UE IP -> PDR).
  auto source_interface = pPdr->getPdi().source_interface.interface_value;
  auto teid = pdr.pdi.fteid.teid;

  switch(source_interface) {
  case INTERFACE_VALUE_ACCESS:
    pUplinkPDRsMap->remove(teid);
    break;
  case INTERFACE_VALUE_CORE:
    // removePDR(pPdr->getPdi().ue_ip_address.ipv4_address, pdr, mpDownlinkPDRsMap);
    break;
  default:
    LOG_ERROR("Source interface {} not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }

  pUPFProgram->getTeidSessionMap()->remove(teid);
  pSessionProgram->getPDRMap()->remove(pdr.pdr_id.rule_id);
}

// add_pfcp_session_by_cp_fseid(fseid, s);
// add_pfcp_session_by_up_seid(session->seid, s);

// bool SessionManager::get_pfcp_session_by_cp_fseid(const pfcp::fseid_t& fseid, std::shared_ptr<pfcp::pfcp_session>&
// session) const {
//   std::unordered_map<fseid_t, std::shared_ptr<pfcp::pfcp_session>>::const_iterator sit =
//   cp_fseid2pfcp_sessions.find(fseid); if (sit == cp_fseid2pfcp_sessions.end()) {
//     return false;
//   } else {
//     session = sit->second;
//     return true;
//   }
// }

// void SessionManager::handleSessionEstablishmentRequest(std::shared_ptr<pfcp::pfcp_session_establishment_request>
// pRequest)
// {
// // void pfcp_switch::handle_pfcp_session_establishment_request(
// //     std::shared_ptr<itti_sxab_session_establishment_request> sreq,
// //     itti_sxab_session_establishment_response* resp) {
//   pfcp::fseid_t fseid                          = {};
//   pfcp::cause_t cause = {.cause_value = CAUSE_VALUE_REQUEST_ACCEPTED};
//   pfcp::offending_ie_t offending_ie = {};

//   if (pRequest.get(fseid)) {
//     std::shared_ptr<pfcp::pfcp_session> s = {};
//     bool exist            = get_pfcp_session_by_cp_fseid(fseid, s);
//     pfcp_session* session = nullptr;
//     if (not exist) {
//       session = new pfcp_session(fseid, generate_seid());

//       for (auto it : pRequest.create_fars) {
//         create_far& cr_far = it;
//         if (not session->create(cr_far, cause, offending_ie.offending_ie)) {
//           session->cleanup();
//           delete session;
//           break;
//         }
//       }

//       if (cause.cause_value == CAUSE_VALUE_REQUEST_ACCEPTED) {
//         //--------------------------------
//         // Process PDR to be created
//         cause.cause_value = CAUSE_VALUE_REQUEST_ACCEPTED;
//         for (auto it : pRequest.create_pdrs) {
//           create_pdr& cr_pdr            = it;
//           pfcp::fteid_t allocated_fteid = {};

//           pfcp::far_id_t far_id = {};
//           if (not cr_pdr.get(far_id)) {
//             // should be caught in lower layer
//             cause.cause_value         = CAUSE_VALUE_MANDATORY_IE_MISSING;
//             offending_ie.offending_ie = PFCP_IE_FAR_ID;
//             session->cleanup();
//             delete session;
//             break;
//           }
//           // create pdr after create far
//           pfcp::create_far cr_far = {};
//           if (not pRequest.get(far_id, cr_far)) {
//             // should be caught in lower layer
//             cause.cause_value         = CAUSE_VALUE_MANDATORY_IE_MISSING;
//             offending_ie.offending_ie = PFCP_IE_CREATE_FAR;
//             session->cleanup();
//             delete session;
//             break;
//           }

//           if (not session->create(
//                   cr_pdr, cause, offending_ie.offending_ie, allocated_fteid)) {
//             session->cleanup();
//             delete session;
//             if (cause.cause_value == CAUSE_VALUE_CONDITIONAL_IE_MISSING) {
//               resp->pfcp_ies.set(offending_ie);
//             }
//             resp->pfcp_ies.set(cause);
//             break;
//           }
//           pfcp::created_pdr created_pdr = {};
//           created_pdr.set(cr_pdr.pdr_id.second);
//           created_pdr.set(allocated_fteid);
//           resp->pfcp_ies.set(created_pdr);
//         }
//       }

//       if (cause.cause_value == CAUSE_VALUE_REQUEST_ACCEPTED) {
//         s = std::shared_ptr<pfcp_session>(session);
//         add_pfcp_session_by_cp_fseid(fseid, s);
//         add_pfcp_session_by_up_seid(session->seid, s);
//         // start_timer_min_commit_interval();
//         // start_timer_max_commit_interval();

//         pfcp::fseid_t up_fseid = {};
//         spgwu_cfg.get_pfcp_fseid(up_fseid);
//         up_fseid.seid = session->get_up_seid();
//         resp->pfcp_ies.set(up_fseid);

//         // Register session
//         pfcp::node_id_t node_id = {};
//         pRequest.get(node_id);
//         pfcp_associations::get_instance().notify_add_session(node_id, fseid);
//       }
//     } else {
//       cause.cause_value = CAUSE_VALUE_REQUEST_REJECTED;
//     }
//   } else {
//     // should be caught in lower layer
//     cause.cause_value         = CAUSE_VALUE_MANDATORY_IE_MISSING;
//     offending_ie.offending_ie = PFCP_IE_F_SEID;
//   }
//   resp->pfcp_ies.set(cause);
//   if ((cause.cause_value == CAUSE_VALUE_MANDATORY_IE_MISSING) ||
//       (cause.cause_value == CAUSE_VALUE_CONDITIONAL_IE_MISSING)) {
//     resp->pfcp_ies.set(offending_ie);
//   }

// #if DEBUG_IS_ON
//   std::cout
//       << "\n+------------------------------------------------------------------"
//          "---------------------------------------------------------------------"
//          "-----------------------------------------------------------+"
//       << std::endl;
//   std::cout
//       << "| PFCP switch Packet Detection Rule list ordered by established "
//          "sessions:                                                            "
//          "                                                              |"
//       << std::endl;
//   std::cout
//       << "+----------------+----+--------+--------+------------+---------------"
//          "------------------------+----------------------+----------------+----"
//          "---------------------------------------------------------+"
//       << std::endl;
//   std::cout
//       << "|  SEID          |pdr |  far   |predence|   action   |        create "
//          "outer hdr         tun id| rmv outer hdr  tun id|    UE IPv4     |    "
//          "                                                         |"
//       << std::endl;
//   std::cout
//       << "+----------------+----+--------+--------+------------+---------------"
//          "------------------------+----------------------+----------------+----"
//          "---------------------------------------------------------+"
//       << std::endl;
//   for (const auto& it : up_seid2pfcp_sessions) {
//     std::cout << it.second->to_string() << std::endl;
//   }
// #endif
// }
