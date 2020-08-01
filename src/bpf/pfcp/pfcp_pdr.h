#ifndef __PFCP_PDR_H__
#define __PFCP_PDR_H__

#include <types.h>
#include <ie/group_ie/create_pdr.h>

typedef struct pfcp_pdr_s
{
  u64 local_seid;
  teid_t teid;
  pdr_id_t pdr_id;
  precedence_t precedence;
  pdi_t pdi;
  outer_header_removal_t outer_header_removal;
  far_id_t far_id;
  urr_id_t urr_id;
  qer_id_t qer_id;
  activate_predefined_rules_t activate_predefined_rules;
} pfcp_pdr_t;

#endif // __PFCP_PDR_H__