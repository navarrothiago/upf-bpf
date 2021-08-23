#include "SessionProgramManager.h"
#include <FARProgram.h>
#include <SessionProgram.h>
#include <SessionPrograms.h>
#include <UPFProgram.h>
#include <UserPlaneComponent.h>
#include <net/if.h> // if_nametoindex
#include <next_prog_rule_key.h>
#include <observer/OnStateChangeSessionProgramObserver.h>
#include <pfcp/pfcp_far.h>
#include <spdlog/fmt/ostr.h>
#include <types.h>
#include <utils/LogDefines.h>
#include <wrappers/BPFMap.hpp>

#define EMPTY_SLOT -1l

//  TODO navarrothiago - encapsulate in order file.
// Custom format for next_rule_prog_index_key.

std::ostream &operator<<(std::ostream &Str, next_rule_prog_index_key const &v)
{
  Str << "teid: " << v.teid << " source_interface: " << v.source_value << "ip: ", v.ipv4_address;
  return Str;
}

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

void SessionProgramManager::createPipeline(uint32_t seid, uint32_t teid, uint8_t sourceInterface, uint32_t ueIpAddress,
                                   std::shared_ptr<pfcp::pfcp_far> pFar)
{
  LOG_FUNC();
  struct next_rule_prog_index_key key = {.teid = teid, .source_value = sourceInterface, .ipv4_address = ueIpAddress};

  LOG_DBG("Instantiate a new FARProgram");
  // Instantiate a new FARProgram
  std::shared_ptr<FARProgram> pFARProgram = std::make_shared<FARProgram>();
  pFARProgram->setup();

  LOG_DBG("Store FARProgram index in the UPFProgram");
  auto pUPFProgram = UserPlaneComponent::getInstance().getUPFProgram();
  auto id = pFARProgram->getId();
  auto fd = pFARProgram->getFd();
  pUPFProgram->getNextProgRuleIndexMap()->update(key, id, BPF_ANY);

  // TODO navarrothiago - get the nextProgRule index from a pool of values.
  pUPFProgram->getNextProgRuleMap()->update(id, fd, BPF_ANY);

  LOG_DBG("Store FAR in the FAR program");
  uint8_t index = 0;
  // TODO navarrothiago - create a method to encapuslate.
  pfcp_far_t_ far = {// FAR ID.
                     .far_id.far_id = pFar->far_id.far_id,
                     //  Fwd - Destination interface value
                     .forwarding_parameters.destination_interface.interface_value =
                         pFar->forwarding_parameters.second.destination_interface.second.interface_value,
                     //  Fwd - teid
                     .forwarding_parameters.outer_header_creation.teid =
                         pFar->forwarding_parameters.second.outer_header_creation.second.teid,
                     //  Fwd - port
                     .forwarding_parameters.outer_header_creation.port_number =
                         pFar->forwarding_parameters.second.outer_header_creation.second.port_number,
                     // Fwd - ipv4
                     .forwarding_parameters.outer_header_creation.ipv4_address.s_addr =
                         pFar->forwarding_parameters.second.outer_header_creation.second.ipv4_address.s_addr};
  pFARProgram->getFARMap()->update(index, far, BPF_ANY);

  // Map the pipeline deployed to the seid. The seid will be used to detroyed it.
  mSessionProgramsMap[seid] = std::make_shared<SessionPrograms>(key, pFARProgram);
}

void SessionProgramManager::removePipeline(uint32_t seid)
{
  LOG_FUNC();

  LOG_DBG("Remove FARProgram index from UPFProgram map");
  auto it = mSessionProgramsMap.find(seid);
  if(it == mSessionProgramsMap.end()){
    LOG_WARN("Trying to remove inexistente session: id {} !!", seid);
    return;
  }

  LOG_DBG("Delete the SessionPrograms object. It will release the pipeline");
  // The key represent the pointer to the pipeline related to the session.
  auto key = it->second->getKey();
  // it->second->
  it->second.reset();
  mSessionProgramsMap.erase(seid);

  LOG_DBG("Clean session from the entry program's map");
  auto pUPFProgram = UserPlaneComponent::getInstance().getUPFProgram();
  pUPFProgram->getNextProgRuleIndexMap()->remove(key);
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

SessionProgramManager::SessionProgramManager()
{
  LOG_FUNC();
  for(auto &item : mProgramArray) {
    item = EMPTY_SLOT;
  }
}

int32_t SessionProgramManager::getEmptySlot()
{
  LOG_FUNC();
  auto it = std::find(mProgramArray.begin(), mProgramArray.end(), EMPTY_SLOT);
  if(it != mProgramArray.end()) {
    auto index = it - mProgramArray.begin();
    LOG_DBG("element with index {} is empty", index);
    return index;
  } else {
    LOG_ERROR("No space available");
    throw std::runtime_error("No space available");
  }
}
