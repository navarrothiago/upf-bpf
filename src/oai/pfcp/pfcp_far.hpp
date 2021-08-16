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

/*! \file pfcp_far.hpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_PFCP_FAR_HPP_SEEN
#define FILE_PFCP_FAR_HPP_SEEN

#include <linux/ip.h>
#include <linux/ipv6.h>
#include "msg_pfcp.hpp"

namespace pfcp {

class pfcp_far {
 public:
  pfcp::far_id_t far_id;
  pfcp::apply_action_t apply_action;
  std::pair<bool, pfcp::forwarding_parameters> forwarding_parameters;
  std::pair<bool, pfcp::duplicating_parameters> duplicating_parameters;
  std::pair<bool, pfcp::bar_id_t> bar_id;

  pfcp_far()
      : far_id(),
        apply_action(),
        forwarding_parameters(),
        duplicating_parameters(),
        bar_id() {}

  explicit pfcp_far(const pfcp::create_far& c)
      : forwarding_parameters(c.forwarding_parameters),
        duplicating_parameters(c.duplicating_parameters),
        bar_id(c.bar_id) {
    far_id       = c.far_id.second;
    apply_action = c.apply_action.second;
  }

  pfcp_far(const pfcp_far& c)
      : far_id(c.far_id),
        apply_action(c.apply_action),
        forwarding_parameters(c.forwarding_parameters),
        duplicating_parameters(c.duplicating_parameters),
        bar_id(c.bar_id) {}

  // virtual ~pfcp_far() {};
  void set(const pfcp::far_id_t& v) { far_id = v; }
  void set(const pfcp::apply_action_t& v) { apply_action = v; }
  void set(const pfcp::forwarding_parameters& v) {
    forwarding_parameters.first  = true;
    forwarding_parameters.second = v;
  }
  void set(const pfcp::duplicating_parameters& v) {
    duplicating_parameters.first  = true;
    duplicating_parameters.second = v;
  }
  void set(const pfcp::bar_id_t& v) {
    bar_id.first  = true;
    bar_id.second = v;
  }

  bool get(pfcp::far_id_t& v) const {
    v = far_id;
    return true;
  }
  bool get(pfcp::apply_action_t& v) const {
    v = apply_action;
    return true;
  }
  bool get(pfcp::forwarding_parameters& v) const {
    if (forwarding_parameters.first) {
      v = forwarding_parameters.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::duplicating_parameters& v) const {
    if (duplicating_parameters.first) {
      v = duplicating_parameters.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::bar_id_t& v) const {
    if (bar_id.first) {
      v = bar_id.second;
      return true;
    }
    return false;
  }

  bool update(const pfcp::update_far& update, uint8_t& cause_value);
};
}  // namespace pfcp

#endif
