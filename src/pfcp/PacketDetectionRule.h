#ifndef __PACKETDETECTIONRULE_H__
#define __PACKETDETECTIONRULE_H__

#include "PacketDetectionInformation.h"
#include <ie/far_id.h>
#include <memory.h>
#include <types.h>

class PacketDetectionRule
{
public:
  PacketDetectionRule(u32 precedence, std::shared_ptr<PacketDetectionInformation> mPDI, u32 farId);
  virtual ~PacketDetectionRule();
  u32 getPrecedence();
  std::shared_ptr<PacketDetectionInformation> getPDI();
  u32 getFarId();

private:
  u32 mPrecedence;
  std::shared_ptr<PacketDetectionInformation> mPDI;
  u32 mFarId;
};

#endif // __PACKETDETECTIONRULE_H__
