#include "Controller.h"
#include <Configuration.h>
#include <InformationElementFactory.hpp>
#include <RulesUtilitiesImpl.h>
#include <SessionManager.h>
#include <SessionProgramManager.h>
#include <UserPlaneComponent.h>
#include <linux/bpf.h>
#include <netinet/ether.h>
#include <programs/SessionProgram.h>
#include <utils/LogDefines.h>
#include <utils/Util.h>
#include <wrappers/BPFMap.hpp>

static std::shared_ptr<SessionManager> spSessionManager;
static std::map<std::string, FlowDirection> sMapFlowDirection = {{"downlink", DOWNLINK}, {"uplink", UPLINK}};

Controller::Controller(/* args */) { LOG_FUNC(); }

Controller::~Controller() { LOG_FUNC(); }

int Controller::setup(json jBody)
{
  LOG_FUNC();
  std::shared_ptr<RulesUtilities> mpRulesFactory;
  mpRulesFactory = std::make_shared<RulesUtilitiesImpl>();

  Configuration::sGTPInterface = jBody["gtpInterface"];
  Configuration::sUDPInterface = jBody["udpInterface"];
  LOG_DBG("GTP interface: {}", Configuration::sGTPInterface);
  LOG_DBG("UDP interface: {}", Configuration::sUDPInterface);

  UserPlaneComponent::getInstance().setup(mpRulesFactory, Configuration::sGTPInterface, Configuration::sUDPInterface);
  spSessionManager = UserPlaneComponent::getInstance().getSessionManager();
  return 200;
}

int Controller::createSesssion(json jBody)
{
  LOG_FUNC();

  apply_action_t_ actions;
  actions.forw = true;
  u16 dstPort = 1234;
  dstPort = 1234;
  struct in_addr src_addr;
  struct in_addr ue_ip;
  struct in_addr dst_addr;

  // TODO navarrothiago - Create a logic to parse the json and only after that create the session, pdr and far.
  seid_t_ seid = jBody["seid"];
  LOG_INF("Case: create session");
  auto pSession = createSession(seid);
  spSessionManager->createSession(pSession);

  for(const auto &element : jBody["pdrs"]) {
    u32 sourceInterface, destinationInterface;
    u16 outerHeaderRemoval, outerHeaderCreation;

    switch(sMapFlowDirection[element["type"]]) {
    case UPLINK:
      LOG_DBG("Uplink direction");
      sourceInterface = INTERFACE_VALUE_ACCESS;
      outerHeaderRemoval = OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4;
      destinationInterface = INTERFACE_VALUE_CORE;
      outerHeaderCreation = OUTER_HEADER_CREATION_UDP_IPV4;
      break;
    case DOWNLINK:
      LOG_DBG("Downlink direction");
      sourceInterface = INTERFACE_VALUE_CORE;
      outerHeaderRemoval = OUTER_HEADER_REMOVAL_UDP_IPV4;
      destinationInterface = INTERFACE_VALUE_ACCESS;
      outerHeaderCreation = OUTER_HEADER_CREATION_GTPU_UDP_IPV4;
      break;
    default:
      break;
    }
    auto pPdr = createPDR(element["pdrId"], element["farId"], element["teid"], sourceInterface, Util::convertIpToInet(std::string(element["dstIPAddress"])), outerHeaderRemoval);
    auto pFar = createFAR(element["farId"], actions, destinationInterface, outerHeaderCreation, Util::convertIpToInet(std::string(element["dstIPAddress"])), dstPort);
    LOG_INF("Case: add PDR");
    spSessionManager->addPDR(pSession->getSeid(), pPdr);
    LOG_INF("Case: add FAR");
    spSessionManager->addFAR(pSession->getSeid(), pFar);
  }

  LOG_INF("Case: update ARP Table");
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  std::map<std::string, std::string> arpTable;
  for(const auto &element : jBody["arpTable"]) {
    std::cout << element << std::endl;

    // Map ip to mac address.
    arpTable[element["ip"]] = element["mac"];

    struct in_addr ip_addr;
    if(inet_aton(std::string(element["ip"]).c_str(), &ip_addr) == 0) {

      return 400;
    }

    auto ip = static_cast<uint32_t>(ip_addr.s_addr);
    // auto pMacAddress = Util::getInstance().stringToMac(element["mac"]).data();
    auto pMacAddress = ether_aton(std::string(element["mac"]).c_str());
    pSessionProgram->getArpTableMap()->update(ip, pMacAddress->ether_addr_octet, BPF_ANY);
  }
  return 200;
}
