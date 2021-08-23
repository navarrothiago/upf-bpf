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


  LOG_DBG("Find the PDR with highest precedence");
  // The lower precedence values indicate higher precedence of the PDR, and the
  // higher precedence values indicate lower precedence of the PDR when matching
  // a packet.

  // TODO navarrothiago - create a list for DL and UL. There will be two
  // deployment on the dataplane. One related to UL and other related to DL.
  // Today, we only deploy the highest priority. We dont take into account if it
  // is a DL or UP.
  std::sort(pSession->pdrs.begin(), pSession->pdrs.end(), SessionManager::comparePDR);

  LOG_DBG("Extract the key (PDI) from the highest priority PDR");
  auto pUPFProgram = UserPlaneComponent::getInstance().getUPFProgram();

  pfcp::pdi pdi;
  pfcp::fteid_t fteid;
  pfcp::ue_ip_address_t ueIpAddress;
  pfcp::source_interface_t sourceInterface;

  if (pSession->pdrs.empty()){
    LOG_ERROR("No PDR was found in session %d", pSession->seid);
    throw std::runtime_error("No PDR was found in session");
  }

  auto pdrHighPriority = pSession->pdrs[0];
  if(!(pdrHighPriority->get(pdi) && pdi.get(fteid) && pdi.get(sourceInterface) && pdi.get(ueIpAddress))) {
    throw std::runtime_error("No fields available");
  }
  LOG_DBG("PDI extracted from PDR %d", pdrHighPriority->pdr_id.rule_id);

  // pUPFProgram->getNextProgRuleMap()->update(&next_rule_prog_index_key)
  LOG_DBG("Extract FAR from the highest priority PDR");
  std::shared_ptr<pfcp::pfcp_far> pFar;
  pfcp::far_id_t farId;

  if (!(pdrHighPriority->get(farId) && pSession->get(farId.far_id, pFar))){
    throw std::runtime_error("No fields available");
  }

  SessionProgramManager::getInstance().createPipeline(pSession->get_up_seid(), fteid.teid, sourceInterface.interface_value, ueIpAddress.ipv4_address.s_addr, pFar);

  LOG_DBG("Add session");
  mSeidToSession[pSession->get_up_seid()] = pSession;
}

void SessionManager::removeBPFSession(uint64_t seid)
{
  LOG_FUNC();
  SessionProgramManager::getInstance().removePipeline(seid);
  LOG_DBG("Session {} has been removed", seid);
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
