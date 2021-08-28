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
#include <msg_pfcp.hpp>
#include <3gpp_29.244.hpp>

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

std::shared_ptr<pfcp::create_pdr> createOaiPDR(u16 pdrId, u32 farId, u32 teid, u32 sourceInterface, struct in_addr ueIPv4, u16 outerHeaderRemoval, u32 precedence)
{
  LOG_FUNC();
  std::shared_ptr<pfcp::create_pdr> pPdr = std::make_shared<pfcp::create_pdr>();

  pPdr->pdr_id.first = true;
  pPdr->pdr_id.second.rule_id = pdrId;
  pPdr->far_id.first = true;
  pPdr->far_id.second.far_id = farId;
  pPdr->outer_header_removal.first = true;
  pPdr->outer_header_removal.second.outer_header_removal_description = outerHeaderRemoval;
  pPdr->precedence.first = true;
  pPdr->precedence.second.precedence = precedence;
  pPdr->pdi.first = true;
  pPdr->pdi.second.local_fteid.first = true;
  pPdr->pdi.second.local_fteid.second.teid = teid;
  // TODO navarrothiago - Today, the management is done by control plane. The OAI was commeted.
  // Check the create method in the pfcp_session file.
  pPdr->pdi.second.local_fteid.second.ch = true;
  // pPdr->pdi.second.local_fteid.second.chid = true;
  pPdr->pdi.first = true;
  pPdr->pdi.second.source_interface.first = true;
  pPdr->pdi.second.source_interface.second.interface_value = sourceInterface;
  pPdr->pdi.second.ue_ip_address.first = true;
  pPdr->pdi.second.ue_ip_address.second.ipv4_address.s_addr = ueIPv4.s_addr;

  return pPdr;
}

std::shared_ptr<pfcp::create_far> createOaiFAR(u32 farId, apply_action_t_ actions, u32 destinationInterface, u16 outerHeadeCreation, struct in_addr destinationIPv4, u16 destinationPort)
{
  LOG_FUNC();
  std::shared_ptr<pfcp::create_far> pFar = std::make_shared<pfcp::create_far>();

  pFar->far_id.first = true;
  pFar->far_id.second.far_id = farId;
  pFar->apply_action.first = true;
  pFar->apply_action.second = {.forw = actions.forw};
  pFar->forwarding_parameters.first = true;
  pFar->forwarding_parameters.second.outer_header_creation.first = true;
  pFar->forwarding_parameters.second.outer_header_creation.second.outer_header_creation_description = outerHeadeCreation;
  pFar->forwarding_parameters.second.outer_header_creation.second.ipv4_address = destinationIPv4;
  pFar->forwarding_parameters.second.outer_header_creation.second.port_number = destinationPort;
  pFar->forwarding_parameters.second.destination_interface.first = true;
  pFar->forwarding_parameters.second.destination_interface.second.interface_value = destinationInterface;

  return pFar;
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
