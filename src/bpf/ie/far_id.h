#ifndef __FAR_ID_H__
#define __FAR_ID_H__

#include <types.h>
#include <ie/ie_base.h>

//-------------------------------------
// 8.2.74 FAR ID
typedef struct far_id_s
{
  ie_base_t base;
  u32 far_id;
} far_id_t;

#endif // __FAR_ID_H__