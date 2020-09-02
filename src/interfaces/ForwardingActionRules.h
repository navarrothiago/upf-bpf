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
  virtual far_id_t getFARId() = 0;
  virtual apply_action_t getApplyRules() = 0;
  virtual forwarding_parameters_t getForwardingParameters() = 0;
  virtual duplicating_parameters_t getDuplicatingParameters() = 0;
  virtual bar_id_t getBarId() = 0;
  virtual pfcp_far_t getData() = 0;
};

#endif // __FORWARDINGACTIONRULES_H__
