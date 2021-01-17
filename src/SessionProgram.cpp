#include "SessionProgram.h"
#include <UPFProgram.h>
#include <bpf/bpf.h>          // bpf calls
#include <bpf/libbpf.h>       // bpf wrappers
#include <sys/resource.h>     // rlimit
#include <utils/LogDefines.h> // Logs

SessionProgram::SessionProgram() {
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
  // initializeMaps();
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