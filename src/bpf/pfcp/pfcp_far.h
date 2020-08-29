#ifndef __PFCP_FAR_H__
#define __PFCP_FAR_H__

#include <types.h>
#include <ie/apply_action.h>
#include <ie/group_ie/forwarding_parameters.h>
#include <ie/group_ie/duplicating_parameters.h>
#include <ie/bar_id.h>
#include <ie/far_id.h>

typedef struct pfcp_far_s
{
  far_id_t far_id;
  apply_action_t apply_action;
  forwarding_parameters_t forwarding_parameters;
  duplicating_parameters_t duplicating_parameters;
  bar_id_t bar_id;

} pfcp_far_t;
#endif // __PFCP_FAR_H__
