#ifndef __SESSIONMANAGER_H__
#define __SESSIONMANAGER_H__

#include <UserPlaneComponent.h>
#include <ie/fseid.h>
#include <ie/pdr_id.h>
#include <interfaces/RulesUtilities.h>
#include <memory>
#include <pfcp/pfcp_far.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_pdrs.h>
#include <pfcp/pfcp_session.h>
#include <vector>
#include <wrappers/BPFMap.hpp>

class BPFMap;
class ForwardingActionRules;
class PacketDetectionRules;
class SessionBpf;

// FIXME navarrothiago - This class compose the UserPlaneComponent. When the program is deleted,
// this class must be updated.

// TODO navarrothiago - SessionRequestHandler? SessionMessageHandler?

// TODO navarrothiago - Decouple PDR and FAR management.

/**
 * @brief This class abstracts the communication to manager the session BPF maps.
 * It communicate with BPF maps in order to update its PDRs and FARs.
 * This class does not validate the input.
 */
class SessionManager
{
public:
  // Set of PDRs.
  using pdrs_t = std::vector<std::shared_ptr<PacketDetectionRules>>;
  /**
   * @brief Construct a new Session Manager object.
   *
   */
  SessionManager();
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
   * @param seid The session endpoint identifier representing the session to be removed.
   */
  void removeSession(uint64_t seid);
  /**
   * @brief Add the FAR in the BPF map.
   *
   * @param seid The session endpoint identifier.
   * @param pFar The FAR to be added.
   */
  void addFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar);
  /**
   * @brief Add the PDR in the BPF map.
   * It is inserted in session map and the UL/DL PDR map.
   * The idea is to to check the PDI source interface in order to insert in
   * the correct maps.
   *
   * If the value is ACCESS, then insert in UL PDR map.
   * If the value is CORE, then  insert in DL PDR map.
   *
   * @param seid The session endpoint identifier.
   * @param pPdr The PDR to be added.
   */
  void addPDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr);
  /**
   * @brief Lookup FAR in the BPF map.
   *
   * @param seid The session endpoint identifier where will be lookup.
   * @param farId The FAR in the session to be lookup.
   * @return std::shared_ptr<ForwardingActionRules> The FAR found or NULL with not found.
   */
  std::shared_ptr<ForwardingActionRules> lookupFAR(uint64_t seid, uint32_t farId);
  /**
   * @brief Lookup PDR in the BPF map.
   *
   * @param seid The session endpoint identifier where will be lookup.
   * @param ruleId The rule identifier in the session to be lookup.
   * @return std::shared_ptr<pfcp_far_t> The PDR found or NULL with not found.
   */
  std::shared_ptr<PacketDetectionRules> lookupPDR(uint64_t seid, uint16_t ruleId);
  /**
   * @brief Update the FAR in the BPF map.
   *
   * @param seid The session endpoint identifier.
   * @param pFar The FAR to be updated.
   */
  void updateFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar);
  /**
   * @brief Update the PDR in the BPF map.
   *
   * @param seid The session endpoint identifier.
   * @param pPdr The PDR to be updated.
   */
  void updatePDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr);
  /**
   * @brief Remove the FAR in the BPF map.
   *
   * @param seid The session endpoint identifier.
   * @param pFar The FAR to be removed.
   */
  void removeFAR(uint64_t seid, std::shared_ptr<ForwardingActionRules> pFar);
  /**
   * @brief Remove the PDR in the BPF map.
   *
   * @param seid The session endpoint identifier.
   * @param pPdr The PDR to be removed.
   */
  void removePDR(uint64_t seid, std::shared_ptr<PacketDetectionRules> pPdr);

private:
  /**
   * @brief Lookup PDRs by key (UE IP address or TEID) in the BPF map.
   *
   * @param key The UE IP address or TEID which will be used to lookup.
   * @return pdrs_t The PDRs found or empty with not found.
   */
  template <class KeyType>
  pdrs_t lookupPDRs(KeyType key, std::shared_ptr<BPFMap> pBPFMap);
  // Store the abstraction of the BPF map.
  std::shared_ptr<BPFMap> mpDownlinkPDRsMap;
  std::shared_ptr<BPFMap> mpProgramsMap;
  std::shared_ptr<BPFMap> mpFARsMap;
};

// TODO navarrothiago - Return boolean?
template <class KeyType>
SessionManager::pdrs_t SessionManager::lookupPDRs(KeyType key, std::shared_ptr<BPFMap> pBPFMap)
{
  LOG_FUNC();
  std::shared_ptr<pfcp_pdrs_t_> pPdrs = std::make_shared<pfcp_pdrs_t_>();
  std::shared_ptr<PacketDetectionRules> pPdr;
  pdrs_t pdrs;

  // Lookup PDRs based on UE IP address or TEID.
  // use *.get() rather than & to get the pointer held by the smart pointer.
  pBPFMap->lookup(key, pPdrs.get());

  if(pPdrs->pdrs_counter == 0) {
    // Empty PDR.
    LOG_WARN("There is no PDRs in key {}", key);
    return pdrs;
  }

  auto pUtils = UserPlaneComponent::getInstance().getRulesUtilities();
  for(uint32_t i = 0; i < pPdrs->pdrs_counter; i++) {
    pPdr = pUtils->createPDR(&pPdrs->pdrs[i]);
    pdrs.push_back(pPdr);
  }

  return pdrs;
}

#endif // __SESSIONMANAGER_H__
