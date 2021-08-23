#include "SessionPrograms.h"
#include <utils/LogDefines.h>

SessionPrograms::SessionPrograms(struct next_rule_prog_index_key key, std::shared_ptr<FARProgram> pFARProgram)
  : mKey(key), mpFARProgram(pFARProgram)
{
  LOG_FUNC();
}

SessionPrograms::~SessionPrograms()
{
  LOG_FUNC();
  mpFARProgram->tearDown();
}

struct next_rule_prog_index_key SessionPrograms::getKey() const
{
  LOG_FUNC();
  return mKey;
}
