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

SessionManager::SessionManager() { LOG_FUNC(); }

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
  uint32_t programIdkey = hash(ueIp);
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
  if(pSessionProgram->getUplinkPDRsMap()->lookup(ruleId, &pPdrFound) != 0 ||
     pSessionProgram->getDownlinkPDRsMap()->lookup(ruleId, &pPdrFound) != 0) {
    LOG_WARN("PDR {} not found in program addr {} in map {} or ", ruleId, static_cast<void *>(pSessionProgram.get()),
             pSessionProgram->getUplinkPDRsMap()->getName(), pSessionProgram->getDownlinkPDRsMap()->getName());
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
  auto pDownlinkPDRsMap = pSessionProgram->getDownlinkPDRsMap();
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
  auto newHash = hash(pdr.pdi.ue_ip_address.ipv4_address);
  auto oldTeid = pdrFound.pdi.fteid.teid;
  auto oldHash = hash(pdrFound.pdi.ue_ip_address.ipv4_address);
  s32 fd = 0;

  switch(source_interface) {
  case INTERFACE_VALUE_ACCESS:
    // We are supposing 1x1 (TEIDxPDR)
    // Remove the old entry PDR from TEID->PDR map.
    pUplinkPDRsMap->remove(oldTeid);
    // Add the new entry PDR from TEID->PDR map.
    pUplinkPDRsMap->update(teid, pdr, BPF_NOEXIST);
    fd = pSessionProgram->getUplinkFileDescriptor();
    pUPFProgram->getTeidSessionMap()->remove(oldTeid);
    pUPFProgram->getTeidSessionMap()->update(teid, fd, BPF_ANY);
    break;
  case INTERFACE_VALUE_CORE:
    // We are supposing 1x1 (UEIPxPDR)
    // Remove the old entry PDR from UEIP->PDR map.
    pDownlinkPDRsMap->remove(oldHash);
    // Add the new entry PDR from TEID->PDR map.
    pDownlinkPDRsMap->update(teid, newHash, BPF_NOEXIST);
    fd = pSessionProgram->getDownlinkFileDescriptor();
    pUPFProgram->getUeIpSessionMap()->remove(oldHash);
    pUPFProgram->getTeidSessionMap()->update(newHash, fd, BPF_ANY);
    break;
  default:
    LOG_ERROR("Source interface {} not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }
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
  auto pDownlinkPDRsMap = pSessionProgram->getDownlinkPDRsMap();
  auto pdr = pPdr->getData();

  // Suppose that the other parameters was already check.
  // TODO navarrothiago - check parameters.
  // TODO navarrothiago - check the order, maybe it is better to fill the program map in the end.
  // Remove PDR from PDR maps (TEID/UE IP -> PDR).
  auto source_interface = pPdr->getPdi().source_interface.interface_value;
  auto teid = pdr.pdi.fteid.teid;
  auto hashValue = hash(pdr.pdi.ue_ip_address.ipv4_address);

  switch(source_interface) {
  case INTERFACE_VALUE_ACCESS:
    pUplinkPDRsMap->remove(teid);
    pUPFProgram->getTeidSessionMap()->remove(teid);
    pSessionProgram->getUplinkPDRsMap()->remove(teid);
    break;
  case INTERFACE_VALUE_CORE:
    pUplinkPDRsMap->remove(hashValue);
    pUPFProgram->getTeidSessionMap()->remove(hashValue);
    pSessionProgram->getDownlinkPDRsMap()->remove(hashValue);
    break;
  default:
    LOG_ERROR("Source interface {} not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }

}

uint32_t SessionManager::hash(uint32_t ueIp)
{
  LOG_FUNC();
  return static_cast<uint32_t>((ueIp * 0x80008001) >> 16);
}
