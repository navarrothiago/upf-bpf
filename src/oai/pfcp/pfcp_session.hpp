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

/*! \file pfcp_session.hpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_PFCP_SESSION_HPP_SEEN
#define FILE_PFCP_SESSION_HPP_SEEN

#include "3gpp_29.244.h"
#include "msg_pfcp.hpp"
#include "pfcp_far.hpp"
#include "pfcp_pdr.hpp"

namespace pfcp {

#define PFCP

class pfcp_session {
 private:
  void add(std::shared_ptr<pfcp::pfcp_far>);
  void add(std::shared_ptr<pfcp::pfcp_pdr>);

  bool remove(const pfcp::far_id_t& far_id, uint8_t& cause_value);
  bool remove(const pfcp::pdr_id_t& pdr_id, uint8_t& cause_value);

 public:
  pfcp::fseid_t cp_fseid;
  uint64_t seid;  // User plane
  uint8_t qfi;

  // TO DO better than this :(sooner the better)  when inserting or removing new
  // PDRs, FARS, should not conflict with switching operations
  std::vector<std::shared_ptr<pfcp::pfcp_pdr>> pdrs;
  std::vector<std::shared_ptr<pfcp::pfcp_far>> fars;

  pfcp_session() : cp_fseid(), seid(0), pdrs(), fars() {
    pdrs.reserve(8);
    fars.reserve(8);
  }
  pfcp_session(pfcp::fseid_t& cp, uint64_t up_seid) : pfcp_session() {
    cp_fseid = cp;
    seid     = up_seid;
  }

  pfcp_session(const pfcp_session& c)
      : cp_fseid(c.cp_fseid), seid(c.seid), pdrs(c.pdrs), fars(c.fars) {}

  virtual ~pfcp_session() {
    cleanup();
    cp_fseid = {};
    seid     = {};
  };

  void cleanup();
  std::string to_string() const;

  uint64_t get_up_seid() const { return seid; };
  bool get(const uint32_t, std::shared_ptr<pfcp::pfcp_far>&) const;
  bool get(const uint16_t, std::shared_ptr<pfcp::pfcp_pdr>&) const;

  bool update(const pfcp::update_far& update, uint8_t& cause_value);
  bool update(const pfcp::update_pdr& update, uint8_t& cause_value);

  bool create(
      const pfcp::create_far& cr_far, pfcp::cause_t& cause,
      uint16_t& offending_ie);
  bool create(
      const pfcp::create_pdr& cr_pdr, pfcp::cause_t& cause,
      uint16_t& offending_ie, pfcp::fteid_t& allocated_fteid);

  bool remove(
      const pfcp::remove_far& rm_far, pfcp::cause_t& cause,
      uint16_t& offending_ie);
  bool remove(
      const pfcp::remove_pdr& rm_pdr, pfcp::cause_t& cause,
      uint16_t& offending_ie);
};
}  // namespace pfcp
#endif
