#include "ForwardingActionRulesImpl.h"
#include <utils/LogDefines.h>

ForwardingActionRulesImpl::ForwardingActionRulesImpl(pfcp_far_t &myFarStruct)
  : ForwardingActionRules()
{
  LOG_FUNC();
  mFar = myFarStruct;
}

ForwardingActionRulesImpl::~ForwardingActionRulesImpl()
{
  LOG_FUNC();
}

far_id_t ForwardingActionRulesImpl::getFARId()
{
  // Do not put LOG_FUNC() here.
  return mFar.far_id;
}

apply_action_t ForwardingActionRulesImpl::getApplyRules()
{
  LOG_FUNC();
  return mFar.apply_action;
}

forwarding_parameters_t ForwardingActionRulesImpl::getForwardingParameters()
{
  LOG_FUNC();
  return mFar.forwarding_parameters;
}

duplicating_parameters_t ForwardingActionRulesImpl::getDuplicatingParameters()
{
  LOG_FUNC();
  return mFar.duplicating_parameters;
}

bar_id_t ForwardingActionRulesImpl::getBarId()
{
  LOG_FUNC();
  return mFar.bar_id;
}

pfcp_far_t ForwardingActionRulesImpl::getData()
{
  LOG_FUNC();
  return mFar;
}
