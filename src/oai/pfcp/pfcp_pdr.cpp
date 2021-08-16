/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file pfcp_pdr.cpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/
#include "common_defs.h"
#include "endian.h"
#include "pfcp_pdr.hpp"

using namespace pfcp;


//------------------------------------------------------------------------------
bool pfcp_pdr::update(const pfcp::update_pdr& update, uint8_t& cause_value) {
  if (update.get(outer_header_removal.second))
    outer_header_removal.first = true;
  if (update.get(precedence.second)) precedence.first = true;
  if (update.get(pdi.second)) pdi.first = true;
  if (update.get(far_id.second)) far_id.first = true;
  if (update.get(urr_id.second)) urr_id.first = true;
  if (update.get(qer_id.second)) qer_id.first = true;
  // TODO activate_predefined_rules
  // TODO deactivate_predefined_rules
  return true;
}

//------------------------------------------------------------------------------
void pfcp_pdr::notify_cp_requested(
    std::shared_ptr<pfcp::pfcp_session> session) {
  if (not notified_cp) {
    Logger::spgwu_sx().trace("notify_cp_requested()");
    notified_cp = true;

    pfcp::pfcp_session_report_request h;

    pfcp::report_type_t report = {};
    report.dldr                = 1;

    pfcp::downlink_data_report dl_data_report;
    dl_data_report.set(pdr_id);

    h.set(report);
    h.set(dl_data_report);

    // spgwu_sx_inst->send_sx_msg(session->cp_fseid, h);
  }
}
