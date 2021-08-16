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

/*! \file pfcp_pdr.hpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_PFCP_PDR_HPP_SEEN
#define FILE_PFCP_PDR_HPP_SEEN

#include <linux/ip.h>
#include <linux/ipv6.h>
#include "endpoint.hpp"
#include "msg_pfcp.hpp"
#include <mutex>

namespace pfcp {

class pfcp_session;

class pfcp_pdr {
 public:
  mutable std::mutex lock;
  uint64_t local_seid;
  pfcp::pdr_id_t pdr_id;
  std::pair<bool, pfcp::precedence_t> precedence;
  std::pair<bool, pfcp::pdi> pdi;
  std::pair<bool, pfcp::outer_header_removal_t> outer_header_removal;
  std::pair<bool, pfcp::far_id_t> far_id;
  std::pair<bool, pfcp::urr_id_t> urr_id;
  std::pair<bool, pfcp::qer_id_t> qer_id;
  std::pair<bool, pfcp::activate_predefined_rules_t> activate_predefined_rules;

  bool notified_cp;

  explicit pfcp_pdr(uint64_t lseid)
      : lock(),
        local_seid(lseid),
        pdr_id(),
        precedence(),
        pdi(),
        outer_header_removal(),
        far_id(),
        urr_id(),
        qer_id(),
        activate_predefined_rules(),
        notified_cp(false) {}

  explicit pfcp_pdr(const pfcp::create_pdr& c)
      : lock(),
        local_seid(0),
        pdr_id(c.pdr_id.second),
        precedence(c.precedence),
        pdi(c.pdi),
        outer_header_removal(c.outer_header_removal),
        far_id(c.far_id),
        urr_id(c.urr_id),
        qer_id(c.qer_id),
        activate_predefined_rules(c.activate_predefined_rules),
        notified_cp(false) {}

  pfcp_pdr(const pfcp_pdr& c)
      : lock(),
        precedence(c.precedence),
        pdi(c.pdi),
        outer_header_removal(c.outer_header_removal),
        far_id(c.far_id),
        urr_id(c.urr_id),
        qer_id(c.qer_id),
        activate_predefined_rules(c.activate_predefined_rules),
        notified_cp(c.notified_cp) {
    local_seid = c.local_seid;
    pdr_id     = c.pdr_id;
  }

  // virtual ~pfcp_pdr() {};
  void set(const uint64_t& v) { local_seid = v; }
  void set(const pfcp::pdr_id_t& v) { pdr_id = v; }
  void set(const pfcp::precedence_t& v) {
    precedence.first  = true;
    precedence.second = v;
  }
  void set(const pfcp::pdi& v) {
    pdi.first  = true;
    pdi.second = v;
  }
  void set(const pfcp::outer_header_removal_t& v) {
    outer_header_removal.first  = true;
    outer_header_removal.second = v;
  }
  void set(const pfcp::far_id_t& v) {
    far_id.first  = true;
    far_id.second = v;
  }
  void set(const pfcp::urr_id_t& v) {
    urr_id.first  = true;
    urr_id.second = v;
  }
  void set(const pfcp::qer_id_t& v) {
    qer_id.first  = true;
    qer_id.second = v;
  }
  void set(const pfcp::activate_predefined_rules_t& v) {
    activate_predefined_rules.first  = true;
    activate_predefined_rules.second = v;
  }

  bool get(uint64_t& v) const {
    v = local_seid;
    return true;
  }
  bool get(pfcp::pdr_id_t& v) const {
    v = pdr_id;
    return true;
  }
  bool get(pfcp::precedence_t& v) const {
    if (precedence.first) {
      v = precedence.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::pdi& v) const {
    if (pdi.first) {
      v = pdi.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::outer_header_removal_t& v) const {
    if (outer_header_removal.first) {
      v = outer_header_removal.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::far_id_t& v) const {
    if (far_id.first) {
      v = far_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::urr_id_t& v) const {
    if (urr_id.first) {
      v = urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::qer_id_t& v) const {
    if (qer_id.first) {
      v = qer_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::activate_predefined_rules_t& v) const {
    if (activate_predefined_rules.first) {
      v = activate_predefined_rules.second;
      return true;
    }
    return false;
  }

  bool update(const pfcp::update_pdr& update, uint8_t& cause_value);

  void notify_cp_requested(std::shared_ptr<pfcp::pfcp_session> session);

  // For sorting in collections
  bool operator<(const pfcp_pdr& rhs) const {
    return (precedence.second.precedence < rhs.precedence.second.precedence);
  }
};
}  // namespace pfcp

#endif
