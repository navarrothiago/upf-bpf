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
  virtual teid_t getTeid() = 0;
  virtual pdr_id_t getPdrId() = 0;
  virtual precedence_t getPrecedence() = 0;
  virtual pdi_t getPdi() = 0;
  virtual outer_header_removal_t getOuterHeaderRemoval() = 0;
  virtual far_id_t getFarId() = 0;
  virtual urr_id_t gerUrrId() = 0;
  virtual qer_id_t getQerId() = 0;
  virtual activate_predefined_rules_t getActivatePredefinedRules() = 0;
  virtual pfcp_pdr_t getData() = 0;
};
#endif // __PACKETDETECTIONRULES_H__
