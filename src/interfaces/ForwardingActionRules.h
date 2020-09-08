#ifndef __FORWARDINGACTIONRULES_H__
#define __FORWARDINGACTIONRULES_H__

#include <pfcp/pfcp_far.h>

/**
 * @brief Interface to abstract the FAR.
 *
 */
class ForwardingActionRules
{
public:
  ForwardingActionRules(/* args */) {}
  virtual ~ForwardingActionRules() {}
  virtual far_id_t_ getFARId() = 0;
  virtual apply_action_t_ getApplyRules() = 0;
  virtual forwarding_parameters_t_ getForwardingParameters() = 0;
  virtual duplicating_parameters_t_ getDuplicatingParameters() = 0;
  virtual bar_id_t_ getBarId() = 0;
  virtual pfcp_far_t_ getData() = 0;
};

#endif // __FORWARDINGACTIONRULES_H__
