#ifndef __PFCP_PDR_H__
#define __PFCP_PDR_H__

#include <types.h>
#include <ie/group_ie/create_pdr.h>

typedef struct pfcp_pdr_s
{
  u64 local_seid;
  teid_t teid;
  create_pdr_t pdr;
}pfcp_pdr_t;


#endif // __PFCP_PDR_H__