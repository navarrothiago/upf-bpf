#include "SessionManager.h"
#include <interfaces/ForwardingActionRules.h>
#include <interfaces/PacketDetectionRules.h>
#include <interfaces/SessionBpf.h>
#include <pfcp/pfcp_session.h>
#include <utils/LogDefines.h>
#include <wrappers/BPFMaps.h>

SessionManager::SessionManager(std::shared_ptr<BPFMap> pSessionsMap, std::shared_ptr<BPFMap> pUplinkPdrMap)
    : mpSessionsMap(pSessionsMap)
    , mpUplinkPDRsMap(pUplinkPdrMap)
{
  LOG_FUNC();
}

SessionManager::~SessionManager() { LOG_FUNC(); }

void SessionManager::createSession(std::shared_ptr<SessionBpf> pSession)
{
  LOG_FUNC();
  auto session = pSession->getData();
  if(mpSessionsMap->update(session.seid, session, BPF_NOEXIST) != 0) {
    LOG_ERROR("Cannot create session {}", pSession->getSeid());
    throw std::runtime_error("Cannot create session");
  }

}

void SessionManager::removeSession(uint64_t seid)
{
  LOG_FUNC();
  if(mpSessionsMap->remove(seid) != 0) {
    LOG_ERROR("Cannot remove session {}", seid);
    throw std::runtime_error("Cannot remove session");
  }
}

// TODO navarrothiago - how can we do atomically?
void SessionManager::addFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar)
{
  LOG_FUNC();
  pfcp_session_t_ session;

  // Lookup session based on seid.
  mpSessionsMap->lookup(seid, &session);

  // Check the far counter.
  if(session.fars_counter >= SESSION_FARS_MAX_SIZE) {
    LOG_ERROR("Array is full!! The FAR {} cannot be added in the session {}", pFar->getFARId().far_id, seid);
    throw std::runtime_error("The FAR cannot be added in the session");
  }

  // Insert the element in the end of the fars array.
  // The next position is represented by the counter var.
  // Update the counter.
  uint32_t index = session.fars_counter++;
  session.fars[index] = pFar->getData();

  // Update session.
  mpSessionsMap->update(seid, session, BPF_EXIST);
  LOG_DBG("FAR {} was inserted at index {} in session {}!", pFar->getFARId().far_id, index, seid);
}

