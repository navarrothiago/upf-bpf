#ifndef __FSEID_H__
#define __FSEID_H__

#include <ie/ie_base.h>
#include <types.h>

#ifdef KERNEL_SPACE
#include <linux/in.h>
#include <linux/in6.h>
#else
#include <netinet/in.h>
#endif

#ifdef __cplusplus
#include <cstdint> // include this header for uint64_t
#endif

typedef u64 seid_t;

//-------------------------------------
// 8.2.37 F-SEID
typedef struct fseid_s
{
  u8 spare : 6;
  u8 v4 : 1;
  u8 v6 : 1;
  u64 seid;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;

  #ifdef __cplusplus
  bool operator==(const struct fseid_s &i) const
  {
    if (
        (i.seid == this->seid) && (i.v4 == this->v4) && (i.ipv4_address.s_addr == this->ipv4_address.s_addr) && (i.v6 == this->v6) &&
        (i.ipv6_address.s6_addr32[0] == this->ipv6_address.s6_addr32[0]) &&
        (i.ipv6_address.s6_addr32[1] == this->ipv6_address.s6_addr32[1]) &&
        (i.ipv6_address.s6_addr32[2] == this->ipv6_address.s6_addr32[2]) &&
        (i.ipv6_address.s6_addr32[3] == this->ipv6_address.s6_addr32[3]))
    {
      return true;
    }
    else
    {
      return false;
    }
  };
  bool operator<(const struct fseid_s &i) const
  {
    if (i.seid < this->seid)
      return true;
    else if (i.seid > this->seid)
      return false;
    if (i.v4 == this->v4)
    {
      if (i.ipv4_address.s_addr > this->ipv4_address.s_addr)
        return true;
      else if (i.ipv4_address.s_addr < this->ipv4_address.s_addr)
        return false;
    }
    else if (this->v4)
      return true;
    if (i.v6 == this->v6)
    {
      uint64_t i64 = ((uint64_t)i.ipv6_address.s6_addr32[0] << 32) |
                     ((uint64_t)i.ipv6_address.s6_addr32[1]);
      uint64_t this64 = ((uint64_t)this->ipv6_address.s6_addr32[0] << 32) |
                        ((uint64_t)this->ipv6_address.s6_addr32[1]);

      if (i64 < this64)
        return true;
      else if (i64 > this64)
        return false;
      i64 = ((uint64_t)i.ipv6_address.s6_addr32[2] << 32) |
            ((uint64_t)i.ipv6_address.s6_addr32[3]);
      this64 = ((uint64_t)this->ipv6_address.s6_addr32[2] << 32) |
               ((uint64_t)this->ipv6_address.s6_addr32[3]);
      if (i64 < this64)
        return true;
      else if (i64 > this64)
        return false;
    }
    else if (this->v6)
      return true;
    return false;
  };
  #endif
} fseid_t;

#endif // __FSEID_H__
