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

/*! \file pfcp_session.cpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/

#include "pfcp_session.hpp"
#include "logger.hpp"

using namespace pfcp;


//------------------------------------------------------------------------------
bool pfcp_session::get(
    const uint32_t far_id, std::shared_ptr<pfcp::pfcp_far>& far) const {
  for (auto it : fars) {
    if (it->far_id.far_id == far_id) {
      far = it;
      return true;
    }
  }
  return false;
}
//------------------------------------------------------------------------------
bool pfcp_session::get(
    const uint16_t pdr_id, std::shared_ptr<pfcp::pfcp_pdr>& pdr) const {
  for (auto it : pdrs) {
    if (it->pdr_id.rule_id == pdr_id) {
      pdr = it;
      return true;
    }
  }
  return false;
}
//------------------------------------------------------------------------------
void pfcp_session::add(std::shared_ptr<pfcp::pfcp_far> far) {
  Logger::spgwu_sx().info("pfcp_session::add(far) seid " SEID_FMT " ", seid);
  fars.push_back(far);
}
//------------------------------------------------------------------------------
void pfcp_session::add(std::shared_ptr<pfcp::pfcp_pdr> pdr) {
  Logger::spgwu_sx().info("pfcp_session::add(pdr) seid " SEID_FMT " ", seid);
  pdrs.push_back(pdr);
}
//------------------------------------------------------------------------------
bool pfcp_session::remove(const pfcp::far_id_t& far_id, uint8_t& cause_value) {
  for (std::vector<std::shared_ptr<pfcp::pfcp_far>>::iterator it = fars.begin();
       it != fars.end(); ++it) {
    if ((*it)->far_id.far_id == far_id.far_id) {
      Logger::spgwu_sx().info(
          "pfcp_session::remove(far) seid " SEID_FMT " ", seid);
      fars.erase(it);
      return true;
    }
  }
  cause_value = pfcp::CAUSE_VALUE_RULE_CREATION_MODIFICATION_FAILURE;  //??
  return false;
}
//------------------------------------------------------------------------------
bool pfcp_session::remove(const pfcp::pdr_id_t& pdr_id, uint8_t& cause_value) {
  for (std::vector<std::shared_ptr<pfcp::pfcp_pdr>>::iterator it = pdrs.begin();
       it != pdrs.end(); ++it) {
    if ((*it)->pdr_id.rule_id == pdr_id.rule_id) {
      Logger::spgwu_sx().info(
          "pfcp_session::remove(pdr) seid " SEID_FMT " ", seid);
      pdrs.erase(it);
      return true;
    }
  }
  cause_value = pfcp::CAUSE_VALUE_RULE_CREATION_MODIFICATION_FAILURE;  //??
  return false;
}
//------------------------------------------------------------------------------
bool pfcp_session::update(
    const pfcp::update_far& update, uint8_t& cause_value) {
  std::shared_ptr<pfcp::pfcp_far> far = {};
  if (get(update.far_id.far_id, far)) {
    if (far->update(update, cause_value)) {
      return true;
    }
    return false;
  }
  cause_value = pfcp::CAUSE_VALUE_RULE_CREATION_MODIFICATION_FAILURE;
  return false;
}
//------------------------------------------------------------------------------
bool pfcp_session::update(
    const pfcp::update_pdr& update, uint8_t& cause_value) {
  std::shared_ptr<pfcp::pfcp_pdr> pdr = {};
  if (get(update.pdr_id.rule_id, pdr)) {
    if (pdr->update(update, cause_value)) {
      return true;
    }
    return false;
  }
  cause_value = pfcp::CAUSE_VALUE_RULE_CREATION_MODIFICATION_FAILURE;
  return false;
}
//------------------------------------------------------------------------------
bool pfcp_session::create(
    const pfcp::create_far& cr_far, pfcp::cause_t& cause,
    uint16_t& offending_ie) {
  if (not cr_far.far_id.first) {
    // should be caught in lower layer
    cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
    offending_ie      = PFCP_IE_FAR_ID;
    return false;
  }
  if (not cr_far.apply_action.first) {
    // should be caught in lower layer
    cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
    offending_ie      = PFCP_IE_APPLY_ACTION;
    return false;
  }
  if (cr_far.apply_action.second.forw) {
    if (not cr_far.forwarding_parameters.first) {
      // should be caught in lower layer
      cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
      offending_ie      = PFCP_IE_FORWARDING_PARAMETERS;
      return false;
    }
  }
  if (cr_far.apply_action.second.dupl) {
    if (not cr_far.duplicating_parameters.first) {
      // should be caught in lower layer
      cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
      offending_ie      = PFCP_IE_DUPLICATING_PARAMETERS;
      return false;
    }
  }
  pfcp_far* far                  = new pfcp_far(cr_far);
  std::shared_ptr<pfcp_far> sfar = std::shared_ptr<pfcp_far>(far);
  add(sfar);
  return true;
}
//------------------------------------------------------------------------------
bool pfcp_session::create(
    const pfcp::create_pdr& cr_pdr, pfcp::cause_t& cause,
    uint16_t& offending_ie, pfcp::fteid_t& allocated_fteid) {
  if (not cr_pdr.pdr_id.first) {
    // should be caught in lower layer
    cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
    offending_ie      = PFCP_IE_PACKET_DETECTION_RULE_ID;
    return false;
  }
  if (not cr_pdr.pdi.first) {
    // should be caught in lower layer
    cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
    offending_ie      = PFCP_IE_PDI;
    return false;
  }
  if (not cr_pdr.precedence.first) {
    // should be caught in lower layer
    cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
    offending_ie      = PFCP_IE_PRECEDENCE;
    return false;
  }
  const pdi& pdi = cr_pdr.pdi.second;
  if (not pdi.source_interface.first) {
    // should be caught in lower layer
    cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
    offending_ie      = PFCP_IE_SOURCE_INTERFACE;
    return false;
  }
  // already checked but !!! keep this code in comment !!!
  //  pfcp::far_id_t    far_id = {};
  //  if (not cr_pdr.get(far_id)) {
  //    //should be caught in lower layer
  //    cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
  //    offending_ie = PFCP_IE_FAR_ID;
  //    return false;
  //  }

  if (pdi.traffic_endpoint_id.first) {
    cause.cause_value = CAUSE_VALUE_REQUEST_REJECTED;
    Logger::spgwu_sx().info("Do not support IE traffic_endpoint_id yet!");
    return false;
  }

  // source interface of the incoming packet
  if (pdi.source_interface.second.interface_value == INTERFACE_VALUE_ACCESS) {
    // Uplink traffic
    if (not pdi.local_fteid.first) {
      cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
      offending_ie      = PFCP_IE_F_TEID;
      return false;
    }
    const pfcp::fteid_t& local_fteid = pdi.local_fteid.second;
    // allocated_fteid                  = {};
    // if (local_fteid.ch) {
      // TODO if (local_fteid.choose_id) {
      // allocated_fteid = pfcp_switch_inst->generate_fteid_s1u();
    // } else {
    //   cause.cause_value = CAUSE_VALUE_REQUEST_REJECTED;
    //   Logger::spgwu_sx().info(
    //       "Do not support IE FTEID managed by CP entity! Rejecting "
    //       "PFCP_XXX_REQUEST");
    //   return false;
    // }
    pfcp_pdr* pdr = new pfcp_pdr(cr_pdr);
    // if (local_fteid.ch) {
    //   pdr->pdi.second.set(allocated_fteid);
    // }

    std::shared_ptr<pfcp_pdr> spdr = std::shared_ptr<pfcp_pdr>(pdr);
    // if (pfcp_switch_inst->create_packet_in_access(
            // spdr, allocated_fteid, cause.cause_value)) {
      pdr->set(get_up_seid());
      add(spdr);
    // } else {
    //   cause.cause_value = CAUSE_VALUE_REQUEST_REJECTED;
    //   Logger::spgwu_sx().info(
    //       "Could not create_packet_in_access ! Rejecting "
    //       "PFCP_SESSION_ESTABLISHMENT_REQUEST");
    //   return false;
    // }
  } else if (
      pdi.source_interface.second.interface_value == INTERFACE_VALUE_CORE) {
    pfcp_pdr* pdr                  = new pfcp_pdr(cr_pdr);
    std::shared_ptr<pfcp_pdr> spdr = std::shared_ptr<pfcp_pdr>(pdr);
    pdr->set(get_up_seid());
    // if ((pdi.ue_ip_address.first) && (pdi.ue_ip_address.second.v4)) {
    //   pfcp_switch_inst->add_pfcp_dl_pdr_by_ue_ip(
    //       be32toh(pdi.ue_ip_address.second.ipv4_address.s_addr), spdr);
    // } else {
    //   cause.cause_value = CAUSE_VALUE_REQUEST_REJECTED;
    //   Logger::spgwu_sx().info(
    //       "Could not create_packet_in_access, cause accept only IPv4 UE IP "
    //       "address! Rejecting PFCP_XXX_REQUEST");
    //   return false;
    // }
    add(spdr);
  } else {
    cause.cause_value = CAUSE_VALUE_REQUEST_REJECTED;
    Logger::spgwu_sx().info(
        "Do not actually support other interface type value as ACCESS and CORE "
        "in PFCP_XXX_REQUEST! Rejecting PFCP_XXX_REQUEST");
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------
bool pfcp_session::remove(
    const pfcp::remove_far& rm_far, pfcp::cause_t& cause,
    uint16_t& offending_ie) {
  if (not rm_far.far_id.first) {
    // should be caught in lower layer
    cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
    offending_ie      = PFCP_IE_FAR_ID;
    return false;
  }
  return remove(rm_far.far_id.second, cause.cause_value);
}
//------------------------------------------------------------------------------
bool pfcp_session::remove(
    const pfcp::remove_pdr& rm_pdr, pfcp::cause_t& cause,
    uint16_t& offending_ie) {
  if (not rm_pdr.pdr_id.first) {
    // should be caught in lower layer
    cause.cause_value = CAUSE_VALUE_MANDATORY_IE_MISSING;
    offending_ie      = PFCP_IE_PACKET_DETECTION_RULE_ID;
    return false;
  }
  return remove(rm_pdr.pdr_id.second, cause.cause_value);
}
//------------------------------------------------------------------------------
void pfcp_session::cleanup() {
  for (std::vector<std::shared_ptr<pfcp::pfcp_pdr>>::iterator it = pdrs.begin();
       it != pdrs.end(); ++it) {
    if (((*it)->pdi.first) && ((*it)->pdi.second.source_interface.first)) {
      if ((*it)->pdi.second.source_interface.second.interface_value ==
          INTERFACE_VALUE_ACCESS) {
        // if ((*it)->pdi.second.local_fteid.first) {
        //   pfcp_switch_inst->remove_pfcp_ul_pdrs_by_up_teid(
        //       (*it)->pdi.second.local_fteid.second.teid);
        // }
      } else if (
          (*it)->pdi.second.source_interface.second.interface_value ==
          INTERFACE_VALUE_CORE) {
        if (((*it)->pdi.second.ue_ip_address.first) &&
            ((*it)->pdi.second.ue_ip_address.second.v4)) {
          // pfcp_switch_inst->remove_pfcp_dl_pdrs_by_ue_ip(be32toh(
          //     (*it)->pdi.second.ue_ip_address.second.ipv4_address.s_addr));
        }
      }
    }
  }
  fars.clear();
  pdrs.clear();
}

//------------------------------------------------------------------------------
std::string pfcp_session::to_string() const {
  std::string s = {};
  for (std::vector<std::shared_ptr<pfcp::pfcp_pdr>>::const_iterator it_pdr =
           pdrs.begin();
       it_pdr != pdrs.end(); ++it_pdr) {
    s.append(fmt::format("|{:016x}", seid));  // TODO continue this line
    std::shared_ptr<pfcp::pfcp_pdr> pdr = *it_pdr;
    std::shared_ptr<pfcp::pfcp_far> far = {};

    s.append(fmt::format("|{:04x}", pdr->pdr_id.rule_id));
    s.append(fmt::format("|{:08x}", pdr->far_id.second.far_id));
    if (pdr->precedence.first) {
      std::string f = fmt::format("|{:08x}", pdr->precedence.second.precedence);
      s.append(f);
    } else {
      s.append("|no prece");
    }
    if (pdr->pdi.first) {
      if (pdr->pdi.second.source_interface.first) {
        switch (pdr->pdi.second.source_interface.second.interface_value) {
          case pfcp::INTERFACE_VALUE_ACCESS:
            s.append("|ACC>");
            break;
          case pfcp::INTERFACE_VALUE_CORE:
            s.append("|COR>");
            break;
          case pfcp::INTERFACE_VALUE_SGI_LAN_N6_LAN:
            s.append("|LAN>");
            break;
          case pfcp::INTERFACE_VALUE_CP_FUNCTION:
            s.append("|CPF>");
            break;
          case pfcp::INTERFACE_VALUE_LI_FUNCTION:
            s.append("|LIF>");
            break;
          default:
            s.append("| ? >");
        }
      } else {
        s.append("| ? >");
      }
    } else {
      s.append("| ? >");
    }

    if ((pdr->far_id.first) && (get(pdr->far_id.second.far_id, far))) {
      char c = '-';
      if (far->apply_action.dupl) {
        c = '=';
      }
      s.append(1, c);
      if (far->apply_action.nocp) {
        s.append("N");
      } else {
        s.append(1, c);
      }
      if (far->apply_action.buff) {
        s.append("B");
      } else {
        s.append(1, c);
      }
      if (far->apply_action.drop) {
        s.append("X");
      } else {
        s.append(1, c);
      }
      if (far->apply_action.forw) {
        if ((far->forwarding_parameters.first) &&
            (far->forwarding_parameters.second.destination_interface.first)) {
          switch (far->forwarding_parameters.second.destination_interface.second
                      .interface_value) {
            case pfcp::INTERFACE_VALUE_ACCESS:
              s.append(">ACC");
              break;
            case pfcp::INTERFACE_VALUE_CORE:
              s.append(">COR");
              break;
            case pfcp::INTERFACE_VALUE_SGI_LAN_N6_LAN:
              s.append(">LAN");
              break;
            case pfcp::INTERFACE_VALUE_CP_FUNCTION:
              s.append(">CPF");
              break;
            case pfcp::INTERFACE_VALUE_LI_FUNCTION:
              s.append(">LIF");
              break;
            default:
              s.append("> ? ");
          }
        }
      } else {
        s.append("> ? ");
      }
      if ((far->forwarding_parameters.first) &&
          (far->forwarding_parameters.second.outer_header_creation.first)) {
        switch (far->forwarding_parameters.second.outer_header_creation.second
                    .outer_header_creation_description) {
          case pfcp::OUTER_HEADER_CREATION_GTPU_UDP_IPV4: {
            s.append("|GTPU_UDP_IPV4:");
            std::string ip =
                conv::toString(far->forwarding_parameters.second
                                   .outer_header_creation.second.ipv4_address);
            ip.resize(INET_ADDRSTRLEN, ' ');
            s.append(ip);
            s.append(fmt::format(
                ":{:08x}", far->forwarding_parameters.second
                               .outer_header_creation.second.teid));
          } break;
          case pfcp::OUTER_HEADER_CREATION_GTPU_UDP_IPV6: {
            s.append("|GTPU_UDP_IPV6:");
            std::string ip =
                conv::toString(far->forwarding_parameters.second
                                   .outer_header_creation.second.ipv6_address);
            ip.resize(INET_ADDRSTRLEN, ' ');
            s.append(fmt::format(
                ":{:08x}", far->forwarding_parameters.second
                               .outer_header_creation.second.teid));
          } break;
          case pfcp::OUTER_HEADER_CREATION_UDP_IPV4: {
            s.append("|UDP_IPV4     :");
            std::string ip =
                conv::toString(far->forwarding_parameters.second
                                   .outer_header_creation.second.ipv4_address);
            ip.resize(INET_ADDRSTRLEN, ' ');
            s.append(ip);
            s.append(9, ' ');
          } break;
          case pfcp::OUTER_HEADER_CREATION_UDP_IPV6: {
            s.append("|UDP_IPV6     :");
            std::string ip =
                conv::toString(far->forwarding_parameters.second
                                   .outer_header_creation.second.ipv6_address);
            ip.resize(INET_ADDRSTRLEN, ' ');
            s.append(ip);
            s.append(9, ' ');
          } break;
          default:
            s.append("|BAD_VALUE    ");
            std::string ip = {};
            ip.resize(INET_ADDRSTRLEN, ' ');
            s.append(ip);
            s.append(9, ' ');
        }
      } else {
        s.append("|none          ");
        std::string ip = {};
        ip.resize(INET_ADDRSTRLEN, ' ');
        s.append(ip);
        s.append(9, ' ');
      }
    }
    if (pdr->outer_header_removal.first) {
      switch (
          pdr->outer_header_removal.second.outer_header_removal_description) {
        case OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4: {
          s.append("|GTPU_UDP_IPV4");
          s.append(
              fmt::format(":{:08x}", pdr->pdi.second.local_fteid.second.teid));
        } break;
        case OUTER_HEADER_REMOVAL_GTPU_UDP_IPV6: {
          s.append("|GTPU_UDP_IPV6");
          s.append(
              fmt::format(":{:08x}", pdr->pdi.second.local_fteid.second.teid));
        } break;
        case OUTER_HEADER_REMOVAL_UDP_IPV4: {
          s.append("|UDP_IPV4     ");
          s.append(9, ' ');
        } break;
        case OUTER_HEADER_REMOVAL_UDP_IPV6: {
          s.append("|UDP_IPV6     ");
          s.append(9, ' ');
        } break;
        default:
          s.append("|BAD_VALUE    ");
          s.append(9, ' ');
      }
    } else {
      s.append("|none         ");
      s.append(9, ' ');
    }
    s.append("|");
    if (pdr->pdi.first) {
      if (pdr->pdi.second.ue_ip_address.first) {
        std::string ip = {};
        if (pdr->pdi.second.ue_ip_address.second.v4) {
          ip =
              conv::toString(pdr->pdi.second.ue_ip_address.second.ipv4_address);
        }
        ip.resize(INET_ADDRSTRLEN, ' ');
        s.append(ip);
        // TODO IPv6
      }
    } else {
      std::string ip = {};
      ip.resize(INET_ADDRSTRLEN, ' ');
      s.append(ip);
    }
    s.append("|\n");
  }
  s.append(
      "+-----------------------------------------------------------------------"
      "------------------------------------------------------------------------"
      "---------------------------------+\n");
  return s;
}
