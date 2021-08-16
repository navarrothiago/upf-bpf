#include "PacketDetectionInformation.h"
#include <utils/LogDefines.h>

PacketDetectionInformation::PacketDetectionInformation(u32 teid, u32 ueIpAddress, SourceInterface sourceInterface)
  : mTeid(teid), mUeIpAddress(ueIpAddress), mSourceInterface(sourceInterface)
{
  LOG_FUNC();

}

PacketDetectionInformation::~PacketDetectionInformation()
{
  LOG_FUNC();

}

u32 PacketDetectionInformation::getTeid()
{
  LOG_FUNC();
  return mTeid;

}

u32 PacketDetectionInformation::getUeIpAddress()
{
  LOG_FUNC();
  return mUeIpAddress;

}

u32 PacketDetectionInformation::getSourceInterface()
{
  LOG_FUNC();
  return mSourceInterface;

}
