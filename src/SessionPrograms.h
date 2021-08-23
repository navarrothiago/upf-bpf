#ifndef __SESSIONPROGRAMS_H__
#define __SESSIONPROGRAMS_H__

#include <memory>
#include <FARProgram.h>
#include <unistd.h>
#include <next_prog_rule_key.h>

/**
 * @brief This class represents the datapath path. It stores the program related
 * to a PFCP session. For each session, there might be a QERProgram. The
 * FARProgram is mandatory.
 *
 */
class SessionPrograms
{
public:
  SessionPrograms(struct next_rule_prog_index_key key, std::shared_ptr<FARProgram> pFARProgram);
  virtual ~SessionPrograms();
  struct next_rule_prog_index_key getKey() const;
private:
  std::shared_ptr<FARProgram> mpFARProgram;
  struct next_rule_prog_index_key mKey;
};

#endif // __SESSIONPROGRAMS_H__
