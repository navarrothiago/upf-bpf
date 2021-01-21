#include "BPFMaps.h"
#include <bpf/libbpf.h>
#include <cassert>
#include <string>

BPFMaps::BPFMaps(bpf_object_skeleton *pBPFObjectSkeleton)
: mpBPFObjectSkeleton(pBPFObjectSkeleton)
{
  // Check if there is at least one element.
  if(mpBPFObjectSkeleton->map_cnt >= 0) {
    for(unsigned int i = 0; i < mpBPFObjectSkeleton->map_cnt; i++) {
      std::string name(mpBPFObjectSkeleton->maps[i].name);
      mMaps.emplace_back(*mpBPFObjectSkeleton->maps[i].map, name);
    }
  }
}

BPFMaps::~BPFMaps() {}

BPFMap& BPFMaps::getMap(const char *pName)
{
  // Check if is not NULL.
  assert(pName != NULL);

  // Check if there is at least one element.
  assert(mpBPFObjectSkeleton->map_cnt);

  unsigned int i;
  std::string nameStr(pName);
  for(i = 0; i < mpBPFObjectSkeleton->map_cnt; i++) {
    std::string mapStr(mpBPFObjectSkeleton->maps[i].name);
    if(nameStr == mapStr) {
      return mMaps[i];
    }
  }
  LOG_ERROR("Map {} not found", pName)
  throw std::runtime_error("Map was not found");
}
