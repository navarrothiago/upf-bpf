#ifndef __PFCP_SESSION_H__
#define __PFCP_SESSION_H__

#include <ie/fseid.h>

#define SESSION_PDRS_SIZE 10

typedef struct pfcp_session_s
{
  seid_t seid;
  u32 iter;
  pfcp_pdr_t pdrs[SESSION_PDRS_SIZE];
} pfcp_session_t;

#endif // __PFCP_SESSION_H__