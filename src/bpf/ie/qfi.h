#if !defined(IE_QFI_H)
#define IE_QFI_H

#include <types.h>
#include <ie/ie_base.h>
//-------------------------------------
// 8.2.89 QFI
typedef struct qfi
{
  ie_base_t base;
  u8 spare : 2;
  u8 qfi : 6;
}qfi_t;

#endif // IE_QFI_H
