#ifndef __ACTIVATE_PREDEFINED_RULES_H__
#define __ACTIVATE_PREDEFINED_RULES_H__

#include <types.h>
#include <ie/ie_base.h>

//-------------------------------------
// 8.2.72 Activate Predefined Rules
typedef struct activate_predefined_rules_s
{
  ie_base_t_ base;
  // TODO Undefined string
  u8 predefined_rules_name[10];
} activate_predefined_rules_t_;

#endif // __ACTIVATE_PREDEFINED_RULES_H__
