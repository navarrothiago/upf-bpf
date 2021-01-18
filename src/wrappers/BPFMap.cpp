#include "BPFMap.hpp"
#include <string>

BPFMap::BPFMap(struct bpf_map *pBPFMap, std::string name)
    : mpBPFMap(pBPFMap), mName(name)
{
}

BPFMap::~BPFMap() {}

std::string BPFMap::getName() const
{
  return mName;
}



