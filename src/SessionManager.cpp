#include "SessionManager.h"
#include <UPFProgramManager.h>
#include <pfcp/pfcp_session.h>
#include <utils/LogDefines.h>
#include <wrappers/BPFMap.hpp>
#include <wrappers/BPFMaps.h>

SessionManager::SessionManager(std::shared_ptr<BPFMap> pSessionsMap)
    : mpSessionsMap(pSessionsMap)
{
  LOG_FUNC();
}

SessionManager::~SessionManager() { LOG_FUNC(); }

void SessionManager::createSession(std::shared_ptr<pfcp_session_t> pSession)
{
  LOG_FUNC();
  if(mpSessionsMap->update(pSession->seid, *pSession, BPF_NOEXIST) != 0) {
    LOG_ERROR("Cannot create session {}", pSession->seid);
    throw std::runtime_error("Cannot create session");
  }
}

void SessionManager::removeSession(seid_t seid)
{
  LOG_FUNC();
  if(mpSessionsMap->remove(seid) != 0) {
    LOG_ERROR("Cannot remove session {}", seid);
    throw std::runtime_error("Cannot remove session");
  }
}

// TODO navarrothiago - how can we do atomically?
void SessionManager::addFAR(seid_t seid, std::shared_ptr<pfcp_far_t> pFar)
{
  LOG_FUNC();
  pfcp_session_t session;

  // Lookup session based on seid.
  mpSessionsMap->lookup(seid, &session);

  // Check the far counter.
  if(session.fars_counter >= SESSION_FARS_MAX_SIZE) {
    LOG_ERROR("Array is full!! The FAR {} cannot be added in the session {}", pFar->far_id.far_id, seid);
    throw std::runtime_error("The FAR cannot be added in the session");
  }

  // Insert the element in the end of the fars array.
  // The next position is represented by the counter var.
  // Update the counter.
  uint32_t index = session.fars_counter++;
  session.fars[index] = *pFar;

  // Update session.
  mpSessionsMap->update(seid, session, BPF_EXIST);
  LOG_DEBUG("FAR {} was inserted at index {} in session {}!", pFar->far_id.far_id, index, seid);
}

std::shared_ptr<pfcp_pdr_t> SessionManager::lookupPDR(seid_t seid, pdr_id_t pdrId)
{
  LOG_FUNC();
  std::shared_ptr<pfcp_pdr_t> pPdr;
  pfcp_session_t session;

  // Lookup session based on seid.
  mpSessionsMap->lookup(seid, &session);

  if(session.pdrs_counter == 0) {
    // Empty PDR.
    LOG_WARN("There is no PDRs");
    return pPdr;
  }

  auto pPdrFound = std::find_if(session.pdrs, session.pdrs + session.pdrs_counter, [&pdrId](pfcp_pdr_t &pdr) { return pdr.pdr_id.rule_id == pdrId.rule_id; });

  // Check if the PDR was found.
  if(pPdrFound == session.pdrs + session.pdrs_counter) {
    LOG_WARN("PDR {} not found", pdrId.rule_id);
    return pPdr;
  }

  pPdr = std::make_shared<pfcp_pdr_t>(*pPdrFound);

  return pPdr;
}

void SessionManager::addPDR(seid_t seid, std::shared_ptr<pfcp_pdr_t> pPdr)
{
  LOG_FUNC();
  pfcp_session_t session;

  // Lookup session based on seid.
  // TODO navarrothiago - check if session not exists.
  mpSessionsMap->lookup(seid, &session);

  // Check the far counter.
  if(session.pdrs_counter >= SESSION_PDRS_MAX_SIZE) {
    LOG_ERROR("Array is full!! The PDR {} cannot be added in the session {}", pPdr->pdr_id.rule_id, seid);
    throw std::runtime_error("The PDR cannot be added in the session");
  }

  // Insert the element in the end of the pdrs array.
  // The next position is represented by the counter var.
  // Update the counter.
  uint32_t index = session.pdrs_counter++;
  session.pdrs[index] = *pPdr;

  // Update session.
  mpSessionsMap->update(seid, session, BPF_EXIST);
  LOG_DEBUG("PDR {} was inserted at index {} in session {}!", pPdr->pdr_id.rule_id, index, seid);
}

void SessionManager::updateFAR(seid_t seid, std::shared_ptr<pfcp_far_t> pFar)
{
  LOG_FUNC();

  pfcp_session_t session;
  // Lookup session based on seid.
  // TODO navarrothiago - check if session not exists.
  mpSessionsMap->lookup(seid, &session);

  // Check if the FAR's counter is equal to zero.
  if(session.fars_counter == 0) {
    LOG_ERROR("There is not any element in PDRs array. The FAR {0} cannot be updated in the session {1}", pFar->far_id.far_id, seid);
    throw std::runtime_error("There is not any element in FARs array. The FAR cannot be updated in the session");
  }

  // Look for the FAR in the array.
  uint32_t i;
  for(uint32_t i = 0; i < session.fars_counter; i++) {
    if(session.fars[i].far_id.far_id == pFar->far_id.far_id) {

      // Update all fields.
      session.fars[i] = *pFar;

      // Update session in BPF map.
      mpSessionsMap->update(seid, session, BPF_EXIST);
      LOG_DEBUG("FAR {} was updated at index {} in session {}!", pFar->far_id.far_id, i, seid);
      return;
    }
  }
}

