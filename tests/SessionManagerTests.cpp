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
