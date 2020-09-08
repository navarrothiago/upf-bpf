#if !defined(IE_SDF_FILTER_H)
#define IE_SDF_FILTER_H

#include <types.h>
#include <ie/ie_base.h>

//-------------------------------------
// 8.2.5 SDF Filter
typedef struct sdf_filter
{
  ie_base_t_ base;
  u8 spare : 3;
  u8 bid : 1;
  u8 fl : 1;
  u8 spi : 1;
  u8 ttc : 1;
  u8 fd : 1;
  u16 length_of_flow_description;
  // TODO It is a string based on length_of_flow_description. How to solve this?
  u8 flow_description;
  u8 tos_traffic_class[2];        // 2 octets
  u8 security_parameter_index[4]; // 4 octets
  u8 flow_label[3];               // 3 octets
  u32 sdf_filter_id;
}sdf_filter_t_;

#endif // IE_SDF_FILTER_H
