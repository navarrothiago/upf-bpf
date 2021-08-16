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

/*! \file fqdn.hpp
 \brief
 \author
 \company Eurecom
 \email:
 */
#ifndef FILE_FQDN_HPP_SEEN
#define FILE_FQDN_HPP_SEEN
#include <string>
class fqdn {
 public:
  /*
   * Resolve a DNS name to get host's IP Addr
   * @param [const std::string &] host_name: host's name/url
   * @param [const std::string &] protocol: protocol
   * @param [uint8_t &] addr_type: addr_type (Ipv4/v6)
   * @return void
   */
  static bool resolve(
      const std::string& host_name, std::string& address, uint32_t& port,
      uint8_t& addr_type, const std::string& protocol = "http");
};

#endif /* FILE_FQDN_HPP_SEEN */
