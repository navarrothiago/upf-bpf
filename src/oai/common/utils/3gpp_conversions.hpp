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

/*! \file 3gpp_conversions.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_3GPP_CONVERSIONS_HPP_SEEN
#define FILE_3GPP_CONVERSIONS_HPP_SEEN
#include "3gpp_29.274.h"
#include "3gpp_29.244.h"
#include "3gpp_29.281.h"
#include "endpoint.hpp"

namespace xgpp_conv {

void paa_to_pfcp_ue_ip_address(
    const paa_t& paa, pfcp::ue_ip_address_t& ue_ip_address);
void pdn_ip_to_pfcp_ue_ip_address(
    const pdn_type_t& pdn_type, const struct in_addr& ipv4_address,
    const struct in6_addr ipv6_address, pfcp::ue_ip_address_t& ue_ip_address);
void pfcp_to_core_fteid(const pfcp::fteid_t& pfteid, fteid_t& fteid);
void pfcp_from_core_fteid(pfcp::fteid_t& pfteid, const fteid_t& fteid);
void pfcp_cause_to_core_cause(const pfcp::cause_t& pc, cause_t& c);
bool endpoint_to_gtp_u_peer_address(
    const endpoint& ep, gtp_u_peer_address_t& gpa);
}  // namespace xgpp_conv

#endif /* FILE_3GPP_CONVERSIONS_HPP_SEEN */
