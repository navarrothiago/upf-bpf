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

#include <msg_pfcp.hpp>
#include "3gpp_29.244.hpp"
#include <pfcp_session.hpp>
#include <unordered_map>

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
   * @brief  Creates BPF pipeline.
   *
   * @param pSession The PFCP session which contains the context that will be deployed.
   */
  void createBPFSession(std::shared_ptr<pfcp::pfcp_session> pSession);
  /**
   * @brief Remove BPF pipeline.
   *
   * @param seid  The PFCP session which contains the context that will be removed.
   */
  void removeBPFSession(uint64_t seid);

  std::unordered_map<uint64_t, std::shared_ptr<pfcp::pfcp_session>> mSeidToSession;
  static bool comparePDR(const std::shared_ptr<pfcp::pfcp_pdr>& first, const std::shared_ptr<pfcp::pfcp_pdr>& second);
};

#endif // __SESSIONMANAGER_H__
