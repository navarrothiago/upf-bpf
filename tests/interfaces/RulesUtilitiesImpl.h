#ifndef __RULESFACTORYIMPL_H__
#define __RULESFACTORYIMPL_H__

#include <interfaces/RulesUtilities.h>

class RulesUtilitiesImpl : public RulesUtilities
{
public:
  RulesUtilitiesImpl(/* args */);
  virtual ~RulesUtilitiesImpl();
  void copyFAR(pfcp_far_t *pFarDestination, ForwardingActionRules *pFarSource) override;
  std::shared_ptr<ForwardingActionRules> createFAR(pfcp_far_t *pFarSource) override;
};
#endif // __RULESFACTORYIMPL_H__
