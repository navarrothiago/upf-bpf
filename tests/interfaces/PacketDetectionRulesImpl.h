#ifndef __PACKETDETECTIONRULESIMPL_H__
#define __PACKETDETECTIONRULESIMPL_H__

#include <interfaces/PacketDetectionRules.h>
#include <pfcp/pfcp_pdr.h>

class PacketDetectionRulesImpl : public PacketDetectionRules
{
public:
  PacketDetectionRulesImpl(pfcp_pdr_t_ &myPdr);
  virtual ~PacketDetectionRulesImpl();
  uint64_t getLocalSeid() override;
  teid_t_ getTeid() override;
  pdr_id_t_ getPdrId() override;
  precedence_t_ getPrecedence() override;
  pdi_t_ getPdi() override;
  outer_header_removal_t_ getOuterHeaderRemoval() override;
  far_id_t_ getFarId() override;
  urr_id_t_ gerUrrId() override;
  qer_id_t_ getQerId() override;
  activate_predefined_rules_t_ getActivatePredefinedRules() override;
  pfcp_pdr_t_ getData() override;


private:
  pfcp_pdr_t_ mPdr;
};

#endif // __PACKETDETECTIONRULESIMPL_H__
