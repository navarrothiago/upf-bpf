#ifndef __URR_ID_H__
#define __URR_ID_H__

#include <types.h>
#include <ie/ie_base.h>

//-------------------------------------
// 8.2.54 URR ID
typedef struct urr_id_s
{
  ie_base_t_ base;
  u32 urr_id;
} urr_id_t_;

#endif // __URR_ID_H__