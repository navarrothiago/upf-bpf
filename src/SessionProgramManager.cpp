#include "SessionProgramManager.h"
#include <FARProgram.h>
#include <SessionProgram.h>
#include <UPFProgram.h>
#include <UserPlaneComponent.h>
#include <net/if.h> // if_nametoindex
#include <next_prog_rule_key.h>
#include <observer/OnStateChangeSessionProgramObserver.h>
#include <types.h>
#include <utils/LogDefines.h>
#include <wrappers/BPFMap.hpp>

SessionProgramManager::~SessionProgramManager()
{
  LOG_FUNC();
  removeAll();
}

SessionProgramManager &SessionProgramManager::getInstance()
{
  LOG_FUNC();
  static SessionProgramManager sInstance;
  return sInstance;
}

void SessionProgramManager::setTeidSessionMap(std::shared_ptr<BPFMap> pProgramsMaps)
{
  LOG_FUNC();
  mpTeidSessionMap = pProgramsMaps;
}

void SessionProgramManager::create(uint32_t teid, uint8_t sourceInterface, uint32_t ueIpAddress, std::shared_ptr<pfcp::pfcp_far> pFar)
{
  LOG_FUNC();
  struct next_rule_prog_index_key key = {.teid = teid, .source_value = sourceInterface, .ipv4_address = ueIpAddress};
  LOG_DBG("Load FAR related program");
  std::shared_ptr<FARProgram> pFARProgram = std::make_shared<FARProgram>();
  // pFar
  LOG_DBG("Store PDI in the PDR program");
  auto pUPFProgram = UserPlaneComponent::getInstance().getUPFProgram();
  // pUPFProgram->getNextProgRuleMap()->update(key, )
  LOG_DBG("Store FAR in the FAR program");
  LOG_DBG("Configure redirect interfaces");
}

void SessionProgramManager::create(uint32_t seid)
{
  LOG_FUNC();

  // Check if there is a key with seid value.
  // TODO navarrothiago - check if can be abstract the programMap.

  if(mSessionProgramMap.find(seid) != mSessionProgramMap.end()) {
    LOG_ERROR("Session {} already exists. Cannot create a new program with this key", seid);
    throw std::runtime_error("Cannot create a new program with key (seid)");
  }

  // Instantiate a new SessionProgram
  auto udpInterface = UserPlaneComponent::getInstance().getUDPInterface();
  auto gtpInterface = UserPlaneComponent::getInstance().getGTPInterface();
  std::shared_ptr<SessionProgram> pSessionProgram = std::make_shared<SessionProgram>(gtpInterface, udpInterface);
  pSessionProgram->setup();

  // Initialize key egress interface map.
  uint32_t udpInterfaceIndex = if_nametoindex(udpInterface.c_str());
  uint32_t gtpInterfaceIndex = if_nametoindex(gtpInterface.c_str());
  uint32_t uplinkId = static_cast<uint32_t>(FlowDirection::UPLINK);
  uint32_t downlinkId = static_cast<uint32_t>(FlowDirection::DOWNLINK);
  pSessionProgram->getEgressInterfaceMap()->update(uplinkId, udpInterfaceIndex, BPF_ANY);
  pSessionProgram->getEgressInterfaceMap()->update(downlinkId, gtpInterfaceIndex, BPF_ANY);

  // Update the SessionProgram map.
  mSessionProgramMap.insert(std::pair<uint32_t, std::shared_ptr<SessionProgram>>(seid, pSessionProgram));
}

void SessionProgramManager::remove(uint32_t seid)
{
  LOG_FUNC();
  auto sessionProgram = findSessionProgram(seid);
  if(!sessionProgram) {
    LOG_ERROR("The session {} does not exist. Cannot be removed", seid);
    throw std::runtime_error("The session does not exist. Cannot be removed");
  }
  sessionProgram->tearDown();
  mSessionProgramMap.erase(seid);
  // Notify observer that a SessionProgram was removed.
  // mpOnNewSessionProgramObserver->onDestroySessionProgram(seid);
}

void SessionProgramManager::removeAll()
{
  LOG_FUNC();

  for(auto pair : mSessionProgramMap) {
    pair.second->tearDown();

    // Notify observer that a SessionProgram was removed.
    mpOnNewSessionProgramObserver->onDestroySessionProgram(pair.first);
  }
  mSessionProgramMap.clear();
}

void SessionProgramManager::setOnNewSessionObserver(OnStateChangeSessionProgramObserver *pObserver)
{
  LOG_FUNC();
  mpOnNewSessionProgramObserver = pObserver;
}

std::shared_ptr<SessionProgram> SessionProgramManager::findSessionProgram(uint32_t seid)
{
  LOG_FUNC();
  std::shared_ptr<SessionProgram> pSessionProgram;

  auto it = mSessionProgramMap.find(seid);
  if(it != mSessionProgramMap.end()) {
    pSessionProgram = it->second;
  }

  return pSessionProgram;
}

SessionProgramManager::SessionProgramManager() { LOG_FUNC(); }
