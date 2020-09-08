#if !defined(IE_IE_BASE_H)
#define IE_IE_BASE_H

#include <types.h>

#define GTPV1U_MSG_HEADER_MIN_SIZE        8
typedef struct ie_base
{
  u16 type;
  u16 length;
}ie_base_t_;

#endif // IE_IE_BASE_H
