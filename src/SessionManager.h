#ifndef __SESSIONMANAGER_H__
#define __SESSIONMANAGER_H__

#include <ie/fseid.h>
#include <ie/pdr_id.h>
#include <memory>
#include <pfcp/pfcp_far.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>

class BPFMap;

// FIXME navarrothiago - This class compose the UPFProgram. When the program is deleted,
// this class must be updated.

/**
 * @brief This class abstracts the communication to manager the session BPF maps.
 * It communicate with BPF maps in order to update its PDRs and FARs.
 * This class does not validate the input.
 */
class SessionManager
{
public:
  /**
   * @brief Construct a new Session Manager object.
   *
   * @param pSessionsMap The sessions BPF map abstraction.
   */
  SessionManager(std::shared_ptr<BPFMap> pSessionsMap);
  /**
   * @brief Destroy the Session Manager object.
   *
   */
  virtual ~SessionManager();
  /**
   * @brief Create a Session object in BPF map.
   *
   * @param pSession The session object to be created.
   */
  void createSession(std::shared_ptr<pfcp_session_t> pSession);
  /**
   * @brief Remove a session object from BPF map.
   *
   * @param seid The session identifier representing the session to be removed.
   */
  void removeSession(seid_t seid);
  /**
   * @brief Add the FAR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pFar The FAR to be added.
   */
  void addFAR(seid_t seid, std::shared_ptr<pfcp_far_t> pFar);

  /**
   * @brief Add the PDR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pPdr The PDR to be added.
   */
  void addPDR(seid_t seid, std::shared_ptr<pfcp_pdr_t> pPdr);
  std::shared_ptr<pfcp_pdr_t> lookupPDR(seid_t seid, pdr_id_t pdrId);
  /**
   * @brief Update the FAR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pFar The FAR to be updated.
   */
  void updateFAR(seid_t seid, std::shared_ptr<pfcp_far_t> pFar);
  /**
   * @brief Update the PDR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pPdr The PDR to be updated.
   */
  void updatePDR(seid_t seid, std::shared_ptr<pfcp_pdr_t> pPdr);
  /**
   * @brief Remove the FAR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pFar The FAR to be removed.
   */
  void removeFAR(seid_t seid, std::shared_ptr<pfcp_far_t> pFar);
  /**
   * @brief Remove the PDR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pPdr The PDR to be removed.
   */
  void removePDR(seid_t seid, std::shared_ptr<pfcp_pdr_t> pPdr);

private:
  // Store the abstraction of the BPF map.
  std::shared_ptr<BPFMap> mpSessionsMap;
};

#endif // __SESSIONMANAGER_H__
