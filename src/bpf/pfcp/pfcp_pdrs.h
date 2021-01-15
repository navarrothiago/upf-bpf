#ifndef __PFCP_PDRS_H__
#define __PFCP_PDRS_H__

#include <types.h>
#include <pfcp/pfcp_pdr.h>

#define PDRS_MAX_SIZE 10

typedef struct pfcp_pdrs_s
{
  u32 pdrs_counter;
  pfcp_pdr_t_ pdrs[PDRS_MAX_SIZE];

} pfcp_pdrs_t_;



#endif // __PFCP_PDRS_H__
