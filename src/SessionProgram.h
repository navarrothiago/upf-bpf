#ifndef __SESSIONPROGRAMMANAGER_H__
#define __SESSIONPROGRAMMANAGER_H__

#include <memory>
#include <map>
#include <session_bpf_skel.h>
#include <ProgramLifeCycle.hpp>

using SessionProgramLifeCycle = ProgramLifeCycle<session_bpf_c>;

class SessionProgram
{
public:
  virtual ~SessionProgram();
  void setup(uint32_t programId);
  void tearDown();

private:
  SessionProgram();
  std::map<uint32_t, struct session_bpf_c*> mBPFObjectMap;

  // The BPF lifecycle program.
  std::unique_ptr<SessionProgramLifeCycle> mpLifeCycle;
};
#endif // __SESSIONPROGRAMMANAGER_H__