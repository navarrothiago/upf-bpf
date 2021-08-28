#ifndef __APPLY_ACTION_H__
#define __APPLY_ACTION_H__

#include <types.h>
#include <ie/ie_base.h>

//-------------------------------------
// 8.2.26 Apply Action
typedef struct apply_action_s
{
  // ie_base_t_ base;
  u8 spare : 3;
  u8 dupl : 1;
  u8 nocp : 1;
  u8 buff : 1;
  u8 forw : 1;
  u8 drop : 1;
} apply_action_t_;

#endif // __APPLY_ACTION_H__
