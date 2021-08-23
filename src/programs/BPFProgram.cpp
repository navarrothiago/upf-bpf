#include "BPFProgram.h"
#include <utils/LogDefines.h>

u_int32_t BPFProgram::sIdCounter = 1;
BPFProgram::BPFProgram(/* args */)
: mId(sIdCounter)
{
  LOG_FUNC();
  sIdCounter++;
  LOG_INF("BPFProgram {} created!!!", mId);
}

BPFProgram::~BPFProgram()
{
  LOG_FUNC();
}

uint32_t BPFProgram::getId() const
{
  LOG_FUNC();
  return mId;
}
