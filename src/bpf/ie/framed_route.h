#if !defined(IE_FRAMED_ROUTE_H)
#define IE_FRAMED_ROUTE_H

#include <ie/ie_base.h>
#include <types.h>

//-------------------------------------
// 8.2.109 Framed-Route
typedef struct framed_route
{
  ie_base_t base;
  u8 framed_route[4];
}framed_route_t;

#endif // IE_FRAMED_ROUTING_H
