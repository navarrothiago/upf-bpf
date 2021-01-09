#ifndef __SESSIONMANAGER_H__
#define __SESSIONMANAGER_H__

#include <UPFProgramManager.h>
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
  // Set of PDRs.
  using pdrs_t = std::vector<std::shared_ptr<PacketDetectionRules>>;
  /**
   * @brief Construct a new Session Manager object.
   *
   * @param pSessionsMap The sessions BPF map abstraction.
   * @param pUplinkPdrMap The uplink PDRs BPF map abstraction.
   */
  SessionManager(std::shared_ptr<BPFMap> pSessionsMap, std::shared_ptr<BPFMap> pUplinkPdrMap);
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
   * @brief Lookup PDRs by TEID in the BPF map.
   *
   * @param teid The tunnel endpoint identifier which will be used to lookup.
   * @return pdrs_t The PDRs found or empty with not found.
   */
  pdrs_t lookupPDRsUplink(uint32_t teid);
  /**
   * @brief Lookup PDRs by UE IP in the BPF map.
   *
   * @param ueIPAddress The IP of the user equipament which will be used to lookup.
   * @return pdrs_t The PDRs found or empty with not found.
   */
  pdrs_t lookupPDRsDownlink(uint32_t ueIPAddress);
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
  /**
   * @brief Add the PDR in the BPF map.
   * Use this method when the source_interface is ACCESS or CORE.
   *
   * @param key The key (e.g. TEID, UE IP Address) which will be used to add the PDR.
   * @param pdr The PDR to be added.
   * @param pBPFMap The BPF map which is related to the source_interface of the PDR.
   */
  template <class KeyType>
  void addPDR(KeyType key, pfcp_pdr_t_ &pdr, std::shared_ptr<BPFMap> pBPFMap);
  /**
   * @brief Update the PDR in the BPF map.
   * Use this method when the source_interface is ACCESS or CORE.
   *
   * @param key The key (e.g. TEID, UE IP Address) which will be used to update the PDR.
   * @param pdr The PDR to be updated.
   * @param pBPFMap The BPF map which is related to the source_interface of the PDR.
   */
  template <class KeyType>
  void updatePDR(KeyType key, pfcp_pdr_t_ &pdr, std::shared_ptr<BPFMap> pBPFMap);
  /**
   * @brief Remove the PDR in the BPF map.
   * Use this method when the source_interface is ACCESS or CORE.
   *
   * @param key The key which will be used add the PDR.
   * @param pdr The PDR to be removed.
   * @param pBPFMap The BPF map which is related to the source_interface of the PDR.
   */
  template <class KeyType>
  void removePDR(KeyType teid, pfcp_pdr_t_ &pdr, std::shared_ptr<BPFMap> pBPFMap);

  // Store the abstraction of the BPF map.
  std::shared_ptr<BPFMap> mpSessionsMap;
  std::shared_ptr<BPFMap> mpUplinkPDRsMap;
  std::shared_ptr<BPFMap> mpDownlinkPDRsMap;
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

  auto pUtils = UPFProgramManager::getInstance().getRulesUtilities();
  for(uint32_t i = 0; i < pPdrs->pdrs_counter; i++) {
    pPdr = pUtils->createPDR(&pPdrs->pdrs[i]);
    pdrs.push_back(pPdr);
  }

  return pdrs;
}

