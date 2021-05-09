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
static std::map<std::string, u8> sMapInterface = {{"INTERFACE_VALUE_CORE", INTERFACE_VALUE_CORE},
                                                  {"INTERFACE_VALUE_ACCESS", INTERFACE_VALUE_ACCESS}};
static std::map<std::string, u32> sMapOuterHeader = {
    {"OUTER_HEADER_CREATION_GTPU_UDP_IPV4", OUTER_HEADER_CREATION_GTPU_UDP_IPV4},
    {"OUTER_HEADER_CREATION_UDP_IPV4", OUTER_HEADER_CREATION_UDP_IPV4},
    {"OUTER_HEADER_REMOVAL_UDP_IPV4", OUTER_HEADER_REMOVAL_UDP_IPV4},
    {"OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4", OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4}};

Controller::Controller(/* args */) { LOG_FUNC(); }

Controller::~Controller() { LOG_FUNC(); }

int Controller::setup(json &jRequest, json &jReponse)
{
  LOG_FUNC();
  std::shared_ptr<RulesUtilities> mpRulesFactory;
  mpRulesFactory = std::make_shared<RulesUtilitiesImpl>();

  Configuration::sGTPInterface = jRequest["gtpInterface"];
  Configuration::sUDPInterface = jRequest["udpInterface"];
  LOG_DBG("GTP interface: {}", Configuration::sGTPInterface);
  LOG_DBG("UDP interface: {}", Configuration::sUDPInterface);

  UserPlaneComponent::getInstance().setup(mpRulesFactory, Configuration::sGTPInterface, Configuration::sUDPInterface);
  spSessionManager = UserPlaneComponent::getInstance().getSessionManager();
  return 200;
}

int Controller::createSesssion(json &jRequest, json &jResponse)
{
  LOG_FUNC();

  apply_action_t_ actions;
  actions.forw = true;
  // u16 dstPort = 1234;
  // dstPort = 1234;
  // struct in_addr src_addr;
  // struct in_addr ue_ip;
  // struct in_addr dst_addr;

  // TODO navarrothiago - Create a logic to parse the json and only after that create the session, pdr and far.
  seid_t_ seid = jRequest["seid"];
  LOG_INF("Case: create session");
  auto pSession = createSession(seid);
  spSessionManager->createSession(pSession);

  for(const auto &element : jRequest["pdrs"]) {
    auto pPdr = createPDR(element["pdrId"], element["farId"], element["pdi"]["teid"],
                          sMapInterface[element["pdi"]["sourceInterface"]],
                          Util::convertIpToInet(std::string(element["pdi"]["ueIPAddress"])),
                          sMapOuterHeader[element["outerHeaderRemoval"]]);
    LOG_INF("Case: add PDR");
    spSessionManager->addPDR(pSession->getSeid(), pPdr);
  }

  for(const auto &element : jRequest["fars"]) {
    auto pFar = createFAR(
        element["farId"], actions, sMapInterface[element["forwardingParameters"]["destinationInterface"]],
        sMapOuterHeader[element["forwardingParameters"]["outerHeaderCreation"]["outerHeaderCreationDescription"]],
        Util::convertIpToInet(std::string(element["forwardingParameters"]["outerHeaderCreation"]["ipv4Address"])),
        element["forwardingParameters"]["outerHeaderCreation"]["portNumber"]);
    LOG_INF("Case: add FAR");
    spSessionManager->addFAR(pSession->getSeid(), pFar);
  }
  LOG_INF("Case: update ARP Table");
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);

  std::map<std::string, std::string> arpTable;
  for(const auto &element : jRequest["arpTable"]) {
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
