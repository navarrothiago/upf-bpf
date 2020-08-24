#include "UPFProgramManager.h"
#include <bpf/bpf.h>             // bpf calls
#include <bpf/libbpf.h>          // bpf wrappers
#include <iostream>              // cout
#include <linux/if_link.h>       // XDP flags
#include <net/if.h>              // if_nametoindex
#include <signal.h>              // signals
#include <sstream>               //stringstream
#include <stdexcept>             // exception
#include <sys/resource.h>        // rlimit
#include <wrappers/BPFMaps.h>

int UPFProgramManager::mIfindexIn;
int UPFProgramManager::mIfindexOut;
unsigned int UPFProgramManager::mProgramId;
unsigned int UPFProgramManager::mProgramRedirectId;
unsigned int UPFProgramManager::mXdpFlags = XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE;
bool UPFProgramManager::mIfindexOutXdpRedirectAttached = true;
int UPFProgramManager::mProgramFd;
int UPFProgramManager::mProgramRedirectFd;
std::mutex UPFProgramManager::sTearDownMutex;
std::atomic<UPFProgramManager::ProgramState> UPFProgramManager::sState;

UPFProgramManager::UPFProgramManager()
{
  LOG_FUNC();
  sState = IDLE;
}
UPFProgramManager::~UPFProgramManager()
{
  LOG_FUNC();
  // Destroy program.
  destroy();
}

void UPFProgramManager::setup()
{
  LOG_FUNC();

  int err;
  unsigned int key_ifmap = 0;

  open();
  load();
  attach();

  // TODO navarrothiago - remove hardcoded.
  mIfindexIn = if_nametoindex("wlp0s20f3");
  mIfindexOut = if_nametoindex("veth0");
  if(!mIfindexIn || !mIfindexOut) {
    perror("if_nametoindex");
    throw std::runtime_error("Interface not found!");
  }

  // Get programs FD from skeleton object.
  mProgramFd = bpf_program__fd(mpSkeleton->progs.upf_chain);
  mProgramRedirectFd = bpf_program__fd(mpSkeleton->progs.xdp_redirect_gtpu);

  signal(SIGINT, UPFProgramManager::tearDown);
  signal(SIGTERM, UPFProgramManager::tearDown);
  signal(SIGSEGV, UPFProgramManager::tearDown);

  if(bpf_set_link_xdp_fd(mIfindexIn, mProgramFd, mXdpFlags) < 0) {
    tearDown(SIGTERM);
    LOG_ERROR("Link set xdp fd IN failed");
    throw std::runtime_error("Link set xdp fd IN failed");
  }

  sState = LINKED;

  // Get info from XDP program.
  err = bpf_obj_get_info_by_fd(mProgramFd, &info, &info_len);
  if(err) {
    tearDown(SIGTERM);
    LOG_ERROR("Can't get prog info IN - {0}", strerror(errno));
    throw std::runtime_error("Can't get prog info IN");
  }
  mProgramId = info.id;

  if(bpf_set_link_xdp_fd(mIfindexOut, mProgramRedirectFd, mXdpFlags | XDP_FLAGS_UPDATE_IF_NOEXIST) < 0) {
    tearDown(SIGTERM);
    LOG_ERROR("Link set xdp fd OUT failed");
    throw std::runtime_error("Link set xdp fd OUT failed");
  }

  // Clean memory. Without this, the error "can't get prog info OUT - Bad address" on bpf_obj_get_info_by_fd is shown.
  memset(&info, 0, sizeof(info));

  // Get info from Reditect XDP program.
  err = bpf_obj_get_info_by_fd(mProgramRedirectFd, &info, &info_len);
  if(err) {
    tearDown(SIGTERM);
    LOG_ERROR("Can't get prog info OUT - {0}", strerror(errno));
    throw std::runtime_error("Can't get prog info OUT");
  }

  mProgramRedirectId = info.id;
  mpMaps->getMap("m_id_txcnt").update(key_ifmap, mIfindexOut, 0);
}

UPFProgramManager &UPFProgramManager::getInstance()
{
  LOG_FUNC();
  static UPFProgramManager sInstance;
  return sInstance;
}

std::shared_ptr<BPFMaps> UPFProgramManager::getMaps()
{
  LOG_FUNC();
  return mpMaps;
}

void UPFProgramManager::tearDown(int signal)
{
  LOG_FUNC();
  std::lock_guard<std::mutex> lock(sTearDownMutex);

  if(sState != LINKED) {
    LOG_DEBUG("Not in a LINKED state");
    return;
  }
  LOG_DEBUG("Program is in a LINKED state");

  __u32 curr_prog_id = 0;
  if(bpf_get_link_xdp_id(mIfindexIn, &curr_prog_id, mXdpFlags)) {
    printf("bpf_get_link_xdp_id failed\n");
    exit(1);
  } else {
    LOG_DEBUG("Get XDP In link successful");
    if(mProgramId == curr_prog_id)
      bpf_set_link_xdp_fd(mIfindexIn, -1, mXdpFlags);
    else if(!curr_prog_id)
      printf("couldn't find a prog id on iface IN\n");
    else
      printf("program on iface IN changed, not removing\n");
  }
  if(mIfindexOutXdpRedirectAttached) {
    curr_prog_id = 0;
    if(bpf_get_link_xdp_id(mIfindexOut, &curr_prog_id, mXdpFlags)) {
      printf("bpf_get_link_xdp_id failed\n");
      exit(1);
    } else {
      LOG_DEBUG("Get XDP Out link successful");
      if(mProgramRedirectId == curr_prog_id)
        bpf_set_link_xdp_fd(mIfindexOut, -1, mXdpFlags);
      else if(!curr_prog_id)
        printf("couldn't find a prog id on iface OUT\n");
      else
        printf("program on iface OUT changed, not removing\n");
    }
  }
  exit(0);
}

void UPFProgramManager::open()
{
  LOG_FUNC();
  struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
  if(setrlimit(RLIMIT_MEMLOCK, &r)) {
    perror("setrlimit(RLIMIT_MEMLOCK)");
    throw std::runtime_error("Cannot change bpf limit program");
  }

  mpSkeleton = upf_xdp_bpf_c__open();
  if(!mpSkeleton)
    destroy();

  mpMaps = std::make_unique<BPFMaps>(mpSkeleton->skeleton);
  sState = OPENED;
}

void UPFProgramManager::load()
{
  LOG_FUNC();
  // Load BPF programs idetified in skeleton.
  if(int err = upf_xdp_bpf_c__load(mpSkeleton)) {
    destroy();
    std::stringstream errMsg;
    errMsg << "Cannot load program - error" << err;
    throw std::runtime_error(errMsg.str());
  }
  sState = LOADED;
}

void UPFProgramManager::attach()
{
  LOG_FUNC();
  // Attach is not support by XDP programs.
  // This call doesnt do anything.
  if(int err = upf_xdp_bpf_c__attach(mpSkeleton)) {
    destroy();
    std::stringstream errMsg;
    errMsg << "Cannot attach program - error" << err;
    throw std::runtime_error(errMsg.str());
  }
  sState = ATTACHED;
}

void UPFProgramManager::destroy()
{
  LOG_FUNC();
  // Destroy program.
  upf_xdp_bpf_c__destroy(mpSkeleton);
  sState = DESTROYED;
}
