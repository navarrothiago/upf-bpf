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
  }
  void TearDown() override
  {
    LOG_FUNC();
    UPFProgramManager::getInstance().tearDown();
    sleep(1);
  }
  virtual ~SessionManagerTests() {}
};

TEST_F(SessionManagerTests, createSession)
{
  LOG_FUNC();
  std::shared_ptr<pfcp_session_t> pSession = std::make_shared<pfcp_session_t>();
  EXPECT_NO_THROW(SessionManager::getInstance().createSession(pSession));
  EXPECT_NO_THROW(SessionManager::getInstance().removeSession(pSession->seid));
  EXPECT_ANY_THROW(SessionManager::getInstance().removeSession(pSession->seid));
}

TEST_F(SessionManagerTests, addPDR)
{
  LOG_FUNC();
  std::shared_ptr<pfcp_pdr_t> pPdr = std::make_shared<pfcp_pdr_t>();
}
