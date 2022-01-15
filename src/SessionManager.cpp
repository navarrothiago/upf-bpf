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
  LOG_DBG("PDI extracted from PDR {}", pdrHighPriority->pdr_id.rule_id);

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
  if(mSeidToSession.find(seid) == mSeidToSession.end()){
    LOG_ERROR("The session {} does not exist. Cannot be removed", seid);
    throw std::runtime_error("The session does not exist. Cannot be removed");
  }
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
