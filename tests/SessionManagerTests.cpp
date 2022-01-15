#include "InformationElementFactory.hpp"
#include "interfaces/ForwardingActionRulesImpl.h"
#include "interfaces/PacketDetectionRulesImpl.h"
#include "interfaces/RulesUtilitiesImpl.h"
#include "interfaces/SessionBpfImpl.h"
#include <3gpp_29.244.hpp>
#include <Configuration.h>
#include <SessionManager.h>
#include <SessionProgramManager.h>
#include <SessionPrograms.h>
#include <programs/UPFProgram.h>
#include <programs/FARProgram.h>
#include <UserPlaneComponent.h>
#include <next_prog_rule_key.h>
#include <gtest/gtest.h>
#include <msg_pfcp.hpp>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>
#include <pfcp_session.hpp>
#include <utils/LogDefines.h>
#include <vector>
#include <spdlog/fmt/ostr.h>

//  TODO navarrothiago - encapsulate in order file.
// Custom format for next_rule_prog_index_key.
extern std::ostream &operator<<(std::ostream &Str, next_rule_prog_index_key const &v);


class SessionManagerTests : public ::testing::Test
{
public:
  SessionManagerTests()
  {
    LOG_FUNC();
    mpRulesFactory = std::make_shared<RulesUtilitiesImpl>();
  }
  void SetUp() override
  {
    LOG_FUNC();
    UserPlaneComponent::getInstance().setup(mpRulesFactory, Configuration::sGTPInterface, Configuration::sUDPInterface);
    mpSessionManager = UserPlaneComponent::getInstance().getSessionManager();
  }
  void TearDown() override
  {
    LOG_FUNC();
    UserPlaneComponent::getInstance().tearDown();
    UserPlaneComponent::getInstance().getUPFProgram().reset();
    mpSessionManager.reset();
  }
  virtual ~SessionManagerTests() {}
  std::shared_ptr<SessionManager> mpSessionManager;
  std::shared_ptr<RulesUtilities> mpRulesFactory;
};

// TODO navarrothiago - Create UT with two sessions.
TEST_F(SessionManagerTests, manageSession)
{
  LOG_FUNC();
  std::shared_ptr<pfcp_session_t_> pSessionRaw = std::make_shared<pfcp_session_t_>();
  std::shared_ptr<SessionBpf> pSession = std::make_shared<SessionBpfImpl>(*pSessionRaw);

  // Case: create and remove session (happy path).
  EXPECT_NO_THROW(mpSessionManager->createSession(pSession));
  EXPECT_NO_THROW(mpSessionManager->removeSession(pSession->getSeid()));

  // Case: remove with an empty list.
  EXPECT_ANY_THROW(mpSessionManager->removeSession(pSession->getSeid()));
}

TEST_F(SessionManagerTests, manageBPFSession)
{
  LOG_FUNC();
  // GTEST_SKIP();
  struct in_addr src_addr;
  struct in_addr ue_ip;
  struct in_addr dst_addr;
  u32 indexProg = 0, indexValue = 0;
  s32 fdProg = 0, fdValue = 0;
  pfcp::offending_ie_t offending_ie = {};
  pfcp::fteid_t allocated_fteid;
  pfcp::cause_t cause = {pfcp::CAUSE_VALUE_REQUEST_ACCEPTED};

  inet_aton("10.10.10.10", &src_addr);
  inet_aton("10.1.3.27", &dst_addr);

  LOG_DBG("src_addr:{} dst_addr:{} ue_ip:{}", src_addr.s_addr, dst_addr.s_addr, ue_ip.s_addr);

  // Initialize context.
  seid_t_ seid = 1000;
  u16 pdrIdUL = 10, pdrIdDL = 20, dstPort = 1234;
  u32 farIdUL = 100, farIdDL = 200, teidUL = 100, teidDL = 0, precedenceUL = 1, precendeceDL = 2;
  apply_action_t_ actions = {.forw = true};
  struct next_rule_prog_index_key key = {.teid = teidUL, .source_value = INTERFACE_VALUE_ACCESS, .ipv4_address = src_addr.s_addr};
  LOG_DBG("teid: {}, source interface: {}, ue ip: {}", key.teid, key.source_value, key.ipv4_address);

  // Downlink. The UE IP is the destination.
  auto pPdrDL = createOaiPDR(pdrIdDL, farIdDL, teidDL, INTERFACE_VALUE_CORE, dst_addr, OUTER_HEADER_REMOVAL_UDP_IPV4, precendeceDL);
  auto pFarDL = createOaiFAR(farIdDL, actions, INTERFACE_VALUE_ACCESS, OUTER_HEADER_CREATION_GTPU_UDP_IPV4, dst_addr, dstPort);

  // Uplink. The UE IP is the source (highest precedence). It should deploy on dataplane.
  auto pPdrUL = createOaiPDR(pdrIdUL, farIdUL, teidUL, INTERFACE_VALUE_ACCESS, src_addr, OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4, precedenceUL);
  auto pFarUL = createOaiFAR(farIdUL, actions, INTERFACE_VALUE_CORE, OUTER_HEADER_CREATION_UDP_IPV4, dst_addr, dstPort);

  std::shared_ptr<pfcp::pfcp_session> pSession = std::make_shared<pfcp::pfcp_session>();
  pSession->seid = seid;

  // FIX: why is not creating the UL. Maybe the DL is overwriting.
  pSession->create(*pPdrDL, cause, offending_ie.offending_ie, allocated_fteid);
  pSession->create(*pFarDL, cause, offending_ie.offending_ie);
  pSession->create(*pPdrUL, cause, offending_ie.offending_ie, allocated_fteid);
  pSession->create(*pFarUL, cause, offending_ie.offending_ie);

  // Case: create and remove session (happy path).
  // - Check if the PDRs were reorded.
  // - Check if the FARProgramMap has one program.
  // - Check if the maps in UPFProgram have the elements.
  // - Check if the maps in FARProgram have the elements.

  EXPECT_NO_THROW(mpSessionManager->createBPFSession(pSession));

  // The key is related to the high precedence (which has the lower value, i.e. UL).
  EXPECT_EQ(pSession->pdrs[0]->pdr_id.rule_id, pdrIdUL);

  // TODO navarrothiago - understand why the code below is not working.
  // A weird behaviour happens when put a debug message in
  // SessionProgramManager, before the fd has beeh assigned in the maps.
  // WHY???

  indexValue = SessionProgramManager::getInstance().findSessionPrograms(seid)->getFARProgram()->getId();
  EXPECT_TRUE(UserPlaneComponent::getInstance().getUPFProgram()->getNextProgRuleIndexMap()->lookup(key, &indexProg) == 0);
  EXPECT_EQ(indexProg, indexValue);

  // TODO navarrothiago - understand why the code below is not working. It
  // retrieve on value in fd, but when the same is saved in the BPF map, it store with other value.

  // fdValue = SessionProgramManager::getInstance().findSessionPrograms(seid)->getFARProgram()->getFd();
  // EXPECT_TRUE(UserPlaneComponent::getInstance().getUPFProgram()->getNextProgRuleMap()->lookup(indexProg, &fdProg) == 0);
  // EXPECT_EQ(fdProg, fdValue);

  EXPECT_NO_THROW(mpSessionManager->removeBPFSession(pSession->get_up_seid()));

  // Case: remove with an empty list.
  EXPECT_ANY_THROW(mpSessionManager->removeBPFSession(pSession->get_up_seid()));
}
