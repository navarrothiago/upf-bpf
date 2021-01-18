#include "SessionProgram.h"
#include <UPFProgram.h>
#include <bpf/bpf.h>          // bpf calls
#include <bpf/libbpf.h>       // bpf wrappers
#include <sys/resource.h>     // rlimit
#include <utils/LogDefines.h> // Logs
#include <wrappers/BPFMaps.h>
#include <wrappers/BPFMap.hpp>

SessionProgram::SessionProgram() 
{
  LOG_FUNC();
  mpLifeCycle = std::make_unique<SessionProgramLifeCycle>(session_bpf_c__open, session_bpf_c__load, session_bpf_c__attach, session_bpf_c__destroy);
}

SessionProgram::~SessionProgram()
{
  LOG_FUNC();
}

void SessionProgram::setup(uint32_t sessionId)
{
  LOG_FUNC();

  mpLifeCycle->open();
  initializeMaps();
  mpLifeCycle->load();
  mpLifeCycle->attach();
 
}

void SessionProgram::tearDown() 
{
  LOG_FUNC();
  mpLifeCycle->destroy();
}

int SessionProgram::getFileDescriptor() const
{
  LOG_FUNC();
  return bpf_program__fd(mpLifeCycle->getBPFSkeleton()->progs.entry_point);
}

std::shared_ptr<BPFMap> SessionProgram::getPDRMap() const
{
  LOG_FUNC();
  return mpPDRMap;
}

std::shared_ptr<BPFMap> SessionProgram::getFARMap() const
{
  LOG_FUNC();
  return mpFARMap;
}

std::shared_ptr<BPFMap> SessionProgram::getUplinkPDRsMap() const
{
  LOG_FUNC();
  return mpUplinkPDRsMap;
}

void SessionProgram::initializeMaps()
{
  LOG_FUNC();
  // Store all maps available in the program.
  mpMaps = std::make_unique<BPFMaps>(mpLifeCycle->getBPFSkeleton()->skeleton);

  // Warning - The name of the map must be the same of the BPF program.
  // Initialize maps.
  mpPDRMap = std::make_shared<BPFMap>(mpMaps->getMap("m_pdrs"));
  mpFARMap = std::make_shared<BPFMap>(mpMaps->getMap("m_fars"));
  mpUplinkPDRsMap = std::make_shared<BPFMap>(mpMaps->getMap("m_teid_pdrs"));
}