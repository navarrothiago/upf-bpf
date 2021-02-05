#ifndef __BPFMAPS_H__
#define __BPFMAPS_H__

#include <vector>
#include <wrappers/BPFMap.hpp>

/**
 * @brief It is used to retrieve all the BPF maps which are stores in skeletons.
 *
 */
class BPFMaps
{
public:
  /**
   * @brief Construct a new BPFMaps object.
   *
   * @param pBPFObjectSkeleton The bpf skeleton.
   */
  BPFMaps(bpf_object_skeleton *pBPFObjectSkeleton);
  /**
   * @brief Destroy the BPFMaps object.
   *
   */
  virtual ~BPFMaps();
  /**
   * @brief Get the BPFMap object.
   *
   * @param pName The name of the BPF map.
   * @return BPFMap& The abstraction of the map.
   */
  BPFMap &getMap(const char *pName);

  BPFMaps(const BPFMaps &) = delete;
  BPFMaps &operator=(const BPFMaps &) = delete;

private:
  // The bpf skeleton.
  bpf_object_skeleton *mpBPFObjectSkeleton;

  // The reference of all maps in bpf program.
  // TODO navarrothiago - Change to hash map.
  std::vector<BPFMap> mMaps;
};

#endif // __BPFMAPS_H__
