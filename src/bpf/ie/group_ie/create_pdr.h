#if !defined(CREATE_PDR_H)
#define CREATE_PDR_H

#include <ie/ie_base.h>
#include <ie/pdr_id.h>
#include <ie/precedence.h>
#include <ie/outer_header_removal.h>
#include <ie/far_id.h>
#include <ie/urr_id.h>
#include <ie/qer_id.h>
#include <ie/qer_id.h>
#include <ie/activate_predefined_rules.h>
#include <ie/group_ie/pdi.h>

//------------------------------------------------------------------------------
// Table 7.5.2.2-1: Create PDR IE within PFCP Session Establishment Request
typedef struct create_pdr_s
{
  ie_base_t_ base;
  pdr_id_t_ pdr_id;
  precedence_t_ precedence;
  pdi_t_ pdi;
  outer_header_removal_t_ outer_header_removal;
  far_id_t_ far_id;
  urr_id_t_ urr_id;
  qer_id_t_ qer_id;
  activate_predefined_rules_t_ activate_predefined_rules;
} create_pdr_t_;

#endif // CREATE_PDR_H
