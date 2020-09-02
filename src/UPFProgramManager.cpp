#include "UPFProgramManager.h"
#include <SessionManager.h>
#include <bpf/bpf.h>       // bpf calls
#include <bpf/libbpf.h>    // bpf wrappers
#include <iostream>        // cout
#include <linux/if_link.h> // XDP flags
#include <net/if.h>        // if_nametoindex
#include <sstream>         //stringstream
#include <stdexcept>       // exception
#include <sys/resource.h>  // rlimit
#include <wrappers/BPFMaps.h>

UPFProgramManager::XDPProgramInfo sXDPProgramInfo[2];
std::mutex UPFProgramManager::sTearDownMutex;
upf_xdp_bpf_c *UPFProgramManager::spSkeleton;
std::atomic<UPFProgramManager::ProgramState> UPFProgramManager::sState;

UPFProgramManager::UPFProgramManager()
{
  LOG_FUNC();
  // TODO navarrothiago - refactors.
  sXDPProgramInfo[0].xdpFlag = XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE;
  sXDPProgramInfo[1].xdpFlag = XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE;
  sXDPProgramInfo[0].state = IDLE;
  sXDPProgramInfo[1].state = IDLE;
  sState = IDLE;
}
UPFProgramManager::~UPFProgramManager()
{
  LOG_FUNC();
  // Destroy program.
  destroy();
}

void UPFProgramManager::setup(std::shared_ptr<RulesUtilities> pRulesUtilities)
{
  LOG_FUNC();

  int err;
  unsigned int key_ifmap = 0;

  // Save rules factory object.
  mpRulesUtilities = pRulesUtilities;

  open();
  load();
  attach();

  sXDPProgramInfo[0].ifIndex = if_nametoindex("wlp0s20f3");
  sXDPProgramInfo[1].ifIndex = if_nametoindex("veth0");

  // TODO navarrothiago - remove hardcoded.
  if(!sXDPProgramInfo[0].ifIndex || !sXDPProgramInfo[1].ifIndex) {
    perror("if_nametoindex");
    throw std::runtime_error("Interface not found!");
  }

  // Get programs FD from skeleton object.
  sXDPProgramInfo[0].programFd = bpf_program__fd(spSkeleton->progs.upf_chain);
  sXDPProgramInfo[1].programFd = bpf_program__fd(spSkeleton->progs.xdp_redirect_gtpu);

  signal(SIGINT, UPFProgramManager::tearDown);
  signal(SIGTERM, UPFProgramManager::tearDown);
  signal(SIGSEGV, UPFProgramManager::tearDown);

  for(uint i = 0; i < spSkeleton->skeleton->prog_cnt; i++) {

    // Clean memory. Without this, the error "can't get prog info OUT - Bad address" on bpf_obj_get_info_by_fd is shown.
    memset(&sXDPProgramInfo[i].info, 0, sizeof(sXDPProgramInfo[i].info));

    if(bpf_set_link_xdp_fd(sXDPProgramInfo[i].ifIndex, sXDPProgramInfo[i].programFd, sXDPProgramInfo[i].xdpFlag) < 0) {
      LOG_ERROR("BPF program link set XDP index {} failed", i);
      tearDown(SIGTERM);
      throw std::runtime_error("BPF program link set XDP IN failed");
    }

    sXDPProgramInfo[i].state = LINKED;
    LOG_INFO("BPF program index {} linked in XDP", i);

    // Get info from XDP program.
    err = bpf_obj_get_info_by_fd(sXDPProgramInfo[i].programFd, &sXDPProgramInfo[i].info, &sXDPProgramInfo[i].info_len);
    if(err) {
      LOG_ERROR("Can't get prog info index {} - {}", i, strerror(errno));
      tearDown(SIGTERM);
      throw std::runtime_error("Can't get prog info IN");
    }
    sXDPProgramInfo[i].programId = sXDPProgramInfo[i].info.id;
  }

  mpMaps->getMap("m_id_txcnt").update(key_ifmap, sXDPProgramInfo[1].ifIndex, 0);
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

// TODO navarrothiago - check when kill when running.
// It was noted the infinity loop.
void UPFProgramManager::tearDown(int signal)
{
  LOG_FUNC();
  std::lock_guard<std::mutex> lock(sTearDownMutex);

  if(sState != IDLE) {
    for(uint8_t i = 0; i < spSkeleton->skeleton->prog_cnt; i++) {
      if(sXDPProgramInfo[i].state != LINKED) {
        LOG_DEBUG("Not in a LINKED state");
        continue;
      }

      LOG_DEBUG("Program {} is in a LINKED state", i);

      if(bpf_get_link_xdp_id(sXDPProgramInfo[i].ifIndex, &sXDPProgramInfo[i].programId, sXDPProgramInfo[i].xdpFlag)) {
        LOG_INFO("bpf_get_link_xdp_id failed");
        continue;
      }
      LOG_DEBUG("Get XDP In link successful");
      bpf_set_link_xdp_fd(sXDPProgramInfo[i].ifIndex, -1, sXDPProgramInfo[i].xdpFlag);
      LOG_INFO("BPF program unlinked from XDP (in)");
    }
    destroy();
  } else {
    LOG_DEBUG("Program is in IDLE state. TearDown skipped");
  }
}

std::shared_ptr<SessionManager> UPFProgramManager::getSessionManager()
{
  LOG_FUNC();
  return mpSessionManager;
}

std::shared_ptr<RulesUtilities> UPFProgramManager::getRulesUtilities()
{
  LOG_FUNC();
  return mpRulesUtilities;
}

void UPFProgramManager::open()
{
  LOG_FUNC();
  struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
  if(setrlimit(RLIMIT_MEMLOCK, &r)) {
    perror("setrlimit(RLIMIT_MEMLOCK)");
    throw std::runtime_error("Cannot change bpf limit program");
  }

  spSkeleton = upf_xdp_bpf_c__open();
  if(!spSkeleton)
    destroy();

  mpMaps = std::make_unique<BPFMaps>(spSkeleton->skeleton);
  sState = OPENED;
}

void UPFProgramManager::load()
{
  LOG_FUNC();
  // Load BPF programs identified in skeleton.
  // We do not need to pass the path of the .o (object), due to encapsulation
  // made by bpftool in skeleton object.
  if(int err = upf_xdp_bpf_c__load(spSkeleton)) {
    destroy();
    std::stringstream errMsg;
    errMsg << "Cannot load program - error" << err;
    throw std::runtime_error(errMsg.str());
  }

  // Warning - The name of the map must be the same of the BPF program.
  std::shared_ptr<BPFMap> pSessionMap = std::make_shared<BPFMap>(UPFProgramManager::getInstance().getMaps()->getMap("m_seid_session"));
  mpSessionManager = std::make_shared<SessionManager>(pSessionMap);

  sState = LOADED;
}

void UPFProgramManager::attach()
{
  LOG_FUNC();
  // Attach is not support by XDP programs.
  // This call does not do anything.
  if(int err = upf_xdp_bpf_c__attach(spSkeleton)) {
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
  if(sState != IDLE) {
    upf_xdp_bpf_c__destroy(spSkeleton);
  }
  // TODO navarrothiago - check if it is necessary delete sessionManager here.
  sState = IDLE;
}
