#ifndef __PACKETDETECTIONINFORMATION_H__
#define __PACKETDETECTIONINFORMATION_H__

#include <types.h>

class PacketDetectionInformation
{
public:
enum SourceInterface{
  INTERFACE_VALUE_ACCESS = 0,
  INTERFACE_VALUE_CORE = 1,
  INTERFACE_VALUE_SGI_LAN_N6_LAN = 2,
  INTERFACE_VALUE_CP_FUNCTION = 3,
  INTERFACE_VALUE_5G_VN_INTERNAL = 4
};
  PacketDetectionInformation(u32 teid, u32 ueIpAddress, SourceInterface sourceInterface);
  virtual ~PacketDetectionInformation();
  u32 getTeid();
  u32 getUeIpAddress();
  u32 getSourceInterface();
private:
  u32 mTeid;
  u32 mUeIpAddress;
  SourceInterface mSourceInterface;
};

#endif // __PACKETDETECTIONINFORMATION_H__
