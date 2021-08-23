#ifndef __SESSIONPROGRAM_H__
#define __SESSIONPROGRAM_H__

#include <memory>
#include <map>
#include <session_bpf_skel.h>
#include <ProgramLifeCycle.hpp>

using SessionProgramLifeCycle = ProgramLifeCycle<session_bpf_c>;

class BPFMaps;
class BPFMap;

/**
 * @brief This class is used to interface with BPF program (Session).
 *
 */
class SessionProgram
{
public:
  /**
   * @brief Construct a new Session Program object.
   *
   */
  SessionProgram(const std::string& gtpInterface, const std::string& udpInterface);
  /**
   * @brief Destroy the Session Program object.
   *
   */
  virtual ~SessionProgram();
  /**
   * @brief Setup the program.
   *
   */
  void setup();
  /**
   * @brief Tear down the program.
   *
   */
  void tearDown();
  /**
   * @brief Get the Downlink File Descriptor (entry point) object.
   *
   * @return int The file descriptor of the entry point program.
   */
  int getUplinkFileDescriptor() const;
  /**
   * @brief Get the Uplink File Descriptor (entry point) object.
   *
   * @return int The file descriptor of the entry point program.
   */
  int getDownlinkFileDescriptor() const;
  /**
   * @brief Get PDR maps reference.
   *
   * @return std::shared_ptr<BPFMap> The PDR map reference.
   */
  std::shared_ptr<BPFMap> getPDRMap() const;
  /**
   * @brief Get the FAR Map object.
   *
   * @return std::shared_ptr<BPFMap> The FAR map.
   */
  std::shared_ptr<BPFMap> getFARMap() const;
  /**
   * @brief Get the Uplink PDRs Map object.
   *
   * @return std::shared_ptr<BPFMap> The uplink to PDR map.
   */
  std::shared_ptr<BPFMap> getUplinkPDRsMap() const;
  /**
   * @brief Get the Downlink PDRs Map object.
   *
   * @return std::shared_ptr<BPFMap> The uplink to PDR map.
   */
  std::shared_ptr<BPFMap> getDownlinkPDRsMap() const;
  /**
   * @brief Get the Counter Map object.
   *
   * @return std::shared_ptr<BPFMap> The counter map.
   */
  std::shared_ptr<BPFMap> getCounterMap() const;
  /**
   * @brief Get the Egress Interface Map object.
   *
   * @return std::shared_ptr<BPFMap> The egress interface map.
   */
  std::shared_ptr<BPFMap> getEgressInterfaceMap() const;
  /**
   * @brief Get the Arp Table Map object.
   *
   * @return std::shared_ptr<BPFMap>  The arp table map.
   */
  std::shared_ptr<BPFMap> getArpTableMap() const;

private:
  /**
   * @brief Initialize BPF maps wrappers references.
   *
   */
  void initializeMaps();

  // The reference of the bpf maps.
  std::shared_ptr<BPFMaps> mpMaps;

  // The PDR map reference.
  std::shared_ptr<BPFMap> mpPDRMap;

  // The PDR map reference.
  std::shared_ptr<BPFMap> mpFARMap;

  // The uplink PDR eBPF map.
  std::shared_ptr<BPFMap> mpUplinkPDRsMap;

  // The downlink PDR eBPF map.
  std::shared_ptr<BPFMap> mpDownlinkPDRsMap;

  // The counter packet map.
  std::shared_ptr<BPFMap> mpCounterMap;

  // The egress interface map.
  std::shared_ptr<BPFMap> mpEgressInterfaceMap;

  // The arp table map.
  std::shared_ptr<BPFMap> mpArpTableMap;

  // The BPF lifecycle program.
  std::shared_ptr<SessionProgramLifeCycle> mpLifeCycle;

  // The GTP interface.
  std::string mGTPInterface;

  // The UDP interface.
  std::string mUDPInterface;
};
#endif // __SESSIONPROGRAM_H__
