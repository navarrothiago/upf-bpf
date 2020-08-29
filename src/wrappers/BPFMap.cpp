#include "BPFMap.hpp"

BPFMap::BPFMap(struct bpf_map *pBPFMap)
    : mpBPFMap(pBPFMap)
{
}

BPFMap::~BPFMap() {}


