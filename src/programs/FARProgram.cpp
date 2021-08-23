#include "FARProgram.h"
#include <SessionManager.h>
#include <bpf/bpf.h>       // bpf calls
#include <bpf/libbpf.h>    // bpf wrappers
#include <iostream>        // cout
#include <stdexcept>       // exception
#include <utils/LogDefines.h>
#include <wrappers/BPFMap.hpp>
#include <wrappers/BPFMaps.h>

FARProgram::FARProgram()
 : BPFProgram()
{
  LOG_FUNC();
  mpLifeCycle = std::make_shared<FARProgramLifeCycle>(far_bpf_c__open, far_bpf_c__load, far_bpf_c__attach, far_bpf_c__destroy);
}

FARProgram::~FARProgram()
{
  LOG_FUNC();
}

void FARProgram::setup()
{
  LOG_FUNC();

  spSkeleton = mpLifeCycle->open();
  initializeMaps();
  mpLifeCycle->load();
  mpLifeCycle->attach();

  LOG_DBG("Configure redirect interface");
  auto udpInterface = UserPlaneComponent::getInstance().getUDPInterface();
  auto gtpInterface = UserPlaneComponent::getInstance().getGTPInterface();
  uint32_t udpInterfaceIndex = if_nametoindex(udpInterface.c_str());
  uint32_t gtpInterfaceIndex = if_nametoindex(gtpInterface.c_str());
  uint32_t uplinkId = static_cast<uint32_t>(FlowDirection::UPLINK);
  uint32_t downlinkId = static_cast<uint32_t>(FlowDirection::DOWNLINK);
  mpEgressInterfaceMap->update(uplinkId, udpInterfaceIndex, BPF_ANY);
  mpEgressInterfaceMap->update(downlinkId, gtpInterfaceIndex, BPF_ANY);
}

std::shared_ptr<BPFMaps> FARProgram::getMaps()
{
  LOG_FUNC();
  return mpMaps;
}

// TODO navarrothiago - check when kill when running.
// It was noted the infinity loop.
void FARProgram::tearDown()
{
  LOG_FUNC();
  mpLifeCycle->tearDown();
}

std::shared_ptr<BPFMap> FARProgram::getFARMap() const
{
  LOG_FUNC();
  return mpFARMap;
}

std::shared_ptr<BPFMap> FARProgram::getEgressInterfaceMap() const
{
  LOG_FUNC();
  return mpEgressInterfaceMap;
}

int FARProgram::getFd() const
{
  LOG_FUNC();
  return bpf_program__fd(mpLifeCycle->getBPFSkeleton()->progs.far_entry_point);
}

void FARProgram::initializeMaps()
{
  LOG_FUNC();
  // Store all maps available in the program.
  mpMaps = std::make_shared<BPFMaps>(mpLifeCycle->getBPFSkeleton()->skeleton);

  // Warning - The name of the map must be the same of the BPF program.
  mpFARMap = std::make_shared<BPFMap>(mpMaps->getMap("m_far"));
  mpEgressInterfaceMap = std::make_shared<BPFMap>(mpMaps->getMap("m_redirect_interfaces"));
}
