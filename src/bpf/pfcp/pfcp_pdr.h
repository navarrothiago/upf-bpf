#ifndef __PFCP_PDR_H__
#define __PFCP_PDR_H__

#include <types.h>
#include <ie/group_ie/create_pdr.h>

typedef struct pfcp_pdr_s
{
  u64 local_seid;
  pdr_id_t_ pdr_id;
  precedence_t_ precedence;
  pdi_t_ pdi;
  outer_header_removal_t_ outer_header_removal;
  far_id_t_ far_id;
  urr_id_t_ urr_id;
  qer_id_t_ qer_id;
  activate_predefined_rules_t_ activate_predefined_rules;
} pfcp_pdr_t_;

#endif // __PFCP_PDR_H__
