#ifndef __BPFMAP_H__
#define __BPFMAP_H__

#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <utils/LogDefines.h>

/**
 * @brief This class abstract the communication with a specific BPF map.
 */
class BPFMap
{
public:
  /**
   * @brief Construct a new BPFMap object.
   *
   * @param pBPFMap The bpf map compatible with libbpf.
   */
  BPFMap(struct bpf_map *pBPFMap);
  /**
   * @brief Destroy the BPFMap object.
   */
  virtual ~BPFMap();
  /**
   * @brief Lookup a element in a specific position.
   * Wrappers the bpf_map_lookup function.
   *
   * @param key The key to be used to find the element.
   * @param pValue The element found.
   * @return 0 if it is success, cc. != 0.
   */
  template <class KeyType>
  int lookup(KeyType &key, void *pValue);
  /**
   * @brief Update a element in a specific position.
   * Wrappers the bpf_map_update function.
   *
   * @param key The key to be used to find the element.
   * @param value The new element.
   * @param flags
   * @return 0 if it is success, cc. != 0.
   */
  template <class KeyType, class ValueType>
  int update(KeyType &key, ValueType &value, int flags);
  /**
   * @brief Remove a element in a specific position.
   * Wrappers the bpf_map_delete function.
   *
   * @param key The key to be deleted.
   * @return 0 if it is success, cc. != 0.
   */
  template <class KeyType>
  int remove(KeyType &key);

private:
  // TODO navarrothiago - Change to unique.
  // The bpf map.
  struct bpf_map *mpBPFMap;
};

template <class KeyType>
int BPFMap::lookup(KeyType &key, void *pValue)
{
  LOG_FUNC();
  int mapFd = bpf_map__fd(mpBPFMap);
  int lookupReturn = bpf_map_lookup_elem(mapFd, &key, pValue);

  if(lookupReturn != 0) {
    perror("Lookup error");
  }
  return lookupReturn;
}

template <class KeyType, class ValueType>
int BPFMap::update(KeyType &key, ValueType &value, int flags)
{
  LOG_FUNC();
  int mapFd = bpf_map__fd(mpBPFMap);
  int updateReturn = bpf_map_update_elem(mapFd, &key, &value, flags);

  if(updateReturn != 0) {
    perror("Update error");
  }
  return updateReturn;
}

template <class KeyType>
int BPFMap::remove(KeyType &key)
{
  int mapFd = bpf_map__fd(mpBPFMap);
  int deleteReturn = bpf_map_delete_elem(mapFd, &key);

  if(deleteReturn != 0) {
    perror("Delete error");
  }

  return deleteReturn;
}

#endif // __BPFMAP_H__
