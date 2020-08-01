#ifndef __HEADER_ENRICHMENT_H__
#define __HEADER_ENRICHMENT_H__

#include <ie/ie_base.h>
#include <types.h>

#define HEADER_ENRICHMENT_FIELD_NAME_MAX_SIZE 100
#define HEADER_ENRICHMENT_FIELD_VALUE_MAX_SIZE 100

//-------------------------------------
// 8.2.67 Header Enrichment
typedef struct header_enrichment_s
{
  u8 spare : 3;
  u8 header_type : 5;
  u8 lenght_of_header_field_name;
  s8 header_field_name[HEADER_ENRICHMENT_FIELD_NAME_MAX_SIZE];
  u8 lenght_of_header_field_value;
  s8 header_field_value[HEADER_ENRICHMENT_FIELD_VALUE_MAX_SIZE];
} header_enrichment_t;

#endif // __HEADER_ENRICHMENT_H__