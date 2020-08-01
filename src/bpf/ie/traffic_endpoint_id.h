#if !defined(IE_TRAFFIC_ENDPOINT_H)
#define IE_TRAFFIC_ENDPOINT_H

#include <types.h>
#include <ie/ie_base.h>

//-------------------------------------
// 8.2.92 Traffic Endpoint ID
typedef struct traffic_endpoint_id
{
  ie_base_t base;
  u8 traffic_endpoint_id;
}traffic_endpoint_id_t;

#endif // IE_TRAFFIC_ENDPOINT_H
