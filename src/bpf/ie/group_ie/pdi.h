#if !defined(IE_PDI_H)
#define IE_PDI_H

#include <types.h>
#include <ie/ie_base.h>
#include <ie/source_interface.h>
#include <ie/fteid.h>
#include <ie/network_instance.h>
#include <ie/ue_ip_address.h>
#include <ie/traffic_endpoint_id.h>
#include <ie/sdf_filter.h>
#include <ie/application_id.h>
#include <ie/ethernet_pdu_session_information.h>
#include <ie/qfi.h>
#include <ie/framed_route.h>
#include <ie/framed_routing.h>
#include <ie/framed_ipv6_routing.h>

//------------------------------------------------------------------------------
// Table 7.5.2.2-2: PDI IE within PFCP Session Establishment Request
typedef struct pdi
{
  ie_base_t base;
  source_interface_t source_interface;
  _fteid_t fteid;
  network_instance_t network_instance;
  ue_ip_address_t ue_ip_address;
  traffic_endpoint_id_t traffic_endpoint_id;
  sdf_filter_t sdf_filter;
  application_id_t application_id;
  ethernet_pdu_session_information_t ethernet_pdu_session_information;
  qfi_t qfi;
  framed_route_t framed_route;
  framed_routing_t framed_routing;
  framed_ipv6_route_t framed_ipv6_route;

}pdi_t;

#endif // IE_PDI_H
