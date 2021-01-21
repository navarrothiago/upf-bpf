#ifndef __PROGRAMLIFECYCLE_H__
#define __PROGRAMLIFECYCLE_H__

#include <atomic>
#include <functional>     // rlimit
#include <sstream>        //stringstream
#include <sys/resource.h> // rlimit
#include <net/if.h>        // if_nametoindex
#include <linux/if_link.h> // XDP flags
#include <bpf/libbpf.h>    //bpf function
#include <utils/LogDefines.h>
#include <map>

/**
 * @brief Program states.
 */
enum ProgramState {
  IDLE,
  OPENED,
  LOADED,
  ATTACHED,
  LINKED,
  DESTROYED,
};

enum HookState{
  OFF,
  HOOKED
};

template <class BPFSkeletonType>
class ProgramLifeCycle
{
public:
  /**
   * @brief Construct a new Program Life Cycle object.
   *
   * @param openFunc
   * @param loadFunc
   * @param attachFunc
   * @param destroyFunc
   */
  ProgramLifeCycle(std::function<BPFSkeletonType *()> openFunc, std::function<int(BPFSkeletonType *)> loadFunc, std::function<int(BPFSkeletonType *)> attachFunc, std::function<void(BPFSkeletonType *)> destroyFunc);
  /**
   * @brief Destroy the Program Life Cycle object.
   *
   */
  virtual ~ProgramLifeCycle();
  /**
   * @brief Open the BPF program.
   *
   */
  BPFSkeletonType *open();
  /**
   * @brief Load the BPF program.
   *
   */
  void load();
  /**
   * @brief Attach the BPF program.
   *
   */
  void attach();
  /**
   * @brief Link The BPF XDP program.
   * The BPF program must be only link to one interface.
   * See RedHat presentation.
   *
   */
  void link(std::string sectionName, std::string interface);
  /**
   * @brief Destroy the BPF program.
   *
   */
  void destroy();
  void tearDown();
  /**
   * @brief Get BPF skeleton constructed.
   *
   * @return The skeleton reference. NULL if is not opened.
   */
  BPFSkeletonType *getBPFSkeleton() const;
  /**
   * @brief Get the State object.
   *
   * @return The BPF program state.
   */
  ProgramState getState() const;

private:
  // Mutex for tearDown (async).
  std::mutex sTearDownMutex;
  // The program state.
  std::atomic<ProgramState> mState;
  std::map<std::string, HookState> mSectionHookStateMap;
  std::map<std::string, uint32_t> mSectionIfaceMap;
  std::function<BPFSkeletonType *()> mOpenFunc;
  std::function<int(BPFSkeletonType *)> mLoadFunc;
  std::function<int(BPFSkeletonType *)> mAttachFunc;
  std::function<void(BPFSkeletonType *)> mDestroyFunc;
  BPFSkeletonType *mpSkeleton;
};

template <class BPFSkeletonType>
ProgramLifeCycle<BPFSkeletonType>::ProgramLifeCycle(std::function<BPFSkeletonType *()> openFunc, std::function<int(BPFSkeletonType *)> loadFunc, std::function<int(BPFSkeletonType *)> attachFunc, std::function<void(BPFSkeletonType *)> destroyFunc)
    : mOpenFunc(openFunc)
    , mLoadFunc(loadFunc)
    , mAttachFunc(attachFunc)
    , mDestroyFunc(destroyFunc)
{
  LOG_FUNC();
}

template <class BPFSkeletonType>
ProgramLifeCycle<BPFSkeletonType>::~ProgramLifeCycle()
{
  LOG_FUNC();
}

template <class BPFSkeletonType>
BPFSkeletonType *ProgramLifeCycle<BPFSkeletonType>::open()
{
  LOG_FUNC();
  struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
  if(setrlimit(RLIMIT_MEMLOCK, &r)) {
    perror("setrlimit(RLIMIT_MEMLOCK)");
    throw std::runtime_error("Cannot change bpf limit program");
  }

  mpSkeleton = mOpenFunc();
  if(!mpSkeleton)
    mDestroyFunc(mpSkeleton);

  mState = OPENED;

  return mpSkeleton;
}

