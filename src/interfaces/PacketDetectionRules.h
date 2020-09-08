#ifndef __PACKETDETECTIONRULES_H__
#define __PACKETDETECTIONRULES_H__

#include <pfcp/pfcp_pdr.h>
#include <cstdint>

/**
 * @brief Interface to abstract PDR.
 *
 */
class PacketDetectionRules
{
public:
  PacketDetectionRules(/* args */) {}
  virtual ~PacketDetectionRules() {}
  virtual uint64_t getLocalSeid() = 0;
  virtual teid_t_ getTeid() = 0;
  virtual pdr_id_t_ getPdrId() = 0;
  virtual precedence_t_ getPrecedence() = 0;
  virtual pdi_t_ getPdi() = 0;
  virtual outer_header_removal_t_ getOuterHeaderRemoval() = 0;
  virtual far_id_t_ getFarId() = 0;
  virtual urr_id_t_ gerUrrId() = 0;
  virtual qer_id_t_ getQerId() = 0;
  virtual activate_predefined_rules_t_ getActivatePredefinedRules() = 0;
  virtual pfcp_pdr_t_ getData() = 0;
};
#endif // __PACKETDETECTIONRULES_H__
