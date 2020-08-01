#ifndef __PFCP_SESSION_H__
#define __PFCP_SESSION_H__

#include <ie/fseid.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_far.h>

#define SESSION_PDRS_MAX_SIZE 10
#define SESSION_FARS_MAX_SIZE 10

typedef struct pfcp_session_s
{
  seid_t seid;
  // TODO navarrothiago - Turn to counter the iter variable.
  u32 pdrs_counter;
  pfcp_pdr_t pdrs[SESSION_PDRS_MAX_SIZE];
  u32 fars_counter;
  pfcp_far_t fars[SESSION_FARS_MAX_SIZE];

} pfcp_session_t;

#endif // __PFCP_SESSION_H__