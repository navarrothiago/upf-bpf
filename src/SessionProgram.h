#ifndef __SESSIONPROGRAM_H__
#define __SESSIONPROGRAM_H__

#include <memory>
#include <map>
#include <session_bpf_skel.h>
#include <ProgramLifeCycle.hpp>

using SessionProgramLifeCycle = ProgramLifeCycle<session_bpf_c>;

/**
 * @brief This class is used to interface with BPF program (Session).
 * 
 */
class SessionProgram
{
public:
  SessionProgram();
  /**
   * @brief Destroy the Session Program object.
   * 
   */
  virtual ~SessionProgram();
  /**
   * @brief Setup the program.
   * 
   * @param programId The identifier that will be used to locate the program in BPF map.
   */
  void setup(uint32_t programId);
  /**
   * @brief Tear down the program.
   * 
   */
  void tearDown();
  /**
   * @brief Get the File Descriptor (entry point) object.
   * 
   * @return int The file descriptor of the entry point program.
   */
  int getFileDescriptor() const;

private:
  // The BPF lifecycle program.
  std::unique_ptr<SessionProgramLifeCycle> mpLifeCycle;
};
#endif // __SESSIONPROGRAM_H__