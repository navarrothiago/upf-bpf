#ifndef __SESSION_H__
#define __SESSION_H__

#include "PacketDetectionRule.h"
#include "ForwardingActionRule.h"
#include <list>
#include <vector>
#include <memory>
#include "pfcp_session.hpp"
#include "pfcp_pdr.hpp"
#include "pfcp_far.hpp"

class Session
{
public:
  Session(u64 seid);
  virtual ~Session();
  void addPDR(std::shared_ptr<PacketDetectionRule> pPdr);
  void addFAR(std::shared_ptr<ForwardingActionRule> pFar);
  void addPDR(std::shared_ptr<pfcp::pfcp_pdr> pPdr);
  void addFAR(std::shared_ptr<pfcp::pfcp_far> pFAR);
  static bool comparePDR(const std::shared_ptr<PacketDetectionRule>& first, const std::shared_ptr<PacketDetectionRule>& second);
  std::shared_ptr<PacketDetectionRule> getHighestPrecedencePDR();

private:

 std::list<std::shared_ptr<PacketDetectionRule>> mPDRs;
 std::vector<std::shared_ptr<ForwardingActionRule>> mFARs;
 u64 mSeid;
 std::shared_ptr<pfcp::pfcp_session> mpSession;
};

#endif // __SESSION_H__
