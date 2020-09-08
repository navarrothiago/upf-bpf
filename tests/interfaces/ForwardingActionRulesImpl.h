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
  ForwardingActionRulesImpl(pfcp_far_t_ &myFarStruct);
  ~ForwardingActionRulesImpl();
  far_id_t_ getFARId() override;
  apply_action_t_ getApplyRules() override;
  forwarding_parameters_t_ getForwardingParameters() override;
  duplicating_parameters_t_ getDuplicatingParameters() override;
  bar_id_t_ getBarId() override;
  pfcp_far_t_ getData() override;

private:
  pfcp_far_t_ mFar;
};

#endif // __FORWARDINGACTIONRULESIMPL_H__
