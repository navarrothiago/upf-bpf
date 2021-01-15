#include "UPFProgram.h"
#include <SessionManager.h>
#include <bpf/bpf.h>       // bpf calls
#include <bpf/libbpf.h>    // bpf wrappers
#include <iostream>        // cout
#include <linux/if_link.h> // XDP flags
#include <net/if.h>        // if_nametoindex
#include <stdexcept>       // exception
#include <wrappers/BPFMaps.h>

UPFProgram::XDPProgramInfo sXDPProgramInfo[2];

UPFProgram::UPFProgram()
{
  LOG_FUNC();
  // TODO navarrothiago - refactors.
  sXDPProgramInfo[0].xdpFlag = XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE;
  sXDPProgramInfo[1].xdpFlag = XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE;
  sXDPProgramInfo[0].state = IDLE;
  sXDPProgramInfo[1].state = IDLE;

  mpLifeCycle = std::make_unique<UPFProgramLifeCycle>(upf_xdp_bpf_c__open, upf_xdp_bpf_c__load, upf_xdp_bpf_c__attach, upf_xdp_bpf_c__destroy);
}

UPFProgram::~UPFProgram()
{
  LOG_FUNC();
  // Destroy program.
  // destroy();
}

void UPFProgram::setup()
{
  LOG_FUNC();

  int err;
  unsigned int key_ifmap = 0;

  // Open.
  spSkeleton = mpLifeCycle->open();
  // Initialize Maps.
  initializeMaps();
  // Load.
  mpLifeCycle->load();
  // Attach.
  mpLifeCycle->attach();

  // TODO navarrothiago - Remove hardcoded - https://github.com/navarrothiago/upf-bpf/issues/24
  // sXDPProgramInfo[0].ifIndex = if_nametoindex("eth0") ;
  // sXDPProgramInfo[0].ifIndex = if_nametoindex("wlp0s20f3");
  sXDPProgramInfo[0].ifIndex = if_nametoindex("enp0s20f0u4u2u4");
  sXDPProgramInfo[1].ifIndex = if_nametoindex("veth0");

  // TODO navarrothiago - remove hardcoded.
  if(!sXDPProgramInfo[0].ifIndex || !sXDPProgramInfo[1].ifIndex ) {
    perror("if_nametoindex");
    throw std::runtime_error("Interface not found!");
  }

  // Get programs FD from skeleton object.
  sXDPProgramInfo[0].programFd = bpf_program__fd(spSkeleton->progs.upf_chain);
  sXDPProgramInfo[1].programFd = bpf_program__fd(spSkeleton->progs.xdp_redirect_gtpu);

  for(uint i = 0; i < spSkeleton->skeleton->prog_cnt; i++) {

    // Clean memory. Without this, the error "can't get prog info OUT - Bad address" on bpf_obj_get_info_by_fd is shown.
    memset(&sXDPProgramInfo[i].info, 0, sizeof(sXDPProgramInfo[i].info));

    if(bpf_set_link_xdp_fd(sXDPProgramInfo[i].ifIndex, sXDPProgramInfo[i].programFd, sXDPProgramInfo[i].xdpFlag) < 0) {
      LOG_ERROR("BPF program link set XDP index {} failed", i);
      tearDown();
      throw std::runtime_error("BPF program link set XDP IN failed");
    }

    sXDPProgramInfo[i].state = LINKED;
    LOG_INF("BPF program index {} linked in XDP", i);

    // Get info from XDP program.
    err = bpf_obj_get_info_by_fd(sXDPProgramInfo[i].programFd, &sXDPProgramInfo[i].info, &sXDPProgramInfo[i].info_len);
    if(err) {
      LOG_ERROR("Can't get prog info index {} - {}", i, strerror(errno));
      tearDown();
      throw std::runtime_error("Can't get prog info IN");
    }
    sXDPProgramInfo[i].programId = sXDPProgramInfo[i].info.id;
  }
  
  mpMaps->getMap("m_id_txcnt").update(key_ifmap, sXDPProgramInfo[1].ifIndex, 0);
}

std::shared_ptr<BPFMaps> UPFProgram::getMaps()
{
  LOG_FUNC();
  return mpMaps;
}

// TODO navarrothiago - check when kill when running.
// It was noted the infinity loop.
void UPFProgram::tearDown()
{
  LOG_FUNC();
  std::lock_guard<std::mutex> lock(sTearDownMutex);

  if(mpLifeCycle->getState() != ProgramState::IDLE) {
    for(uint8_t i = 0; i < spSkeleton->skeleton->prog_cnt; i++) {
      if(sXDPProgramInfo[i].state != ProgramState::LINKED) {
        LOG_DBG("Not in a LINKED state");
        continue;
      }

      LOG_DBG("Program {} is in a LINKED state", i);

      if(bpf_get_link_xdp_id(sXDPProgramInfo[i].ifIndex, &sXDPProgramInfo[i].programId, sXDPProgramInfo[i].xdpFlag)) {
        LOG_INF("bpf_get_link_xdp_id failed");
        continue;
      }
      LOG_DBG("Get XDP In link successful");
      bpf_set_link_xdp_fd(sXDPProgramInfo[i].ifIndex, -1, sXDPProgramInfo[i].xdpFlag);
      LOG_INF("BPF program unlinked from XDP (in)");
    }
    mpLifeCycle->destroy();
  } else {
    LOG_DBG("Program is in IDLE state. TearDown skipped");
  }
}

void UPFProgram::updateProgMap(uint32_t key, uint32_t fd)
{
  LOG_FUNC();
  mpProgramsMap->update(key, fd, BPF_ANY);
}

std::shared_ptr<BPFMap> UPFProgram::getSessionsMap() const
{
  LOG_FUNC();
  return mpSessionsMap;
}

std::shared_ptr<BPFMap> UPFProgram::getUplinkPDRsMap() const
{
  LOG_FUNC();
  return mpUplinkPDRsMap;
}

void UPFProgram::initializeMaps()
{
  LOG_FUNC();
  // Store all maps available in the program.
  mpMaps = std::make_unique<BPFMaps>(mpLifeCycle->getBPFSkeleton()->skeleton);

  // Warning - The name of the map must be the same of the BPF program.
  auto sessionsMap = mpMaps->getMap("m_seid_session");
  auto uplinkPDRsMap = mpMaps->getMap("m_teid_pdrs");
  auto programsMap = mpMaps->getMap("m_jmp_table");
  mpSessionsMap = std::make_shared<BPFMap>(sessionsMap);
  mpUplinkPDRsMap = std::make_shared<BPFMap>(uplinkPDRsMap);
  mpProgramsMap = std::make_shared<BPFMap>(programsMap);
}