template <class BPFSkeletonType>
void ProgramLifeCycle<BPFSkeletonType>::load()
{
  LOG_FUNC();
  // Load BPF programs identified in skeleton.
  // We do not need to pass the path of the .o (object), due to encapsulation
  // made by bpftool in skeleton object.
  if(int err = mLoadFunc(mpSkeleton)) {
    mDestroyFunc(mpSkeleton);
    std::stringstream errMsg;
    errMsg << "Cannot load program - error" << err;
    throw std::runtime_error(errMsg.str());
  }
  mState = LOADED;
}

template <class BPFSkeletonType>
void ProgramLifeCycle<BPFSkeletonType>::attach()
{
  LOG_FUNC();
  // Attach is not support by XDP programs.
  // This call does not do anything.
  if(int err = mAttachFunc(mpSkeleton)) {
    mDestroyFunc(mpSkeleton);
    std::stringstream errMsg;
    errMsg << "Cannot attach program - error" << err;
    throw std::runtime_error(errMsg.str());
  }
  mState = ATTACHED;
}

template <class BPFSkeletonType>
void ProgramLifeCycle<BPFSkeletonType>::link(std::string sectionName, std::string interface)
{
  LOG_FUNC();

  struct bpf_program *prog;
  auto ifIndex = if_nametoindex(interface.c_str());
  int fd;
  std::string section;

  // TODO navarrothiago - remove hardcoded.
  if(!ifIndex) {
    perror("if_nametoindex");
    throw std::runtime_error("Interface not found!");
  }

  bpf_object__for_each_program(prog, mpSkeleton->obj)
  {
    section = std::string(bpf_program__section_name(prog));
    if(section == sectionName) {
      // Get programs FD from skeleton object.
      fd = bpf_program__fd(prog);
      mSectionIfaceMap[sectionName] = ifIndex;

      if(bpf_set_link_xdp_fd(ifIndex, fd, XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE) < 0) {
        LOG_ERROR("BPF program link set XDP failed");
        tearDown();
        throw std::runtime_error("BPF program link set XDP failed");
      }

      mSectionHookStateMap[sectionName] = HOOKED;
      mState = LINKED;

      LOG_INF("BPF program hooked in XDP");
      return;
    };
  }
  LOG_ERROR("Section {} not found", sectionName);
  throw std::runtime_error("Section not found");
}

template <class BPFSkeletonType>
void ProgramLifeCycle<BPFSkeletonType>::tearDown()
{
  LOG_FUNC();
  std::lock_guard<std::mutex> lock(sTearDownMutex);
  struct bpf_program *prog;
  std::string section;

  if(mState != ProgramState::IDLE) {
    bpf_object__for_each_program(prog, mpSkeleton->obj)
    {
      section = std::string(bpf_program__section_name(prog));
      if(mSectionHookStateMap[section] != HOOKED) {
        LOG_DBG("{} is not in a HOOKED state", section);
        continue;
      }

      LOG_DBG("BPF program {} is in a HOOKED state", section.c_str());
      if(bpf_set_link_xdp_fd(mSectionIfaceMap[section], -1, XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE)){
        LOG_ERROR("BPF program {} cannot change to OFF state", section);
        throw std::runtime_error("BPF program cannot change to OFF state");
      };
      LOG_INF("BPF program {} change to hook state to OFF ", section);
      mSectionHookStateMap[section] = OFF;
    }
    destroy();
  } else {
    LOG_DBG("Program is in IDLE state. TearDown skipped");
  }
}

template <class BPFSkeletonType>
void ProgramLifeCycle<BPFSkeletonType>::destroy()
{
  LOG_FUNC();
  // Destroy program.
  if(mState != IDLE) {
    mDestroyFunc(mpSkeleton);
  }
  // TODO navarrothiago - check if it is necessary delete sessionManager here.
  mState = IDLE;
}

template <class BPFSkeletonType>
BPFSkeletonType *ProgramLifeCycle<BPFSkeletonType>::getBPFSkeleton() const
{
  LOG_FUNC();
  return mpSkeleton;
}

template <class BPFSkeletonType>
ProgramState ProgramLifeCycle<BPFSkeletonType>::getState() const
{
  LOG_FUNC();
  return mState;
}

#endif // __PROGRAMLIFECYCLE_H__