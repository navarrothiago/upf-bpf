#if !defined(IE_OUTER_HEADER_REMOVAL)
#define IE_OUTER_HEADER_REMOVAL

#include <ie/ie_base.h>
#include <types.h>

//-------------------------------------
// 8.2.64 Outer Header Removal
#define OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4 0
#define OUTER_HEADER_REMOVAL_GTPU_UDP_IPV6 1
#define OUTER_HEADER_REMOVAL_UDP_IPV4 2
#define OUTER_HEADER_REMOVAL_UDP_IPV6 3
typedef struct outer_header_removal_s
{
  ie_base_t ie_base;
  uint8_t outer_header_removal_description;
} outer_header_removal_t;

#endif // IE_OUTER_HEADER_REMOVAL
