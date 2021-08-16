#ifndef __USERPLANECOMPONENT_H__
#define __USERPLANECOMPONENT_H__

#include <bpf/libbpf.h> // enum libbpf_print_level
#include <memory>
#include <observer/OnStateChangeSessionProgramObserver.h>

class SessionManager;
class RulesUtilities;
class UPFProgram;
class SessionProgram;

/**
 * @brief User Plane component class to abstract the BPF Service Function Chain for mobile core network.
 *
 */
class UserPlaneComponent : public OnStateChangeSessionProgramObserver
{
public:
  /**
   * @brief Destroy the User Plane Component object.
   *
   */
  virtual ~UserPlaneComponent();
  /**
   * @brief Get the Instance object.
   *
   * @return The singleton instance.
   */
  static UserPlaneComponent &getInstance();
  /**
   * @brief Setup User Plane Component.
   * Used to setup all the program.
   *
   * @param pRulesUtilities
   * @param gtpInterface
   * @param udpInterface
   */
  void setup(std::shared_ptr<RulesUtilities> pRulesUtilities, const std::string& gtpInterface, const std::string& udpInterface);
  /**
   * @brief Tear down User Plane Component.
   * Tear down all programs that were setup.
   *
   */
  void tearDown();
  /**
   * @brief Get the Session Manager object.
   *
   * @return std::shared_ptr<SessionManager> The session manager reference.
   */
  std::shared_ptr<SessionManager> getSessionManager() const;
  /**
   * @brief Get the Rules Factory object.
   *
   * @return std::shared_ptr<RulesFactory> The rules factory reference.
   */
  std::shared_ptr<RulesUtilities> getRulesUtilities() const;
  /**
   * @brief Get UPFProgram object.
   *
   * @return std::shared_ptr<UPFProgram> The UPFProgram reference.
   */
  std::shared_ptr<UPFProgram> getUPFProgram() const;
  /**
   * @brief Get the GTP interface.
   *
   * @return std::string The GTP interface.
   */
  std::string getGTPInterface() const;
  /**
   * @brief Get UDP interface.
   *
   * @return std::string The UDP interface.
   */
  std::string getUDPInterface() const;

  // From onNewSessionProgramObserver.
  void onNewSessionProgram(u_int32_t programId, u_int32_t fileDescriptor) override;

  // From onNewSessionProgramObserver.
  void onDestroySessionProgram(u_int32_t programId) override;

  // TODO navarrothiago - getSessionManger?
private:
  /**
   * @brief Construct a new User Plane Component object.
   *
   * @param pRulesUtilities the wrapper for rules (PDR, FAR).
   */
  UserPlaneComponent();

  // Log function for libbpf. Do not used it!!
  static int printLibbpfLog(enum libbpf_print_level lvl, const char *fmt, va_list args);

  // The session manager reference.
  std::shared_ptr<SessionManager> mpSessionManager;

  // The rules factory reference.
  std::shared_ptr<RulesUtilities> mpRulesUtilities;

  // The UPFProgram (BPF program entry point) reference.
  std::shared_ptr<UPFProgram> mpUPFProgram;

  // The SessionProgram (BPF program for PFCP Session) reference.
  std::shared_ptr<SessionProgram> mpSessionProgram;

  // The GTP interface.
  std::string mGTPInterface;

  // The UDP interface.
  std::string mUDPInterface;
};

#endif // __USERPLANECOMPONENT_H__
