#ifndef __DESTINATION_INTERFACE_H__
#define __DESTINATION_INTERFACE_H__

#include <ie/ie_base.h>
#include <types.h>

//-------------------------------------
// 8.2.24 Destination Interface
enum destination_interface_value_e
{
  /* Request / Initial message */
  INTERFACE_VALUE_ACCESS = 0,
  INTERFACE_VALUE_CORE = 1,
  INTERFACE_VALUE_SGI_LAN_N6_LAN = 2,
  INTERFACE_VALUE_CP_FUNCTION = 3,
  INTERFACE_VALUE_LI_FUNCTION = 4
};

typedef struct destination_interface_s
{
  ie_base_t_ base;
  u8 interface_value;
  // bool operator==(const struct destination_interface_s &i) const
  // {
  //   return (i.interface_value == interface_value);
  // };
} destination_interface_t_;

#endif // __DESTINATION_INTERFACE_H__
