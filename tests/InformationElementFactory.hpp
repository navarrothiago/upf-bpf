#ifndef __INFORMATIONELEMENTFACTORY_H__
#define __INFORMATIONELEMENTFACTORY_H__

#include <memory>
#include <interfaces/SessionBpfImpl.h>
#include <interfaces/PacketDetectionRulesImpl.h>
#include <interfaces/ForwardingActionRulesImpl.h>
#include <pfcp/pfcp_session.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_far.h>
#include <utils/LogDefines.h>
#include <ie/apply_action.h>
#include <ie/fseid.h>
#include <SessionBpf.h>

std::shared_ptr<SessionBpf> createSession(seid_t_ seid)
{
  LOG_FUNC();
  std::shared_ptr<pfcp_session_t_> pSessionRaw = std::make_shared<pfcp_session_t_>();
  pSessionRaw->seid = seid;
  return std::make_shared<SessionBpfImpl>(*pSessionRaw);
}

std::shared_ptr<PacketDetectionRules> createPDR(u16 pdrId, u32 farId, u32 teid, u32 sourceInterface, struct in_addr ueIPv4, u16 outerHeaderRemoval)
{
  LOG_FUNC();
  // Proprietary struct.
  std::shared_ptr<pfcp_pdr_t_> pPdrProprietary = std::make_shared<pfcp_pdr_t_>();

  // PDR.
  pPdrProprietary->pdr_id.rule_id = pdrId;
  pPdrProprietary->far_id.far_id = farId;
  pPdrProprietary->outer_header_removal.outer_header_removal_description = outerHeaderRemoval;
  pPdrProprietary->pdi.fteid.teid = teid;
  pPdrProprietary->pdi.source_interface.interface_value = sourceInterface;
  pPdrProprietary->pdi.ue_ip_address.ipv4_address = ueIPv4.s_addr;

  // Adapts proprietary struct to the interfaces.
  return std::make_shared<PacketDetectionRulesImpl>(*pPdrProprietary);
}

std::shared_ptr<ForwardingActionRules> createFAR(u32 farId, apply_action_t_ actions, u32 destinationInterface, u16 outerHeadeCreation, struct in_addr destinationIPv4, u16 destinationPort)
{
  LOG_FUNC();
  // Proprietary structs.
  std::shared_ptr<pfcp_far_t_> pFarProprietary = std::make_shared<pfcp_far_t_>();

  // FAR.
  pFarProprietary->far_id.far_id = farId;
  pFarProprietary->apply_action = actions;
  pFarProprietary->forwarding_parameters.outer_header_creation.outer_header_creation_description = outerHeadeCreation;
  pFarProprietary->forwarding_parameters.outer_header_creation.ipv4_address = destinationIPv4;
  pFarProprietary->forwarding_parameters.outer_header_creation.port_number = destinationPort;
  pFarProprietary->forwarding_parameters.destination_interface.interface_value = destinationInterface;

  // Adapts proprietary struct to the interfaces.
  return std::make_shared<ForwardingActionRulesImpl>(*pFarProprietary);
}

#endif // __INFORMATIONELEMENTFACTORY_H__