#include "SessionProgram.h"
#include <UPFProgram.h>
#include <bpf/bpf.h>          // bpf calls
#include <bpf/libbpf.h>       // bpf wrappers
#include <sys/resource.h>     // rlimit
#include <utils/LogDefines.h> // Logs

SessionProgram::SessionProgram() { LOG_FUNC(); }
// SessionProgram& SessionProgram::getInstance()
// {
//   LOG_FUNC();
//   static SessionProgram sInstance;
//   return sInstance;
// }

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

  // UPFProgram::getInstance().updateProgMap(sessionId, fd);
  // mBPFObjectMap.insert(std::pair<uint32_t, bpf_object*>(sessionId, mpSessionBPFObject));
}