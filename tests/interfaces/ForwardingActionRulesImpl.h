#ifndef __FORWARDINGACTIONRULESIMPL_H__
#define __FORWARDINGACTIONRULESIMPL_H__

#include <interfaces/ForwardingActionRules.h>
#include <pfcp/pfcp_far.h>

/**
 * @brief FAR implementation's example.
 *
 */
class ForwardingActionRulesImpl : public ForwardingActionRules
{
public:
  ForwardingActionRulesImpl(pfcp_far_t &myFarStruct);
  ~ForwardingActionRulesImpl();
  far_id_t getFARId() override;
  apply_action_t getApplyRules() override;
  forwarding_parameters_t getForwardingParameters() override;
  duplicating_parameters_t getDuplicatingParameters() override;
  bar_id_t getBarId() override;
  pfcp_far_t getData() override;

private:
  pfcp_far_t mFar;
};

#endif // __FORWARDINGACTIONRULESIMPL_H__
