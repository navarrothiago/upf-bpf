#ifndef __DUPLICATING_PARAMETERS_H__
#define __DUPLICATING_PARAMETERS_H__

#include <ie/ie_base.h>
#include <ie/destination_interface.h>
#include <ie/outer_header_creation.h>
#include <ie/transport_level_marking.h>
#include <ie/fowarding_policy.h>

//------------------------------------------------------------------------------
// Table 7.5.2.3-3: Duplicating Parameters IE in FAR
typedef struct duplicating_parameters_s
{
  ie_base_t base;
  destination_interface_t destination_interface;
  outer_header_creation_t outer_header_creation;
  transport_level_marking_t transport_level_marking;
  forwarding_policy_t forwarding_policy;
} duplicating_parameters_t;

#endif // __DUPLICATING_PARAMETERS_H__