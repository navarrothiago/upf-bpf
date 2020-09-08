#if !defined(IE_SOURCE_INTERFACE_H)
#define IE_SOURCE_INTERFACE_H

#include <types.h>
#include <ie/ie_base.h>

//-------------------------------------
// 8.2.2 Source Interface
typedef struct source_interface
{
  ie_base_t_ base;
  u8 interface_value;
} source_interface_t_;

#endif // IE_SOURCE_INTERFACE_H
