#ifndef __NEXT_PROG_RULE_KEY_H__
#define __NEXT_PROG_RULE_KEY_H__


#include <types.h>
#include <ie/teid.h>

struct next_rule_prog_index_key {
  teid_t_ teid;
  u8 source_value;
  u32 ipv4_address;
};

#endif // __NEXT_PROG_RULE_KEY_H__
