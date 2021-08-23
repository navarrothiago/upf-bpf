#include "BPFMap.hpp"
#include <string>

// TODO navarrothiago - KISS, pass bpf_map_skeleton.
BPFMap::BPFMap(struct bpf_map *pBPFMap, std::string name)
    : mpBPFMap(pBPFMap), mName(name)
{
  // Do not put LOG_FUNC() here.
  // LOG_DBG("{} map was initialize", mName);
}

BPFMap::~BPFMap()
{
  // Do not put LOG_FUNC() here.
  // LOG_DBG("{} map was destroyed", mName);
}

std::string BPFMap::getName() const
{
  // Do not put LOG_FUNC() here.
  return mName;
}



