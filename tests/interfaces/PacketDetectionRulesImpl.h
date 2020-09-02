#ifndef __PACKETDETECTIONRULESIMPL_H__
#define __PACKETDETECTIONRULESIMPL_H__

#include <interfaces/PacketDetectionRules.h>
#include <pfcp/pfcp_pdr.h>

class PacketDetectionRulesImpl : public PacketDetectionRules
{
public:
  PacketDetectionRulesImpl(pfcp_pdr_t &myPdr);
  virtual ~PacketDetectionRulesImpl();
  uint64_t getLocalSeid() override;
  teid_t getTeid() override;
  pdr_id_t getPdrId() override;
  precedence_t getPrecedence() override;
  pdi_t getPdi() override;
  outer_header_removal_t getOuterHeaderRemoval() override;
  far_id_t getFarId() override;
  urr_id_t gerUrrId() override;
  qer_id_t getQerId() override;
  activate_predefined_rules_t getActivatePredefinedRules() override;
  pfcp_pdr_t getData() override;


private:
  pfcp_pdr_t mPdr;
};

#endif // __PACKETDETECTIONRULESIMPL_H__
