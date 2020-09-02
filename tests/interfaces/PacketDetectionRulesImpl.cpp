#include "PacketDetectionRulesImpl.h"
#include <utils/LogDefines.h>

PacketDetectionRulesImpl::PacketDetectionRulesImpl(pfcp_pdr_t &myPdr)
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

teid_t PacketDetectionRulesImpl::getTeid()
{
  LOG_FUNC();
  return mPdr.teid;
}

pdr_id_t PacketDetectionRulesImpl::getPdrId()
{
  LOG_FUNC();
  return mPdr.pdr_id;
}

precedence_t PacketDetectionRulesImpl::getPrecedence()
{
  LOG_FUNC();
  return mPdr.precedence;
}

pdi_t PacketDetectionRulesImpl::getPdi()
{
  LOG_FUNC();
  return mPdr.pdi;
}

outer_header_removal_t PacketDetectionRulesImpl::getOuterHeaderRemoval()
{
  LOG_FUNC();
  return mPdr.outer_header_removal;
}

far_id_t PacketDetectionRulesImpl::getFarId()
{
  LOG_FUNC();
  return mPdr.far_id;
}

urr_id_t PacketDetectionRulesImpl::gerUrrId()
{
  LOG_FUNC();
  return mPdr.urr_id;
}

qer_id_t PacketDetectionRulesImpl::getQerId()
{
  LOG_FUNC();
  return mPdr.qer_id;
}

activate_predefined_rules_t PacketDetectionRulesImpl::getActivatePredefinedRules()
{
  LOG_FUNC();
  return mPdr.activate_predefined_rules;
}

pfcp_pdr_t PacketDetectionRulesImpl::getData()
{
  LOG_FUNC();
  return mPdr;
}
