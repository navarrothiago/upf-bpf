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
#include <SessionPrograms.h>
#include <utils/LogDefines.h>
// #include <utils/Util.h>
#include <wrappers/BPFMap.hpp>
#include <pfcp_session.hpp>

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

  // TODO navarrothiago - Create a logic to parse the json and only after that create the session, pdr and far.
  seid_t_ seid = jRequest["seid"];
  LOG_INF("Case: create session");
  // auto pSession = createSession(seid);
  std::shared_ptr<pfcp::pfcp_session> pSession = std::make_shared<pfcp::pfcp_session>();
  pSession->seid = seid;

  pfcp::offending_ie_t offending_ie = {};
  pfcp::fteid_t allocated_fteid;
  pfcp::cause_t cause = {pfcp::CAUSE_VALUE_REQUEST_ACCEPTED};
  // spSessionManager->createSession(pSession);

  for(const auto &element : jRequest["pdrs"]) {

    auto pPdr = createOaiPDR(element["pdrId"], element["farId"], element["pdi"]["teid"],
                          sMapInterface[element["pdi"]["sourceInterface"]],
                          Util::convertIpToInet(std::string(element["pdi"]["ueIPAddress"])),
                          sMapOuterHeader[element["outerHeaderRemoval"]], 0);
    LOG_INF("Case: add PDR");
    pPdr->precedence.first = true;
    if(sMapInterface[element["pdi"]["sourceInterface"]] == INTERFACE_VALUE_CORE){
      LOG_WARN("FIXME - PRECEDENCE IS HARDCODED");
      LOG_WARN("FIXME - ASSIGN 0 TO DL PDR");
      pPdr->precedence.second.precedence = 0;
    }else{
      LOG_WARN("FIXME - PRECEDENCE IS HARDCODED");
      LOG_WARN("FIXME - ASSIGN 1 TO UL PDR");
      pPdr->precedence.second.precedence = 1;
    }
    pSession->create(*pPdr, cause, offending_ie.offending_ie, allocated_fteid);
    // spSessionManager->addPDR(pSession->getSeid(), pPdr);
  }

  for(const auto &element : jRequest["fars"]) {
    auto pFar = createOaiFAR(
        element["farId"], actions, sMapInterface[element["forwardingParameters"]["destinationInterface"]],
        sMapOuterHeader[element["forwardingParameters"]["outerHeaderCreation"]["outerHeaderCreationDescription"]],
        Util::convertIpToInet(std::string(element["forwardingParameters"]["outerHeaderCreation"]["ipv4Address"])),
        element["forwardingParameters"]["outerHeaderCreation"]["portNumber"]);
    LOG_INF("Case: add FAR");
    LOG_WARN("FIXME - FORWARD ACTION IS ENABLE - HARDCODED");
    pFar->apply_action.first = true;
    pFar->apply_action.second.forw = true;
    pSession->create(*pFar, cause, offending_ie.offending_ie);
    // spSessionManager->addFAR(pSession->getSeid(), pFar);
  }
  spSessionManager->createBPFSession(pSession);
  LOG_INF("Case: update ARP Table");
  auto pSessionPrograms = SessionProgramManager::getInstance().findSessionPrograms(seid);

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
    // pSessionProgram->getArpTableMap()->update(ip, pMacAddress->ether_addr_octet, BPF_ANY);
    pSessionPrograms->getFARProgram()->getArpTableMap()->update(ip, pMacAddress->ether_addr_octet, BPF_ANY);
  }
  return 200;
}