void SessionManager::updatePDR(seid_t seid, std::shared_ptr<pfcp_pdr_t> pPdr)
{
  LOG_FUNC();
  pfcp_session_t session;
  // Lookup session based on seid.
  // TODO navarrothiago - check if session not exists.
  mpSessionsMap->lookup(seid, &session);

  // Check if the PDR's counter is equal to zero.
  if(session.pdrs_counter == 0) {
    LOG_ERROR("There is not any element in PDRs array. The PDR {0} cannot be updated in the session {1}", pPdr->pdr_id.rule_id, seid);
    throw std::runtime_error("There is not any element in PDRs array. The PDR cannot be updated in the session");
  }

  // Look for the PDR in the array.
  auto pPdrFound = std::find_if(session.pdrs, session.pdrs + session.pdrs_counter, [&pPdr](pfcp_pdr_t &pdr) { return pdr.pdr_id.rule_id == pPdr->pdr_id.rule_id; });

  // Check if the PDR was found.
  if(pPdrFound == session.pdrs + session.pdrs_counter) {
    LOG_WARN("PDR {} not found", pPdr->pdr_id.rule_id);
    throw std::runtime_error("PDR not found");
  }

  // Update all fields.
  *pPdrFound = std::move(*pPdr);

  // Update session in BPF map.
  mpSessionsMap->update(seid, session, BPF_EXIST);
  LOG_DEBUG("PDR {} was update  in session {}!", pPdr->pdr_id.rule_id, seid);
}

void SessionManager::removeFAR(seid_t seid, std::shared_ptr<pfcp_far_t> pFar)
{
  LOG_FUNC();

  pfcp_session_t session;
  // Lookup session based on seid.
  // TODO navarrothiago - check if session not exists.
  mpSessionsMap->lookup(seid, &session);

  // Check if the FAR's counter is equal to zero.
  if(session.fars_counter == 0) {
    LOG_ERROR("There is not any element in PDRs array. The FAR {0} cannot be deleted in the session {1}", pFar->far_id.far_id, seid);
    throw std::runtime_error("There is not any element in FARs array. The FAR cannot be deleted in the session");
  }

  auto pFarEnd = std::remove_if(session.fars, session.fars + session.fars_counter, [&](pfcp_far_t &far) { return far.far_id.far_id == pFar->far_id.far_id; });

  // Check if PDR was found.
  if(session.fars + session.fars_counter == pFarEnd) {
    LOG_ERROR("FAR {} not found", pFar->far_id.far_id);
    throw std::runtime_error("PDR not found");
  }

  // Move the new values to session address.
  std::move(session.fars, pFarEnd, session.fars);

  // Update the counter.
  session.pdrs_counter--;

  // Update session map.
  mpSessionsMap->update(session.seid, session, BPF_EXIST);

  LOG_DEBUG("FAR {} was remove at in session {}!", pFar->far_id.far_id, seid);
}

void SessionManager::removePDR(seid_t seid, std::shared_ptr<pfcp_pdr_t> pPdr)
{
  LOG_FUNC();
  pfcp_session_t session;

  // Lookup session based on seid.
  // TODO navarrothiago - check if session not exists.
  mpSessionsMap->lookup(seid, &session);

  // Check if the PDR's counter is equal to zero.
  if(session.pdrs_counter == 0) {
    LOG_ERROR("There is not any element in PDRs array. The PDR {0} cannot be deleted in the session {1}", pPdr->pdr_id.rule_id, seid);
    throw std::runtime_error("There is not any element in PDRs array. The PDR cannot be deleted in the session");
  }

  auto pPdrEnd = std::remove_if(session.pdrs, session.pdrs + session.pdrs_counter, [&](pfcp_pdr_t &pdr) { return pdr.pdr_id.rule_id == pPdr->pdr_id.rule_id; });

  // Check if PDR was found.
  if(session.pdrs + session.pdrs_counter == pPdrEnd) {
    LOG_ERROR("PDR {} not found", pPdr->pdr_id.rule_id);
    throw std::runtime_error("PDR not found");
  }

  // Move the new values to session address.
  std::move(session.pdrs, pPdrEnd, session.pdrs);

  // Update the counter.
  session.pdrs_counter--;

  // Update session map.
  mpSessionsMap->update(session.seid, session, BPF_EXIST);

  LOG_DEBUG("PDR {} was remove at in session {}!", pPdr->pdr_id.rule_id, seid);
}
