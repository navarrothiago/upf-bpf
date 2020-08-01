#ifndef __TRANSPORT_LEVEL_MARKING_H__
#define __TRANSPORT_LEVEL_MARKING_H__

#include <ie/ie_base.h>
#include <types.h>

//-------------------------------------
// 8.2.12 Transport Level Marking
typedef struct transport_level_marking_s
{
  ie_base_t base;
  s8 transport_level_marking[2]; // 2 octets
} transport_level_marking_t;

#endif // __TRANSPORT_LEVEL_MARKING_H__