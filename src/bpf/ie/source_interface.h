#if !defined(IE_SOURCE_INTERFACE_H)
#define IE_SOURCE_INTERFACE_H

#include <types.h>
#include <ie/ie_base.h>

//-------------------------------------
// 8.2.2 Source Interface
// FIXME navarrothiago - conflit with destination_interface
// enum source_interface_value_e
// {
//   /* Request / Initial message */
//   INTERFACE_VALUE_ACCESS = 0,
//   INTERFACE_VALUE_CORE = 1,
//   INTERFACE_VALUE_SGI_LAN_N6_LAN = 2,
//   INTERFACE_VALUE_CP_FUNCTION = 3,
//   INTERFACE_VALUE_5G_VN_INTERNAL = 4
// };

typedef struct source_interface
{
  ie_base_t_ base;
  u8 interface_value;
} source_interface_t_;

#endif // IE_SOURCE_INTERFACE_H
