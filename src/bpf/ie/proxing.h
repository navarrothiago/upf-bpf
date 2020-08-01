#ifndef __PROXING_H__
#define __PROXING_H__

#include <ie/ie_base.h>
#include <types.h>

//-------------------------------------
// 8.2.97 Proxying
typedef struct proxying_s
{
  u8 spare : 6;
  u8 ins : 1;
  u8 arp : 1;
} proxying_t;

#endif // __PROXING_H__