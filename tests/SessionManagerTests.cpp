#include <SessionManager.h>
#include <UserPlaneComponent.h>
#include <Configuration.h>
#include <gtest/gtest.h>
#include <pfcp/pfcp_pdr.h>
#include <utils/LogDefines.h>
#include <pfcp/pfcp_session.h>
#include "interfaces/ForwardingActionRulesImpl.h"
#include "interfaces/PacketDetectionRulesImpl.h"
#include "interfaces/RulesUtilitiesImpl.h"
#include "interfaces/SessionBpfImpl.h"
#include <vector>
#include <pfcp_session.hpp>
#include <msg_pfcp.hpp>
#include <3gpp_29.244.hpp>
#include "InformationElementFactory.hpp"

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

  inet_aton("10.10.10.10", &src_addr);
  inet_aton("10.1.3.27", &dst_addr);

  LOG_DBG("src_addr:{} dst_addr:{} ue_ip:{}", src_addr.s_addr, dst_addr.s_addr, ue_ip.s_addr);

  // Initialize context.
  u16 pdrIdUL = 10, pdrIdDL = 20, dstPort = 1234;
  u32 farIdUL = 100, farIdDL = 200, teid = 100, precedenceUL = 1, precendeceDL = 2;
  apply_action_t_ actions = {.forw = true};

  // Downlink.
  auto pPdrDL = createOaiPDR(pdrIdDL, farIdDL, teid, INTERFACE_VALUE_CORE, dst_addr, OUTER_HEADER_REMOVAL_UDP_IPV4, precendeceDL);
  auto pFarDL = createOaiFAR(farIdDL, actions, INTERFACE_VALUE_ACCESS, OUTER_HEADER_CREATION_GTPU_UDP_IPV4, dst_addr, dstPort);

  // Uplink.
  auto pPdrUL = createOaiPDR(pdrIdUL, farIdUL, teid, INTERFACE_VALUE_ACCESS, src_addr, OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4, precedenceUL);
  auto pFarUL = createOaiFAR(farIdUL, actions, INTERFACE_VALUE_CORE, OUTER_HEADER_CREATION_UDP_IPV4, dst_addr, dstPort);

  std::shared_ptr<pfcp::pfcp_session> pSession = std::make_shared<pfcp::pfcp_session>();
  pfcp::offending_ie_t offending_ie = {};
  pfcp::fteid_t allocated_fteid;
  pfcp::cause_t cause = {pfcp::CAUSE_VALUE_REQUEST_ACCEPTED};
  // FIX: why is not creating the UL. Maybe the DL is overwriting.
  pSession->create(*pPdrUL, cause, offending_ie.offending_ie, allocated_fteid);
  pSession->create(*pFarUL, cause, offending_ie.offending_ie);
  pSession->create(*pPdrDL, cause, offending_ie.offending_ie, allocated_fteid);
  pSession->create(*pFarDL, cause, offending_ie.offending_ie);

  // Case: create and remove session (happy path).
  // Check if the PDRs were reorded.
  // Check if the FARProgramMap has one program.
  // Check if the maps in UPFProgram have the elements.
  // Check if the maps in FARProgram have the elements.
  EXPECT_NO_THROW(mpSessionManager->createBPFSession(pSession));
  EXPECT_EQ(pSession->pdrs[0]->pdr_id.rule_id, pdrIdUL);
  EXPECT_NO_THROW(mpSessionManager->removeBPFSession(pSession->get_up_seid()));

  // EXPECT_NO_THROW(mpSessionManager->removeSession(pSession->getSeid()));

  // // Case: remove with an empty list.
  // EXPECT_ANY_THROW(mpSessionManager->removeSession(pSession->getSeid()));
}