template <class KeyType>
void SessionManager::addPDR(KeyType key, pfcp_pdr_t_ &pdr, std::shared_ptr<BPFMap> pBPFMap)
{
  LOG_FUNC();
  pfcp_pdrs_t_ pdrs;
  memset(&pdrs, 0, sizeof(pfcp_pdr_t_));

  // If exists check the counter. If not, add the pdr to pdrs list.
  // The key could be TEID or the UE IP address.
  if(pBPFMap->lookup(key, &pdrs) == 0) {
    // Check the pdr counter.
    if(pdrs.pdrs_counter >= PDRS_MAX_SIZE) {
      LOG_ERROR("Array is full!! The PDR {} cannot be added in the key {}", pdr.pdr_id.rule_id, key);
      throw std::runtime_error("The PDR cannot be added in the key");
    }
  }

  LOG_DBG("There is {} PDRs available in the key {} (TEID||UE IP)->PDRs map", pdrs.pdrs_counter, key);

  // Add the element in the end of the pdrs array.
  // The next position is represented by the counter var.
  // Update the counter.
  uint32_t index = pdrs.pdrs_counter++;
  pdrs.pdrs[index] = pdr;

  // Update eBPF map.
  pBPFMap->update(key, pdrs, BPF_ANY);
  LOG_DBG("PDR {} was added at index {} in key {} at (TEID||UE IP)->PDRs map!", pdr.pdr_id.rule_id, index, key);
  LOG_DBG("There is {} PDRs available in the key {} (TEID||UE IP)->PDRs map", pdrs.pdrs_counter, key);
}

template <class KeyType>
void SessionManager::updatePDR(KeyType key, pfcp_pdr_t_ &pdr, std::shared_ptr<BPFMap> pBPFMap)
{
  LOG_FUNC();
  pfcp_pdrs_t_ pdrs;
  memset(&pdrs, 0, sizeof(pfcp_pdr_t_));

  // If not exists, error. PDR not found.
  // The key could be TEID or the UE IP address.
  if(pBPFMap->lookup(key, &pdrs) != 0) {
    LOG_ERROR("PDR {} not exists in the key {}", pdr.pdr_id.rule_id, key);
    throw std::runtime_error("The PDR not exists in the key");
  }

  LOG_DBG("There is {} PDRs available in the key {} (TEID||UE IP)->PDRs map", pdrs.pdrs_counter, key);

  // Remove the element in the end of the pdrs array.
  // The next position is represented by the counter var.
  // Update the counter.
  uint32_t index = pdrs.pdrs_counter++;
  pdrs.pdrs[index] = pdr;

  // Update eBPF map.
  pBPFMap->update(key, pdrs, BPF_ANY);
  LOG_DBG("PDR {} was updated at index {} in key {} at (TEID||UE IP)->PDRs map!", pdr.pdr_id.rule_id, index, key);
  LOG_DBG("There is {} PDRs available in the key {} (TEID||UE IP)->PDRs map", pdrs.pdrs_counter, key);
}
template <class KeyType>
void SessionManager::removePDR(KeyType key, pfcp_pdr_t_ &pdr, std::shared_ptr<BPFMap> pBPFMap)
{
  LOG_FUNC();
  pfcp_pdrs_t_ pdrs;
  memset(&pdrs, 0, sizeof(pfcp_pdr_t_));

  // If exists check the counter. If not, add the pdr to pdrs list.
  // The key could be TEID or the UE IP address.
  if(pBPFMap->lookup(key, &pdrs) != 0) {
    // Error, removing PDR that does not exist.
    LOG_ERROR("Array is empty!! The PDR {} cannot be removed in the key {}", pdr.pdr_id.rule_id, key);
    throw std::runtime_error("The PDR cannot be added in the key");
  }

  LOG_DBG("There is {} PDRs available in the key {} (TEID||UE IP)->PDRs map", pdrs.pdrs_counter, key);

  // Remove the element. Only update the counter.
  uint32_t index = pdrs.pdrs_counter--;

  if(pdrs.pdrs_counter == 0) {
    LOG_DBG("Free memory, remove PDRs array for the expecific key.");
    pBPFMap->remove(key);
  } else {
    // Update eBPF map.
    pBPFMap->update(key, pdrs, BPF_ANY);
  }
  LOG_DBG("PDR was removed at index {} in key {} at (TEID||UE IP)->PDRs map!", pdr.pdr_id.rule_id, index, key);
  LOG_DBG("There is {} PDRs available in the key {} (TEID||UE IP)->PDRs map", pdrs.pdrs_counter, key);
}

#endif // __SESSIONMANAGER_H__
