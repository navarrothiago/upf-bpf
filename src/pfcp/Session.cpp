#include <Session.h>
#include <utils/LogDefines.h>

Session::Session(u64 seid)
  : mSeid(seid)
{
  LOG_FUNC();
}

Session::~Session()
{
  LOG_FUNC();
}

void Session::addPDR(std::shared_ptr<PacketDetectionRule> pPdr)
{
  LOG_FUNC();
  mPDRs.push_back(pPdr);
  mPDRs.sort(Session::comparePDR);
}

void Session::addFAR(std::shared_ptr<ForwardingActionRule> pFar)
{
  LOG_FUNC();
  mFARs.push_back(pFar);
}

void Session::addPDR(std::shared_ptr<pfcp::pfcp_pdr> pPdr)
{
  LOG_FUNC();
  // mpSession->pdrs
}

void Session::addFAR(std::shared_ptr<pfcp::pfcp_far> pFAR)
{
  LOG_FUNC();
}

bool Session::comparePDR(const std::shared_ptr<PacketDetectionRule>& first, const std::shared_ptr<PacketDetectionRule>& second)
{
  LOG_FUNC();
  return first->getPrecedence() < second->getPrecedence();
}

std::shared_ptr<PacketDetectionRule> Session::getHighestPrecedencePDR()
{
  LOG_FUNC();
  return *mPDRs.begin();
}