TEST_F(SessionManagerTests, managePDR)
{
  LOG_FUNC();
  // Proprietary structs.
  std::shared_ptr<pfcp_pdr_t_> pPdrProprietary = std::make_shared<pfcp_pdr_t_>();
  std::shared_ptr<pfcp_pdr_t_> pPdrUpdatedProprietary = std::make_shared<pfcp_pdr_t_>();
  std::shared_ptr<pfcp_pdr_t_> pPdr2Proprietary = std::make_shared<pfcp_pdr_t_>();
  std::shared_ptr<pfcp_session_t_> pSessionRaw = std::make_shared<pfcp_session_t_>();

  // Session identifier.
  pSessionRaw->seid = 1;
  std::shared_ptr<SessionBpf> pSession = std::make_shared<SessionBpfImpl>(*pSessionRaw);

  // UL PDR 1.
  pPdrProprietary->pdr_id.rule_id = 100;
  pPdrProprietary->far_id.far_id = 100;
  pPdrProprietary->pdi.fteid.teid = 10;
  pPdrProprietary->pdi.source_interface.interface_value = INTERFACE_VALUE_ACCESS;

  // UL PDR 1 updated.
  pPdrUpdatedProprietary->pdr_id.rule_id = 100;
  pPdrUpdatedProprietary->far_id.far_id = 101;
  pPdrUpdatedProprietary->pdi.fteid.teid = 11;
  pPdrUpdatedProprietary->pdi.source_interface.interface_value = INTERFACE_VALUE_ACCESS;

  // PDR 2.
  pPdr2Proprietary->pdr_id.rule_id = 101;

  // Adapts proprietary struct to the interfaces.
  std::shared_ptr<PacketDetectionRules> pPdr = std::make_shared<PacketDetectionRulesImpl>(*pPdrProprietary);
  std::shared_ptr<PacketDetectionRules> pPdrUpdated = std::make_shared<PacketDetectionRulesImpl>(*pPdrUpdatedProprietary);
  std::shared_ptr<PacketDetectionRules> pPdr2 = std::make_shared<PacketDetectionRulesImpl>(*pPdr2Proprietary);

  EXPECT_NO_THROW(mpSessionManager->createSession(pSession));

  LOG_INF("Case: add, lookup and remove (happy path)");
  EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->getSeid(), pPdr));
  EXPECT_TRUE(mpSessionManager->lookupPDR(pSession->getSeid(), pPdr->getPdrId().rule_id)->getPdrId().rule_id == pPdr->getPdrId().rule_id);
  EXPECT_NO_THROW(mpSessionManager->removePDR(pSession->getSeid(), pPdr));

  LOG_INF("Case: remove without adding");
  EXPECT_ANY_THROW(mpSessionManager->removePDR(pSession->getSeid(), pPdr));

  LOG_INF("Case: update without adding");
  EXPECT_ANY_THROW(mpSessionManager->updatePDR(pSession->getSeid(), pPdr));

  LOG_INF("Case: add, update, lookup and remove")
  // Add PDR in session and PDR maps.
  EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->getSeid(), pPdr));
  // Update TEID and FAR_ID.
  EXPECT_NO_THROW(mpSessionManager->updatePDR(pSession->getSeid(), pPdrUpdated));
  // Check if FAR_ID was updated.
  EXPECT_TRUE(mpSessionManager->lookupPDR(pSession->getSeid(), pPdr->getPdrId().rule_id)->getFarId().far_id != pPdr->getFarId().far_id);
  // Remove old PDR. Tryin to remove TEID which does not exists. So it will fail when remove from the program map.
  EXPECT_ANY_THROW(mpSessionManager->removePDR(pSession->getSeid(), pPdr));
  // Remove new PDR. TEID is updated and there is a PDR ID.
  EXPECT_NO_THROW(mpSessionManager->removePDR(pSession->getSeid(), pPdrUpdated));

  LOG_INF("Case: lookup with an empty list");
  EXPECT_FALSE(mpSessionManager->lookupPDR(pSession->getSeid(), pPdr->getPdrId().rule_id));

  LOG_INF("Case: lookup with a non-empty list");
  // Add PDR in session and PDR maps.
  EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->getSeid(), pPdr));
  // Lookup PDR2, it will return false.
  EXPECT_FALSE(mpSessionManager->lookupPDR(pSession->getSeid(), pPdr2->getPdrId().rule_id));
  // Remove PDR1 in the session and PDR maps.
  EXPECT_NO_THROW(mpSessionManager->removePDR(pSession->getSeid(), pPdr));

  // LOG_INF("Case: buffer overflow");
  // for(uint8_t i = 0; i < SESSION_PDRS_MAX_SIZE; i++) {
  //   EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->getSeid(), pPdr));
  // }

  // // FIXME navarrothiago -
  // EXPECT_ANY_THROW(mpSessionManager->addPDR(pSession->getSeid(), pPdr));

  EXPECT_NO_THROW(mpSessionManager->removeSession(pSession->getSeid()));
}

