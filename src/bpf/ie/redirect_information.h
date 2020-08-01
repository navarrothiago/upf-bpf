#ifndef __REDIRECT_INFORMATION_H__
#define __REDIRECT_INFORMATION_H__

#include <ie/ie_base.h>
#include <types.h>

//-------------------------------------
// 8.2.20 Redirect Information
typedef struct redirect_information_s
{
  ie_base_t base;
  u8 redirect_address_type : 4;
  u8 spare : 4;
  u16 redirect_server_address_length;
} redirect_information_t;

#endif // __REDIRECT_INFORMATION_H__