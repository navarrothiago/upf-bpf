#include "PacketDetectionRule.h"
#include <utils/LogDefines.h>

PacketDetectionRule::PacketDetectionRule(u32 precedence, std::shared_ptr<PacketDetectionInformation> pdi, u32 farId)
 : mPrecedence(precedence), mPDI(pdi), mFarId(farId)
{
  LOG_FUNC();

}

PacketDetectionRule::~PacketDetectionRule()
{
  LOG_FUNC();

}

u32 PacketDetectionRule::getPrecedence()
{
  LOG_FUNC();
  return mPrecedence;

}

std::shared_ptr<PacketDetectionInformation> PacketDetectionRule::getPDI()
{
  LOG_FUNC();
  return mPDI;

}

u32 PacketDetectionRule::getFarId()
{
  LOG_FUNC();
  return mFarId;

}
