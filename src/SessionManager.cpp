#include "SessionManager.h"
#include <UPFProgramManager.h>
#include <pfcp/pfcp_session.h>
#include <utils/LogDefines.h>
#include <wrappers/BPFMap.hpp>
#include <wrappers/BPFMaps.h>

SessionManager &SessionManager::getInstance()
{
  LOG_FUNC();
  static SessionManager sInstance;
  return sInstance;
}

SessionManager::~SessionManager() { LOG_FUNC(); }

void SessionManager::createSession(std::shared_ptr<pfcp_session_t> pSession)
{
  LOG_FUNC();
  if(mpSessionsMap->update(pSession->seid, *pSession, BPF_NOEXIST) != 0){
    LOG_ERROR("Cannot create session {}", pSession->seid);
    throw std::runtime_error("Cannot create session");
  }
}

void SessionManager::removeSession(seid_t seid)
{
  LOG_FUNC();
  if(mpSessionsMap->remove(seid) != 0){
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
    LOG_ERROR("The FAR {0} cannot be added in the session {1}", pFar->far_id.far_id, seid);
    throw std::runtime_error("The FAR cannot be added in the session");
  }

  // Insert the element in the end of the fars array.
  // The next position is represented by the counter var.
  // Update the counter.
  uint32_t index = session.fars_counter++;
  session.fars[index] = *pFar;

  // Update session.
  mpSessionsMap->update(seid, session, BPF_NOEXIST);
  LOG_DEBUG("FAR {} was inserted at index {} in session {}!", pFar->far_id.far_id, index, seid);
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
    LOG_ERROR("The PDR {0} cannot be added in the session {1}", pPdr->pdr_id.rule_id, seid);
    throw std::runtime_error("The PDR cannot be added in the session");
  }

  // Insert the element in the end of the pdrs array.
  // The next position is represented by the counter var.
  // Update the counter.
  uint32_t index = session.pdrs_counter++;
  session.pdrs[index] = *pPdr;

  // Update session.
  mpSessionsMap->update(seid, session, BPF_NOEXIST);
  LOG_DEBUG("PDR {} was inserted at index {} in session {}!", pPdr->pdr_id.rule_id, index, seid);
}

SessionManager::SessionManager()
{
  LOG_FUNC();
  // Warning - The name of the map must be the same of the BPF program.
  mpSessionsMap = std::make_shared<BPFMap>(UPFProgramManager::getInstance().getMaps()->getMap("m_seid_session"));
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
  uint32_t i;
  for(uint32_t i = 0; i < session.pdrs_counter; i++) {
    if(session.pdrs[i].pdr_id.rule_id == pPdr->pdr_id.rule_id) {

      // Update all fields.
      session.pdrs[i] = *pPdr;

      // Update session in BPF map.
      mpSessionsMap->update(seid, session, BPF_EXIST);
      LOG_DEBUG("PDR {} was update at index {} in session {}!", pPdr->pdr_id.rule_id, i, seid);
      return;
    }
  }
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

  // Look for the FAR in the array.
  uint32_t i;
  for(uint32_t i = 0; i < session.fars_counter; i++) {
    if(session.fars[i].far_id.far_id == pFar->far_id.far_id) {

      // Delete session in BPF map.
      mpSessionsMap->remove(seid);
      LOG_DEBUG("FAR {} was remove at index {} in session {}!", pFar->far_id.far_id, i, seid);
      return;
    }
  }
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

  // Look for the PDR in the array.
  uint32_t i;
  for(uint32_t i = 0; i < session.pdrs_counter; i++) {
    if(session.pdrs[i].pdr_id.rule_id == pPdr->pdr_id.rule_id) {

      // Delete session in BPF map.
      mpSessionsMap->remove(seid);
      LOG_DEBUG("PDR {} was remove at index {} in session {}!", pPdr->pdr_id.rule_id, i, seid);
    }
  }
}
