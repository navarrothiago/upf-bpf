#ifndef __PDR_ID_H__
#define __PDR_ID_H__

#include <types.h>
#include <ie/ie_base.h>

//-------------------------------------
// 8.2.36 Packet Detection Rule ID (PDR ID)
typedef struct pdr_id_s
{
  ie_base_t_ base;
  __u16 rule_id;
} pdr_id_t_;

#endif // __PDR_ID_H__