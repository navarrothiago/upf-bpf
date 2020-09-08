#ifndef __RULESFACTORYIMPL_H__
#define __RULESFACTORYIMPL_H__

#include <interfaces/RulesUtilities.h>

class RulesUtilitiesImpl : public RulesUtilities
{
public:
  RulesUtilitiesImpl(/* args */);
  virtual ~RulesUtilitiesImpl();
  void copyFAR(pfcp_far_t_ *pFarDestination, ForwardingActionRules *pFarSource) override;
  std::shared_ptr<ForwardingActionRules> createFAR(pfcp_far_t_ *pFarSource) override;
  void copyPDR(pfcp_pdr_t_ *pPdrDestination, PacketDetectionRules *pPdrSource) override;
  std::shared_ptr<PacketDetectionRules> createPDR(pfcp_pdr_t_ *pPdrSource) override;
};
#endif // __RULESFACTORYIMPL_H__
