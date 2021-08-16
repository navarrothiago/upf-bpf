/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
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

/*! \file get_gateway_netlink.h
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_IF_HPP_SEEN
#define FILE_IF_HPP_SEEN
#include <string>

int get_gateway_and_iface(std::string* gw /*OUT*/, std::string* iface /*OUT*/);
int get_inet_addr_from_iface(
    const std::string& if_name, struct in_addr& inet_addr);
int get_mtu_from_iface(const std::string& if_name, uint32_t& mtu);
int get_inet_addr_infos_from_iface(
    const std::string& if_name, struct in_addr& inet_addr,
    struct in_addr& inet_netmask, unsigned int& mtu);

#endif /* FILE_IF_HPP_SEEN */
