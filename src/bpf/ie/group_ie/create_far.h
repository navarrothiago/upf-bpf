#ifndef __CREATE_FAR_H__
#define __CREATE_FAR_H__

#include <ie/far_id.h>
#include <ie/apply_action.h>
#include <ie/group_ie/forwarding_parameters.h>
#include <ie/group_ie/duplicating_parameters.h>
#include <ie/bar_id.h>

//------------------------------------------------------------------------------
// Table 7.5.2.3-1: Create FAR IE within PFCP Session Establishment Request
typedef struct create_far_s
{
  far_id_t far_id;
  apply_action_t apply_action;
  forwarding_parameters_t forwarding_parameters;
  duplicating_parameters_t duplicating_parameters;
  bar_id_t bar_id;
} create_far_t;


#endif // __CREATE_FAR_H__