#if !defined(IE_TEID_H)
#define IE_TEID_H

#include <types.h>

// TEIDs
typedef u32                      teid_t;
#define TEID_FMT                 "0x%" PRIx32
#define TEID_SCAN_FMT            SCNx32
#define INVALID_TEID             ((teid_t)0x00000000)
#define UNASSIGNED_TEID          ((teid_t)0x00000000)


#endif // IE_TEID_H
