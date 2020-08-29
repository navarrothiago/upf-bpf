#ifndef __SESSIONMANAGER_H__
#define __SESSIONMANAGER_H__

#include <memory>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_far.h>
#include <pfcp/pfcp_session.h>
#include <ie/fseid.h>

class BPFMap;

/**
 * @brief This class abstracts the communication to manager the session BPF maps.
 * It communicate with BPF maps in order to update its PDRs and FARs.
 * This class does not validate the input.
 */
class SessionManager
{
public:
  /**
   * @brief Get the Instance object.
   *
   * @return SessionManager& The singleton reference.
   */
  static SessionManager &getInstance();
  /**
   * @brief Destroy the Session Manager object
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
  /**
   * @brief Construct a new Session Manager object.
   *
   */
  SessionManager();

  // Store the abstraction of the BPF map.
  std::shared_ptr<BPFMap> mpSessionsMap;
};

#endif // __SESSIONMANAGER_H__
