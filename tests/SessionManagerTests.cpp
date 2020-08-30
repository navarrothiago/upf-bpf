#include <SessionManager.h>
#include <UPFProgramManager.h>
#include <gtest/gtest.h>
#include <pfcp/pfcp_pdr.h>
#include <utils/LogDefines.h>
#include <pfcp/pfcp_session.h>

class SessionManagerTests : public ::testing::Test
{
public:
  void SetUp() override
  {
    LOG_FUNC();
    UPFProgramManager::getInstance().setup();
    mpSessionManager = UPFProgramManager::getInstance().getSessionManager();
  }
  void TearDown() override
  {
    LOG_FUNC();
    UPFProgramManager::getInstance().tearDown();
    mpSessionManager.reset();
  }
  virtual ~SessionManagerTests() {}
  std::shared_ptr<SessionManager> mpSessionManager;
};

TEST_F(SessionManagerTests, manageSession)
{
  LOG_FUNC();
  std::shared_ptr<pfcp_session_t> pSession = std::make_shared<pfcp_session_t>();

  // Case: create and remove session (happy path).
  EXPECT_NO_THROW(mpSessionManager->createSession(pSession));
  EXPECT_NO_THROW(mpSessionManager->removeSession(pSession->seid));

  // Case: remove with an empty list.
  EXPECT_ANY_THROW(mpSessionManager->removeSession(pSession->seid));
}

TEST_F(SessionManagerTests, managePDR)
{
  LOG_FUNC();
  std::shared_ptr<pfcp_pdr_t> pPdr = std::make_shared<pfcp_pdr_t>();
  std::shared_ptr<pfcp_pdr_t> pPdrUpdated = std::make_shared<pfcp_pdr_t>();
  std::shared_ptr<pfcp_pdr_t> pPdr2 = std::make_shared<pfcp_pdr_t>();
  std::shared_ptr<pfcp_session_t> pSession = std::make_shared<pfcp_session_t>();

  // Session identifier.
  pSession->seid = 1;

  // PDR 1.
  pPdr->pdr_id.rule_id = 100;
  pPdr->far_id.far_id = 100;

  // PDR 1 updated.
  pPdrUpdated->pdr_id.rule_id = 100;
  pPdrUpdated->far_id.far_id = 101;

  // PDR 2.
  pPdr2->pdr_id.rule_id = 101;

  EXPECT_NO_THROW(mpSessionManager->createSession(pSession));

  LOG_INFO("Case: add, lookup and remove (happy path)");
  EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->seid, pPdr));
  EXPECT_TRUE(mpSessionManager->lookupPDR(pSession->seid, pPdr->pdr_id)->pdr_id.rule_id == pPdr->pdr_id.rule_id);
  EXPECT_NO_THROW(mpSessionManager->removePDR(pSession->seid, pPdr));

  LOG_INFO("Case: remove without adding");
  EXPECT_ANY_THROW(mpSessionManager->removePDR(pSession->seid, pPdr));

  LOG_INFO("Case: update without adding");
  EXPECT_ANY_THROW(mpSessionManager->updatePDR(pSession->seid, pPdr));

  LOG_INFO("Case:  add, update, lookup and remove")
  EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->seid, pPdr));
  EXPECT_NO_THROW(mpSessionManager->updatePDR(pSession->seid, pPdrUpdated));
  EXPECT_TRUE(mpSessionManager->lookupPDR(pSession->seid, pPdr->pdr_id)->far_id.far_id != pPdr->far_id.far_id);
  EXPECT_NO_THROW(mpSessionManager->removePDR(pSession->seid, pPdr));

  LOG_INFO("Case: lookup with an empty list");
  EXPECT_FALSE(mpSessionManager->lookupPDR(pSession->seid, pPdr->pdr_id));

  LOG_INFO("Case: lookup an with a non-empty list");
  EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->seid, pPdr));
  EXPECT_FALSE(mpSessionManager->lookupPDR(pSession->seid, pPdr2->pdr_id));
  EXPECT_NO_THROW(mpSessionManager->removePDR(pSession->seid, pPdr));

  LOG_INFO("Case: buffer overflow");
  for(uint8_t i = 0; i < SESSION_PDRS_MAX_SIZE; i++) {
    EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->seid, pPdr));
  }
  EXPECT_ANY_THROW(mpSessionManager->addPDR(pSession->seid, pPdr));

  EXPECT_NO_THROW(mpSessionManager->removeSession(pSession->seid));
}

TEST_F(SessionManagerTests, manageFAR)
{
  LOG_FUNC();
  std::shared_ptr<pfcp_far_t> pFar = std::make_shared<pfcp_far_t>();
  std::shared_ptr<pfcp_far_t> pFarUpdated = std::make_shared<pfcp_far_t>();
  std::shared_ptr<pfcp_far_t> pFar2 = std::make_shared<pfcp_far_t>();
  std::shared_ptr<pfcp_session_t> pSession = std::make_shared<pfcp_session_t>();

  // Session identifier.
  pSession->seid = 1;

  // FAR 1.
  pFar->far_id.far_id = 100;
  pFar->apply_action.drop = 1;

  // FAR 1 updated.
  pFarUpdated->far_id.far_id = 100;
  pFarUpdated->apply_action.drop = 0;

  // PDR 2.
  pFar2->far_id.far_id = 101;

  EXPECT_NO_THROW(mpSessionManager->createSession(pSession));

  LOG_INFO("Case: add, lookup and remove (happy path)");
  EXPECT_NO_THROW(mpSessionManager->addFAR(pSession->seid, pFar));
  EXPECT_TRUE(mpSessionManager->lookupFAR(pSession->seid, pFar->far_id)->far_id.far_id == pFar->far_id.far_id);
  EXPECT_NO_THROW(mpSessionManager->removeFAR(pSession->seid, pFar));

  LOG_INFO("Case: remove without adding");
  EXPECT_ANY_THROW(mpSessionManager->removeFAR(pSession->seid, pFar));

  LOG_INFO("Case: update without adding");
  EXPECT_ANY_THROW(mpSessionManager->updateFAR(pSession->seid, pFar));

  LOG_INFO("Case:  add, update, lookup and remove")
  EXPECT_NO_THROW(mpSessionManager->addFAR(pSession->seid, pFar));
  EXPECT_NO_THROW(mpSessionManager->updateFAR(pSession->seid, pFarUpdated));
  EXPECT_TRUE(mpSessionManager->lookupFAR(pSession->seid, pFar->far_id)->apply_action.drop != pFar->apply_action.drop);
  EXPECT_NO_THROW(mpSessionManager->removeFAR(pSession->seid, pFar));

  LOG_INFO("Case: lookup with an empty list");
  EXPECT_FALSE(mpSessionManager->lookupFAR(pSession->seid, pFar->far_id));

  LOG_INFO("Case: lookup an with a non-empty list");
  EXPECT_NO_THROW(mpSessionManager->addFAR(pSession->seid, pFar));
  EXPECT_FALSE(mpSessionManager->lookupFAR(pSession->seid, pFar2->far_id));
  EXPECT_NO_THROW(mpSessionManager->removeFAR(pSession->seid, pFar));

  LOG_INFO("Case: buffer overflow");
  for(uint8_t i = 0; i < SESSION_FARS_MAX_SIZE; i++) {
    EXPECT_NO_THROW(mpSessionManager->addFAR(pSession->seid, pFar));
  }
  EXPECT_ANY_THROW(mpSessionManager->addFAR(pSession->seid, pFar));
}