void SessionManager::addPDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr)
{
  LOG_FUNC();
  pfcp_session_t_ session;

  // Lookup session based on seid.
  // TODO navarrothiago - check if session not exists.
  mpSessionsMap->lookup(seid, &session);

  // Check the pdr counter.
  if(session.pdrs_counter >= SESSION_PDRS_MAX_SIZE) {
    LOG_ERROR("Array is full!! The PDR {} cannot be added in the session {}", pPdr->getPdrId().rule_id, seid);
    throw std::runtime_error("The PDR cannot be added in the session");
  }

  // Insert the element in the end of the pdrs array.
  // The next position is represented by the counter var.
  // Update the counter.
  uint32_t index = session.pdrs_counter++;
  session.pdrs[index] = pPdr->getData();

  // Suppose that the other parameters was already check.
  // TODO (navarrothiago) check parameters.
  auto source_interface = session.pdrs[index].pdi.source_interface.interface_value;
  switch(source_interface) {
  case INTERFACE_VALUE_ACCESS:
    addPDR(session.pdrs[index].pdi.fteid.teid, session.pdrs[index], mpUplinkPDRsMap);
    break;
  case INTERFACE_VALUE_CORE:
    addPDR(session.pdrs[index].pdi.ue_ip_address.ipv4_address, session.pdrs[index], mpDownlinkPDRsMap);
    break;
  default:
    LOG_ERROR("Source interface %d not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }

  // Update session.
  mpSessionsMap->update(seid, session, BPF_EXIST);
  LOG_DBG("PDR {} was inserted at index {} in session {}!", pPdr->getPdrId().rule_id, index, seid);
}

std::shared_ptr<PacketDetectionRules> SessionManager::lookupPDR(uint64_t seid, uint16_t ruleId)
{
  LOG_FUNC();
  std::shared_ptr<PacketDetectionRules> pPdr;
  pfcp_session_t_ session;

  // Lookup session based on seid.
  mpSessionsMap->lookup(seid, &session);

  if(session.pdrs_counter == 0) {
    // Empty PDR.
    LOG_WARN("There is no PDRs");
    return pPdr;
  }

  auto pPdrFound = std::find_if(session.pdrs, session.pdrs + session.pdrs_counter, [&ruleId](pfcp_pdr_t_ &pdr) { return pdr.pdr_id.rule_id == ruleId; });

  // Check if the PDR was found.
  if(pPdrFound == session.pdrs + session.pdrs_counter) {
    LOG_WARN("PDR {} not found", ruleId);
    return pPdr;
  }

  auto pUtils = UserPlaneComponent::getInstance().getRulesUtilities();
  pPdr = pUtils->createPDR(pPdrFound);

  return pPdr;
}

SessionManager::pdrs_t SessionManager::lookupPDRsUplink(uint32_t teid)
{
  LOG_FUNC();
  return lookupPDRs(teid, mpUplinkPDRsMap);
}

SessionManager::pdrs_t SessionManager::lookupPDRsDownlink(uint32_t ueIPAddress)
{
  LOG_FUNC();
  return lookupPDRs(ueIPAddress, mpDownlinkPDRsMap);
}

std::shared_ptr<ForwardingActionRules> SessionManager::lookupFAR(uint64_t seid, uint32_t farId)
{
  LOG_FUNC();
  std::shared_ptr<ForwardingActionRules> pFar;
  pfcp_session_t_ session;

  // Lookup session based on seid.
  mpSessionsMap->lookup(seid, &session);

  if(session.fars_counter == 0) {
    // Empty PDR.
    LOG_WARN("There is no FARs");
    return pFar;
  }

  auto pFarFound = std::find_if(session.fars, session.fars + session.fars_counter, [&farId](pfcp_far_t_ &far) { return far.far_id.far_id == farId; });

  // Check if the PDR was found.
  if(pFarFound == session.fars + session.fars_counter) {
    LOG_WARN("FAR {} not found", farId);
    return pFar;
  }

  auto pUtils = UserPlaneComponent::getInstance().getRulesUtilities();
  pFar = pUtils->createFAR(pFarFound);

  return pFar;
}

void SessionManager::updateFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar)
{
  LOG_FUNC();

  pfcp_session_t_ session;
  // Lookup session based on seid.
  // TODO navarrothiago - check if session not exists.
  mpSessionsMap->lookup(seid, &session);

  // Check if the FAR's counter is equal to zero.
  if(session.fars_counter == 0) {
    LOG_ERROR("There is not any element in FARs array. The FAR {0} cannot be updated in the session {1}", pFar->getFARId().far_id, seid);
    throw std::runtime_error("There is not any element in FARs array. The FAR cannot be updated in the session");
  }

  // Look for the PDR in the array.
  auto pFarFound = std::find_if(session.fars, session.fars + session.fars_counter, [&pFar](pfcp_far_t_ &far) { return far.far_id.far_id == pFar->getFARId().far_id; });

  // Check if the PDR was found.
  if(pFarFound == session.fars + session.fars_counter) {
    LOG_WARN("FAR {} not found", pFar->getFARId().far_id);
    throw std::runtime_error("FAR not found");
  }

  // Update all fields.
  // *pFarFound = std::move(*pFar);
  auto pUtil = UserPlaneComponent::getInstance().getRulesUtilities();
  pUtil->copyFAR(pFarFound, pFar.get());

  // Update session in BPF map.
  mpSessionsMap->update(seid, session, BPF_EXIST);
  LOG_DBG("FAR {} was update  in session {}!", pFar->getFARId().far_id, seid);
}

void SessionManager::updatePDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr)
{
  LOG_FUNC();
  pfcp_session_t_ session;
  // Lookup session based on seid.
  // TODO navarrothiago - check if session not exists.
  mpSessionsMap->lookup(seid, &session);

  // Check if the PDR's counter is equal to zero.
  if(session.pdrs_counter == 0) {
    LOG_ERROR("There is not any element in PDRs array. The PDR {0} cannot be updated in the session {1}", pPdr->getPdrId().rule_id, seid);
    throw std::runtime_error("There is not any element in PDRs array. The PDR cannot be updated in the session");
  }

  // Look for the PDR in the array.
  auto pPdrFound = std::find_if(session.pdrs, session.pdrs + session.pdrs_counter, [&pPdr](pfcp_pdr_t_ &pdr) { return pdr.pdr_id.rule_id == pPdr->getPdrId().rule_id; });

  // Check if the PDR was found.
  if(pPdrFound == session.pdrs + session.pdrs_counter) {
    LOG_WARN("PDR {} not found", pPdr->getPdrId().rule_id);
    throw std::runtime_error("PDR not found");
  }

  // Suppose that the other parameters was already check.
  // TODO (navarrothiago) check parameters.
  auto pUtil = UserPlaneComponent::getInstance().getRulesUtilities();
  auto source_interface = pPdrFound->pdi.source_interface.interface_value;
  switch(source_interface) {
  case INTERFACE_VALUE_ACCESS:
    // Remove the old entry PDR from TEID->PDR map.
    removePDR(pPdrFound->pdi.fteid.teid, *pPdrFound, mpUplinkPDRsMap);
    // Update all fields. Copy the new PDR to the BPF address.
    pUtil->copyPDR(pPdrFound, pPdr.get());
    // Add the new entry PDR from TEID->PDR map.
    addPDR(pPdrFound->pdi.fteid.teid, *pPdrFound, mpUplinkPDRsMap);
    break;
  case INTERFACE_VALUE_CORE:
    // Remove the old entry PDR from TEID->PDR map.
    removePDR(pPdrFound->pdi.fteid.teid, *pPdrFound, mpDownlinkPDRsMap);
    // Update all fields. Copy the new PDR to the BPF address.
    pUtil->copyPDR(pPdrFound, pPdr.get());
    // Add the new entry PDR from TEID->PDR map.
    addPDR(pPdrFound->pdi.fteid.teid, *pPdrFound, mpDownlinkPDRsMap);
    break;
  default:
    LOG_ERROR("Source interface %d not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }

  // Update session in BPF map.
  mpSessionsMap->update(seid, session, BPF_EXIST);
  LOG_DBG("PDR {} was update in session {}!", pPdr->getPdrId().rule_id, seid);
}

void SessionManager::removeFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar)
{
  LOG_FUNC();

  pfcp_session_t_ session;
  // Lookup session based on seid.
  // TODO navarrothiago - check if session not exists.
  mpSessionsMap->lookup(seid, &session);

  // Check if the FAR's counter is equal to zero.
  if(session.fars_counter == 0) {
    LOG_ERROR("There is not any element in PDRs array. The FAR {0} cannot be deleted in the session {1}", pFar->getFARId().far_id, seid);
    throw std::runtime_error("There is not any element in FARs array. The FAR cannot be deleted in the session");
  }

  auto pFarEnd = std::remove_if(session.fars, session.fars + session.fars_counter, [&](pfcp_far_t_ &far) { return far.far_id.far_id == pFar->getFARId().far_id; });

  // Check if PDR was found.
  if(session.fars + session.fars_counter == pFarEnd) {
    LOG_ERROR("FAR {} not found", pFar->getFARId().far_id);
    throw std::runtime_error("PDR not found");
  }

  // Move the new values to session address.
  std::move(session.fars, pFarEnd, session.fars);

  // Update the counter.
  session.fars_counter--;

  // Update session map.
  mpSessionsMap->update(session.seid, session, BPF_EXIST);

  LOG_DBG("FAR {} was remove at in session {}!", pFar->getFARId().far_id, seid);
}

void SessionManager::removePDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr)
{
  LOG_FUNC();
  pfcp_session_t_ session;

  // Lookup session based on seid.
  // TODO navarrothiago - check if session not exists.
  mpSessionsMap->lookup(seid, &session);

  // Check if the PDR's counter is equal to zero.
  if(session.pdrs_counter == 0) {
    LOG_ERROR("There is not any element in PDRs array. The PDR {0} cannot be deleted in the session {1}", pPdr->getPdrId().rule_id, seid);
    throw std::runtime_error("There is not any element in PDRs array. The PDR cannot be deleted in the session");
  }

  auto pPdrEnd = std::remove_if(session.pdrs, session.pdrs + session.pdrs_counter, [&](pfcp_pdr_t_ &pdr) { return pdr.pdr_id.rule_id == pPdr->getPdrId().rule_id; });

  // Check if PDR was found.
  if(session.pdrs + session.pdrs_counter == pPdrEnd) {
    LOG_ERROR("PDR {} not found", pPdr->getPdrId().rule_id);
    throw std::runtime_error("PDR not found");
  }

  // Remove PDR from PDR maps (TEID/UE IP -> PDR).
  auto source_interface = pPdrEnd->pdi.source_interface.interface_value;
  switch(source_interface) {
  case INTERFACE_VALUE_ACCESS:
    removePDR(pPdrEnd->pdi.fteid.teid, *pPdrEnd, mpUplinkPDRsMap);
    break;
  case INTERFACE_VALUE_CORE:
    removePDR(pPdrEnd->pdi.ue_ip_address.ipv4_address, *pPdrEnd, mpDownlinkPDRsMap);
    break;
  default:
    LOG_ERROR("Source interface %d not supported", source_interface);
    throw std::runtime_error("Source interface not supported");
  }

  // Remove PDR from PDRs array in the session map.
  // Move the new values to session address.
  // TODO navarrothiago - move [begin to it) U [it + 1 to end).
  // std::move(session.pdrs, pPdrEnd, session.pdrs);

  // if(pPdrEnd != &session.pdrs[session.pdrs_counter]) {
  //   LOG_DBG("Removed PDR is was not in the last position. So, move the [it + 1 to end) to the PDRs array.");
  //   // shift one position to the left.
  //   std::move(pPdrEnd + 1, &session.pdrs[session.pdrs_counter - 1], pPdrEnd);
  // }

  // Update the counter.
  session.pdrs_counter--;

  // Update session map.
  mpSessionsMap->update(session.seid, session, BPF_EXIST);

  LOG_DBG("PDR {} was removed at in session {}!", pPdr->getPdrId().rule_id, seid);
}

void SessionManager::createBPFContext(seid_t_ seid)
{
  LOG_FUNC();
  // UserPlaneComponent::getInstance().updateProgMap(seid);
}
