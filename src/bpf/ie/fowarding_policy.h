#ifndef __FOWARDING_POLICY_H__
#define __FOWARDING_POLICY_H__

#include <ie/ie_base.h>
#include <types.h>

#define FOWARDING_POLICY_ID_MAX_SIZE 100
//-------------------------------------
// 8.2.23 Forwarding Policy
typedef struct forwarding_policy_s
{
  u8 forwarding_policy_identifier_length;
  s8 forwarding_policy_identifier[FOWARDING_POLICY_ID_MAX_SIZE]; // TODO CHECK TYPE
} forwarding_policy_t_;
#endif // __FOWARDING_POLICY_H__