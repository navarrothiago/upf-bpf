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
  ie_base_t base;
  pdr_id_t pdr_id;
  precedence_t precedence;
  pdi_t pdi;
  outer_header_removal_t outer_header_removal;
  far_id_t far_id;
  urr_id_t urr_id;
  qer_id_t qer_id;
  activate_predefined_rules_t activate_predefined_rules;
} create_pdr_t;

#endif // CREATE_PDR_H