TEST_F(SessionManagerTests, manageFAR)
{
  LOG_FUNC();

  // Proprietary structs.
  std::shared_ptr<pfcp_far_t_> pFarProprietary = std::make_shared<pfcp_far_t_>();
  std::shared_ptr<pfcp_far_t_> pFarUpdatedProprietary = std::make_shared<pfcp_far_t_>();
  std::shared_ptr<pfcp_far_t_> pFar2Proprietary = std::make_shared<pfcp_far_t_>();
  std::shared_ptr<pfcp_session_t_> pSessionRaw = std::make_shared<pfcp_session_t_>();

  // Session identifier.
  pSessionRaw->seid = 1;
  std::shared_ptr<SessionBpf> pSession = std::make_shared<SessionBpfImpl>(*pSessionRaw);

  // FAR 1.
  pFarProprietary->far_id.far_id = 100;
  pFarProprietary->apply_action.drop = 1;

  // FAR 1 updated.
  pFarUpdatedProprietary->far_id.far_id = 100;
  pFarUpdatedProprietary->apply_action.drop = 0;

  // FAR 2.
  pFar2Proprietary->far_id.far_id = 101;

  // Adapts proprietary struct to the interfaces.
  std::shared_ptr<ForwardingActionRules> pFar = std::make_shared<ForwardingActionRulesImpl>(*pFarProprietary);
  std::shared_ptr<ForwardingActionRules> pFarUpdated = std::make_shared<ForwardingActionRulesImpl>(*pFarUpdatedProprietary);
  std::shared_ptr<ForwardingActionRules> pFar2 = std::make_shared<ForwardingActionRulesImpl>(*pFar2Proprietary);

  // Create the session that will be used in the tests.
  EXPECT_NO_THROW(mpSessionManager->createSession(pSession));

  LOG_INF("Case: add, lookup and remove (happy path)");
  EXPECT_NO_THROW(mpSessionManager->addFAR(pSession->getSeid(), pFar));
  EXPECT_TRUE(mpSessionManager->lookupFAR(pSession->getSeid(), pFar->getFARId().far_id)->getFARId().far_id == pFar->getFARId().far_id);
  EXPECT_NO_THROW(mpSessionManager->removeFAR(pSession->getSeid(), pFar));
  LOG_INF("Case: remove without adding");
  EXPECT_ANY_THROW(mpSessionManager->removeFAR(pSession->getSeid(), pFar));

  LOG_INF("Case: update without adding");
  EXPECT_ANY_THROW(mpSessionManager->updateFAR(pSession->getSeid(), pFar));

  LOG_INF("Case:  add, update, lookup and remove")
  EXPECT_NO_THROW(mpSessionManager->addFAR(pSession->getSeid(), pFar));
  EXPECT_NO_THROW(mpSessionManager->updateFAR(pSession->getSeid(), pFarUpdated));
  EXPECT_TRUE(mpSessionManager->lookupFAR(pSession->getSeid(), pFar->getFARId().far_id)->getApplyRules().drop != pFar->getApplyRules().drop);
  EXPECT_NO_THROW(mpSessionManager->removeFAR(pSession->getSeid(), pFar));

  LOG_INF("Case: lookup with an empty list");
  EXPECT_FALSE(mpSessionManager->lookupFAR(pSession->getSeid(), pFar->getFARId().far_id));

  LOG_INF("Case: lookup an with a non-empty list");
  EXPECT_NO_THROW(mpSessionManager->addFAR(pSession->getSeid(), pFar));
  EXPECT_FALSE(mpSessionManager->lookupFAR(pSession->getSeid(), pFar2->getFARId().far_id));
  EXPECT_NO_THROW(mpSessionManager->removeFAR(pSession->getSeid(), pFar));

  // LOG_INF("Case: buffer overflow");
  // for(uint8_t i = 0; i < SESSION_FARS_MAX_SIZE; i++) {
  //   EXPECT_NO_THROW(mpSessionManager->addFAR(pSession->getSeid(), pFar));
  // }
  // EXPECT_ANY_THROW(mpSessionManager->addFAR(pSession->getSeid(), pFar));
  // for(uint8_t i = 0; i < SESSION_FARS_MAX_SIZE; i++) {
  //   EXPECT_NO_THROW(mpSessionManager->removeFAR(pSession->getSeid(), pFar));
  // }
}
