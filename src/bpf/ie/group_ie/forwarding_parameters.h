#ifndef __FORWARDING_PARAMETERS_H__
#define __FORWARDING_PARAMETERS_H__

#include <ie/ie_base.h>
#include <ie/destination_interface.h>
#include <ie/network_instance.h>
#include <ie/redirect_information.h>
#include <ie/outer_header_creation.h>
#include <ie/transport_level_marking.h>
#include <ie/fowarding_policy.h>
#include <ie/header_enrichment.h>
#include <ie/traffic_endpoint_id.h>
#include <ie/proxing.h>

//-----------------------------------------------------------------------------
// Table 7.5.2.3-2: Forwarding Parameters IE in FAR
typedef struct forwarding_parameters_s
{
  ie_base_t_ base;
  destination_interface_t_ destination_interface;
  redirect_information_t_ redirect_information;
  outer_header_creation_t_ outer_header_creation; 
  transport_level_marking_t_ transport_level_marking;
  forwarding_policy_t_ forwarding_policy;
  header_enrichment_t_ header_enrichment;
  traffic_endpoint_id_t_ traffic_endpoint_id;
  proxying_t_ proxying;
} forwarding_parameters_t_;

#endif // __FORWARDING_PARAMETERS_H__