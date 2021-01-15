#ifndef __PROGRAMLIFECYCLE_H__
#define __PROGRAMLIFECYCLE_H__

#include <atomic>
#include <functional>     // rlimit
#include <sstream>        //stringstream
#include <sys/resource.h> // rlimit
#include <utils/LogDefines.h>


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
  BPFSkeletonType * open();
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
   * @brief Destroy the BPF program.
   *
   */
  void destroy();
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
  // The program state.
  std::atomic<ProgramState> mState;
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
BPFSkeletonType* ProgramLifeCycle<BPFSkeletonType>::open()
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