#if !defined(IE_FRAMED_IPV6_ROUTING_H)
#define IE_FRAMED_IPV6_ROUTING_H

#include <ie/ie_base.h>
#include <types.h>

//-------------------------------------
// 8.2.111 Framed-IPv6-Route
typedef struct framed_ipv6_route
{
  ie_base_t base;
  // TODO undefined value
  u8 framed_ipv6_route[20];
}framed_ipv6_route_t;

#endif // IE_FRAMED_IPV6_ROUTING_H
