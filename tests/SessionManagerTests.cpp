#include <SessionManager.h>
#include <UPFProgramManager.h>
#include <gtest/gtest.h>
#include <pfcp/pfcp_pdr.h>
#include <utils/LogDefines.h>

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
  EXPECT_NO_THROW(mpSessionManager->createSession(pSession));
  EXPECT_NO_THROW(mpSessionManager->removeSession(pSession->seid));
  EXPECT_ANY_THROW(mpSessionManager->removeSession(pSession->seid));
}

TEST_F(SessionManagerTests, managePDR)
{
  LOG_FUNC();
  std::shared_ptr<pfcp_pdr_t> pPdr = std::make_shared<pfcp_pdr_t>();
  std::shared_ptr<pfcp_pdr_t> pPdr2 = std::make_shared<pfcp_pdr_t>();
  std::shared_ptr<pfcp_session_t> pSession = std::make_shared<pfcp_session_t>();

  pSession->seid = 1;
  pPdr->pdr_id.rule_id = 100;
  pPdr2->pdr_id.rule_id = 101;

  EXPECT_NO_THROW(mpSessionManager->createSession(pSession));

  // Case: add and remove (happy path).
  EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->seid, pPdr));
  EXPECT_NO_THROW(mpSessionManager->removePDR(pSession->seid, pPdr));

  // Case: remove without adding.
  EXPECT_ANY_THROW(mpSessionManager->removePDR(pSession->seid, pPdr));

  // Case:  add, update, lookup and remove.
  EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->seid, pPdr));
  EXPECT_TRUE(mpSessionManager->lookupPDR(pSession->seid, pPdr->pdr_id)->pdr_id.rule_id == pPdr->pdr_id.rule_id);
  EXPECT_NO_THROW(mpSessionManager->removePDR(pSession->seid, pPdr));

  // Case: lookup in empty list.
  EXPECT_FALSE(mpSessionManager->lookupPDR(pSession->seid, pPdr->pdr_id));

  // Case: lookup in non-empty list.
  EXPECT_NO_THROW(mpSessionManager->addPDR(pSession->seid, pPdr));
  EXPECT_FALSE(mpSessionManager->lookupPDR(pSession->seid, pPdr2->pdr_id));
}
