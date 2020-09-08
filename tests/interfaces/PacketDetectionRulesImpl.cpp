#include "PacketDetectionRulesImpl.h"
#include <utils/LogDefines.h>

PacketDetectionRulesImpl::PacketDetectionRulesImpl(pfcp_pdr_t_ &myPdr)
{
  LOG_FUNC();
  mPdr = myPdr;
}

PacketDetectionRulesImpl::~PacketDetectionRulesImpl()
{
  LOG_FUNC();
}

uint64_t PacketDetectionRulesImpl::getLocalSeid()
{
  LOG_FUNC();
  return mPdr.local_seid;
}

teid_t_ PacketDetectionRulesImpl::getTeid()
{
  LOG_FUNC();
  return mPdr.teid;
}

pdr_id_t_ PacketDetectionRulesImpl::getPdrId()
{
  LOG_FUNC();
  return mPdr.pdr_id;
}

precedence_t_ PacketDetectionRulesImpl::getPrecedence()
{
  LOG_FUNC();
  return mPdr.precedence;
}

pdi_t_ PacketDetectionRulesImpl::getPdi()
{
  LOG_FUNC();
  return mPdr.pdi;
}

outer_header_removal_t_ PacketDetectionRulesImpl::getOuterHeaderRemoval()
{
  LOG_FUNC();
  return mPdr.outer_header_removal;
}

far_id_t_ PacketDetectionRulesImpl::getFarId()
{
  LOG_FUNC();
  return mPdr.far_id;
}

urr_id_t_ PacketDetectionRulesImpl::gerUrrId()
{
  LOG_FUNC();
  return mPdr.urr_id;
}

qer_id_t_ PacketDetectionRulesImpl::getQerId()
{
  LOG_FUNC();
  return mPdr.qer_id;
}

activate_predefined_rules_t_ PacketDetectionRulesImpl::getActivatePredefinedRules()
{
  LOG_FUNC();
  return mPdr.activate_predefined_rules;
}

pfcp_pdr_t_ PacketDetectionRulesImpl::getData()
{
  LOG_FUNC();
  return mPdr;
}
