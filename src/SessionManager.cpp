#include "SessionManager.h"
#include <SessionProgram.h>
#include <SessionProgramManager.h>
#include <UPFProgram.h>
#include <interfaces/ForwardingActionRules.h>
#include <interfaces/PacketDetectionRules.h>
#include <interfaces/SessionBpf.h>
#include <pfcp/pfcp_session.h>
#include <utils/LogDefines.h>
#include <wrappers/BPFMaps.h>

SessionManager::SessionManager()
{
  LOG_FUNC();
}

SessionManager::~SessionManager() { LOG_FUNC(); }

void SessionManager::createSession(std::shared_ptr<SessionBpf> pSession)
{
  LOG_FUNC();
  SessionProgramManager::getInstance().create(pSession->getSeid());
  LOG_DBG("Session {} has been cretead", pSession->getSeid());
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
    LOG_ERROR("Session %d not found", seid);
    throw std::runtime_error("The PDR cannot be added in the session");
  }

  auto pUPFProgram = UserPlaneComponent::getInstance().getUPFProgram();
  auto pUplinkPDRsMap = pSessionProgram->getUplinkPDRsMap();
  auto pdr = pPdr->getData();

  // TODO navarrothiago - Check the pdr counter.

  // Suppose that the other parameters was already check.
  // TODO navarrothiago - check parameters.
  // TODO navarrothiago - check the order, maybe it is better to fill the program map in the end.
  auto source_interface = pdr.pdi.source_interface.interface_value;
  auto teid = pdr.pdi.fteid.teid;

  switch(source_interface) {
  case INTERFACE_VALUE_ACCESS:
    pSessionProgram->getUplinkPDRsMap()->update(teid, pdr, BPF_ANY);
    break;
  case INTERFACE_VALUE_CORE:
    // addPDR(pdr.pdi.ue_ip_address.ipv4_address, pdr, mpDownlinkPDRsMap);
    break;
  default:
    LOG_ERROR("Source interface %d not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }

  s32 fd = pSessionProgram->getFileDescriptor();
  pUPFProgram->getProgramsMap()->update(teid, fd, BPF_ANY);
  pSessionProgram->getPDRMap()->update(pdr.pdr_id.rule_id, pdr, BPF_ANY);
}

std::shared_ptr<PacketDetectionRules> SessionManager::lookupPDR(uint64_t seid, uint16_t ruleId)
{
  LOG_FUNC();
  std::shared_ptr<PacketDetectionRules> pPdr;
  pfcp_pdr_t_ pPdrFound;

  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  if(!pSessionProgram) {
    LOG_ERROR("Session %d not found", seid);
    throw std::runtime_error("The PDR cannot be lookup in the session");
  }

  // Check if the PDR was found.
  if(pSessionProgram->getPDRMap()->lookup(ruleId, &pPdrFound) != 0) {
    LOG_WARN("PDR {} not found in program addr {} in map {}", ruleId, static_cast<void *>(pSessionProgram.get()), pSessionProgram->getPDRMap()->getName());
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
    LOG_ERROR("Session %d not found", seid);
    throw std::runtime_error("The FAR cannot be lookup in the session");
  }

  // Check if the PDR was found.
  if(pSessionProgram->getFARMap()->lookup(farId, &pFarFound) != 0) {
    LOG_WARN("FAR {} not found in program addr {} in map {}", farId, static_cast<void *>(pSessionProgram.get()), pSessionProgram->getFARMap()->getName());
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
    LOG_ERROR("Session %d not found", seid);
    throw std::runtime_error("The FAR cannot be updated in the session");
  }

  auto pFarFound = lookupFAR(seid, pFar->getFARId().far_id);
  if(!pFarFound){
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
    LOG_ERROR("Session %d not found", seid);
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
    LOG_ERROR("Source interface %d not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }

  // TODO navarrothiago - check if TEID can change in update procedures.
  s32 fd = pSessionProgram->getFileDescriptor();
  pUPFProgram->getProgramsMap()->remove(oldTeid);
  pUPFProgram->getProgramsMap()->update(teid, fd, BPF_ANY);
  pSessionProgram->getPDRMap()->update(pdr.pdr_id.rule_id, pdr, BPF_ANY);
}

void SessionManager::removeFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar)
{
  LOG_FUNC();
  // TODO navarrothiago - Check if it is good to encapsulate in UserPlaneComponent like UPFProgram.
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  // Check if there is the session.
  if(!pSessionProgram) {
    LOG_ERROR("Session %d not found", seid);
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
    LOG_ERROR("Session %d not found", seid);
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
    LOG_ERROR("Source interface %d not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }

  pUPFProgram->getProgramsMap()->remove(teid);
  pSessionProgram->getPDRMap()->remove(pdr.pdr_id.rule_id);
}