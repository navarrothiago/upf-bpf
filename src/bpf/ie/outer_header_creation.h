#ifndef __OUTER_HEADER_CREATION_H__
#define __OUTER_HEADER_CREATION_H__

#include <ie/ie_base.h>
#include <linux/in6.h>
#include <types.h>

#ifdef KERNEL_SPACE
#include <linux/in.h>
#else
#include <netinet/in.h>
#endif

enum outer_header_creation_description_value_e {
  OUTER_HEADER_CREATION_GTPU_UDP_IPV4 = 0x0100,
  OUTER_HEADER_CREATION_GTPU_UDP_IPV6 = 0x0200,
  OUTER_HEADER_CREATION_UDP_IPV4 = 0x0400,
  OUTER_HEADER_CREATION_UDP_IPV6 = 0x0800
};

//-------------------------------------
// 8.2.56 Outer Header Creation
typedef struct outer_header_creation_s {
  ie_base_t base;
  u16 outer_header_creation_description;
  u32 teid;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  u16 port_number;
} outer_header_creation_t;

#endif // __OUTER_HEADER_CREATION_H__