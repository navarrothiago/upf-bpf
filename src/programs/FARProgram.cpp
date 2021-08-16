#include "FARProgram.h"
#include <SessionManager.h>
#include <bpf/bpf.h>       // bpf calls
#include <bpf/libbpf.h>    // bpf wrappers
#include <iostream>        // cout
#include <stdexcept>       // exception
#include <utils/LogDefines.h>
#include <wrappers/BPFMap.hpp>
#include <wrappers/BPFMaps.h>

FARProgram::FARProgram(const std::string& gtpInterface, const std::string& udpInterface)
{
  LOG_FUNC();
  mpLifeCycle = std::make_unique<FARProgramLifeCycle>(far_bpf_c__open, far_bpf_c__load, far_bpf_c__attach, far_bpf_c__destroy);
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

void FARProgram::initializeMaps()
{
  LOG_FUNC();
  // Store all maps available in the program.
  mpMaps = std::make_unique<BPFMaps>(mpLifeCycle->getBPFSkeleton()->skeleton);

  // Warning - The name of the map must be the same of the BPF program.
  mpFARMap = std::make_shared<BPFMap>(mpMaps->getMap("m_far"));
}
