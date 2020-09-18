#ifndef __SESSIONMANAGER_H__
#define __SESSIONMANAGER_H__

#include <ie/fseid.h>
#include <ie/pdr_id.h>
#include <memory>
#include <pfcp/pfcp_far.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>

class BPFMap;
class ForwardingActionRules;
class PacketDetectionRules;
class SessionBpf;

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
  void createSession(std::shared_ptr<SessionBpf> pSession);
  /**
   * @brief Remove a session object from BPF map.
   *
   * @param seid The session identifier representing the session to be removed.
   */
  void removeSession(uint64_t seid);
  /**
   * @brief Add the FAR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pFar The FAR to be added.
   */
  void addFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar);

  /**
   * @brief Add the PDR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pPdr The PDR to be added.
   */
  void addPDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr);
  /**
   * @brief Lookup FAR in the BPF map.
   *
   * @param seid The session identifier where will be lookup.
   * @param farId The FAR in the session to be lookup.
   * @return std::shared_ptr<ForwardingActionRules> The FAR found or NULL with not found.
   */
  std::shared_ptr<ForwardingActionRules> lookupFAR(uint64_t seid, uint32_t farId);
  /**
   * @brief Lookup PDR in the BPF map.
   *
   * @param seid The session identifier where will be lookup.
   * @param ruleId The rule identifier in the session to be lookup.
   * @return std::shared_ptr<pfcp_far_t> The PDR found or NULL with not found.
   */
  std::shared_ptr<PacketDetectionRules> lookupPDR(uint64_t seid, uint16_t ruleId);
  /**
   * @brief Update the FAR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pFar The FAR to be updated.
   */
  void updateFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar);
  /**
   * @brief Update the PDR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pPdr The PDR to be updated.
   */
  void updatePDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr);
  /**
   * @brief Remove the FAR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pFar The FAR to be removed.
   */
  void removeFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar);
  /**
   * @brief Remove the PDR in the BPF map.
   *
   * @param seid The session identifier.
   * @param pPdr The PDR to be removed.
   */
  void removePDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr);

private:
  // Store the abstraction of the BPF map.
  std::shared_ptr<BPFMap> mpSessionsMap;
};

#endif // __SESSIONMANAGER_H__
