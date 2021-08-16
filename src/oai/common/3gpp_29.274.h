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

/*! \file 3gpp_29.274.h
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_3GPP_29_274_SEEN
#define FILE_3GPP_29_274_SEEN
#include "common_root_types.h"
#include "3gpp_24.008.h"
#include "3gpp_commons.h"
#include "conversions.hpp"
#include "logger.hpp"  // for fmt::format in spdlog

#include <arpa/inet.h>
#include <stdint.h>
#include <string>

extern const char* interface_type2char[];

namespace gtpv2c {

struct gtpc_exception : public std::exception {
  gtpc_exception() throw() {
    cause = 0;
    phrase.assign("GTPV2-C Exception unknown cause");
  }

  gtpc_exception(int acause) throw() {
    cause  = acause;
    phrase = fmt::format("GTPV2-C Exception cause {}", cause);
  }
  const char* what() const throw() { return phrase.c_str(); }

 public:
  int cause;
  std::string phrase;
};

struct gtpc_msg_bad_length_exception : public gtpc_exception {
 public:
  gtpc_msg_bad_length_exception(
      const uint8_t msg_type, const uint16_t msg_size) throw() {
    phrase = fmt::format(
        "GTPV2-C msg {} Bad Length {} Exception", msg_type, msg_size);
  }
  gtpc_msg_bad_length_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~gtpc_msg_bad_length_exception() throw() {}
};

struct gtpc_msg_unimplemented_ie_exception : public gtpc_exception {
 public:
  gtpc_msg_unimplemented_ie_exception(
      const uint8_t msg_type, const uint8_t ie_type,
      const uint8_t instance = 0) throw() {
    phrase = fmt::format(
        "GTPV2-C msg {} Unimplemented {} IE Instance {} Exception", msg_type,
        ie_type, instance);
  }
  gtpc_msg_unimplemented_ie_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~gtpc_msg_unimplemented_ie_exception() throw() {}
};

struct gtpc_msg_illegal_ie_exception : public gtpc_exception {
 public:
  gtpc_msg_illegal_ie_exception(
      const uint8_t msg_type, const uint8_t ie_type, const char* file,
      const int line) throw() {
    phrase = fmt::format(
        "GTPV2-C msg {} Illegal IE {} Exception {}:{}", msg_type, ie_type, file,
        line);
  }
  gtpc_msg_illegal_ie_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~gtpc_msg_illegal_ie_exception() throw() {}
};

struct gtpc_ie_exception : public gtpc_exception {
 public:
  gtpc_ie_exception(uint8_t ie_type) throw() {
    phrase = fmt::format("GTPV2-C IE {} Exception", ie_type);
  }
  gtpc_ie_exception(std::string& aphrase) throw() { phrase = aphrase; }
  virtual ~gtpc_ie_exception() throw() {}
};

struct gtpc_missing_ie_exception : public gtpc_exception {
 public:
  gtpc_missing_ie_exception(uint8_t gtpc_msg, uint8_t ie_type) throw() {
    phrase = fmt::format(
        "GTPV2-C msg {} missing IE {} Exception", gtpc_msg, ie_type);
  }
  gtpc_missing_ie_exception(const char* gtpc_msg, const char* ie_type) throw() {
    phrase = fmt::format(
        "GTPV2-C msg {} missing IE {} Exception", gtpc_msg, ie_type);
  }
  gtpc_missing_ie_exception(std::string& aphrase) throw() { phrase = aphrase; }
  virtual ~gtpc_missing_ie_exception() throw() {}
};

struct gtpc_ie_unimplemented_exception : public gtpc_ie_exception {
 public:
  gtpc_ie_unimplemented_exception(uint8_t ie_type) throw()
      : gtpc_ie_exception(ie_type) {
    phrase = fmt::format("GTPV2-C IE {} Unimplemented Exception", ie_type);
  }
  virtual ~gtpc_ie_unimplemented_exception() throw() {}
};

struct gtpc_tlv_exception : public gtpc_ie_exception {
 public:
  gtpc_tlv_exception(uint8_t ie_type) throw() : gtpc_ie_exception(ie_type) {
    phrase = fmt::format("GTPV2-C IE TLV {} Exception", ie_type);
  }
  virtual ~gtpc_tlv_exception() throw() {}
};

struct gtpc_tlv_bad_length_exception : public gtpc_tlv_exception {
 public:
  gtpc_tlv_bad_length_exception(uint8_t ie_type, uint16_t ie_length) throw()
      : gtpc_tlv_exception(ie_type) {
    phrase = fmt::format("GTPV2-C IE TLV {} Bad Length {} Exception", ie_type);
  }
  virtual ~gtpc_tlv_bad_length_exception() throw() {}
};

struct gtpc_tlv_bad_instance_exception : public gtpc_tlv_exception {
 public:
  gtpc_tlv_bad_instance_exception(uint8_t ie_type, uint8_t ie_instance) throw()
      : gtpc_tlv_exception(ie_type) {
    phrase = fmt::format(
        "GTPV2-C IE TLV {} Bad Instance {} Exception", ie_type, ie_instance);
  }
  virtual ~gtpc_tlv_bad_instance_exception() throw() {}
};

struct gtpc_ie_value_exception : public gtpc_ie_exception {
 public:
  gtpc_ie_value_exception(uint8_t ie_type, const char* field) throw()
      : gtpc_ie_exception(ie_type) {
    phrase =
        fmt::format("GTPV2-C IE {} Bad Value of {} Exception", ie_type, field);
  }
  virtual ~gtpc_ie_value_exception() throw() {}
};

#define GTP_IE_IMSI (1)
#define GTP_IE_CAUSE (2)
#define GTP_IE_RECOVERY_RESTART_COUNTER (3)
#define GTP_IE_ACCESS_POINT_NAME (71)
#define GTP_IE_AGGREGATE_MAXIMUM_BIT_RATE (72)
#define GTP_IE_EPS_BEARER_ID (73)
#define GTP_IE_IP_ADDRESS (74)
#define GTP_IE_MOBILE_EQUIPMENT_IDENTITY (75)
#define GTP_IE_MSISDN (76)
#define GTP_IE_INDICATION (77)
#define GTP_IE_PROTOCOL_CONFIGURATION_OPTIONS (78)
#define GTP_IE_PDN_ADDRESS_ALLOCATION (79)
#define GTP_IE_BEARER_QUALITY_OF_SERVICE (80)
#define GTP_IE_FLOW_QUALITY_OF_SERVICE (81)
#define GTP_IE_RAT_TYPE (82)
#define GTP_IE_SERVING_NETWORK (83)
#define GTP_IE_EPS_BEARER_LEVEL_TRAFFIC_FLOW_TEMPLATE (84)
#define GTP_IE_TRAFFIC_AGGREGATE_DESCRIPTION (85)
#define GTP_IE_USER_LOCATION_INFORMATION (86)
#define GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER (87)
#define GTP_IE_TMSI (88)
#define GTP_IE_GLOBAL_CN_ID (89)
#define GTP_IE_S103_PDN_DATA_FORWARDING_INFO (90)
#define GTP_IE_S1U_DATA_FORWARDING_INFO (91)
#define GTP_IE_DELAY_VALUE (92)
#define GTP_IE_BEARER_CONTEXT (93)
#define GTP_IE_CHARGING_ID (94)
#define GTP_IE_CHARGING_CHARACTERISTICS (95)
#define GTP_IE_TRACE_INFORMATION (96)
#define GTP_IE_BEARER_FLAGS (97)
#define GTP_IE_PDN_TYPE (99)
#define GTP_IE_PROCEDURE_TRANSACTION_ID (100)
#define GTP_IE_MM_CONTEXT_GSM_KEY_AND_TRIPLETS (103)
#define GTP_IE_MM_CONTEXT_UMTS_KEY_USED_CIPHER_AND_QUINTUPLETS (104)
#define GTP_IE_MM_CONTEXT_GSM_KEY_USED_CIPHER_AND_QUINTUPLETS (105)
#define GTP_IE_MM_CONTEXT_UMTS_KEY_AND_QUINTUPLETS (106)
#define GTP_IE_MM_CONTEXT_EPS_SECURITY_CONTEXT_AND_QUADRUPLETS (107)
#define GTP_IE_MM_CONTEXT_UMTS_KEY_QUADRUPLETS_AND_QUINTUPLETS (108)
#define GTP_IE_PDN_CONNECTION (109)
#define GTP_IE_PDU_NUMBERS (110)
#define GTP_IE_PACKET_TMSI (111)
#define GTP_IE_P_TMSI_SIGNATURE (112)
#define GTP_IE_HOP_COUNTER (113)
#define GTP_IE_UE_TIME_ZONE (114)
#define GTP_IE_TRACE_REFERENCE (115)
#define GTP_IE_COMPLETE_REQUEST_MESSAGE (116)
#define GTP_IE_GUTI (117)
#define GTP_IE_FULLY_QUALIFIED_CONTAINER (118)
#define GTP_IE_FULLY_QUALIFIED_CAUSE (119)
#define GTP_IE_PLMN_ID (120)
#define GTP_IE_TARGET_IDENTIFICATION (121)
#define GTP_IE_PACKET_FLOW_ID (123)
#define GTP_IE_RAB_CONTEXT (124)
#define GTP_IE_SOURCE_RNC_PDCP_CONTEXT_INFO (125)
#define GTP_IE_PORT_NUMBER (126)
#define GTP_IE_APN_RESTRICTION (127)
#define GTP_IE_SELECTION_MODE (128)
#define GTP_IE_SOURCE_IDENTIFICATION (129)
#define GTP_IE_CHANGE_REPORTING_ACTION (131)
#define GTP_IE_FQ_CSID (132)
#define GTP_IE_CHANNEL_NEEDED (133)
#define GTP_IE_EMLPP_PRIORITY (134)
#define GTP_IE_NODE_TYPE (135)
#define GTP_IE_FULLY_QUALIFIED_DOMAIN_NAME (136)
#define GTP_IE_TRANSACTION_IDENTIFIER (137)
#define GTP_IE_MBMS_SESSION_DURATION (138)
#define GTP_IE_MBMS_SERVICE_AREA (139)
#define GTP_IE_MBMS_SESSION_IDENTIFIER (140)
#define GTP_IE_MBMS_FLOW_IDENTIFIER (141)
#define GTP_IE_MBMS_IP_MULTICAST_DISTRIBUTION (142)
#define GTP_IE_MBMS_DISTRIBUTION_ACKNOWLEDGE (143)
#define GTP_IE_RFSP_INDEX (144)
#define GTP_IE_USER_CSG_INFORMATION (145)
#define GTP_IE_CSG_REPORTING_ACTION (146)
#define GTP_IE_CSG_ID (147)
#define GTP_IE_CSG_MEMBERSHIP_INDICATION (148)
#define GTP_IE_SERVICE_INDICATOR (149)
#define GTP_IE_DETACH_TYPE (150)
#define GTP_IE_LOCAL_DISTINGUISHED_NAME (151)
#define GTP_IE_NODE_FEATURES (152)
#define GTP_IE_MBMS_TIME_TO_DATA_TRANSFER (153)
#define GTP_IE_THROTTLING (154)
#define GTP_IE_ALLOCATION_RETENTION_PRIORITY (155)
#define GTP_IE_EPC_TIMER (156)
#define GTP_IE_SIGNALLING_PRIORITY_INDICATION (157)
#define GTP_IE_TMGI (158)
#define GTP_IE_ADDITIONAL_MM_CONTEXT_FOR_SRVCC (159)
#define GTP_IE_ADDITIONAL_FLAGS_FOR_SRVCC (160)
#define GTP_IE_MDT_CONFIGURATION (162)
#define GTP_IE_ADDITIONAL_PROTOCOL_CONFIGURATION_OPTIONS (163)
#define GTP_IE_ABSOLUTE_TIME_OF_MBMS_DATA_TRANSFER (164)
#define GTP_IE_H_E_NB_INFORMATION_REPORTING (165)
#define GTP_IE_IPV4_CONFIGURATION_PARAMETERS (166)
#define GTP_IE_CHANGE_TO_REPORT_FLAGS (167)
#define GTP_IE_ACTION_INDICATION (168)
#define GTP_IE_TWAN_IDENTIFIER (169)
#define GTP_IE_ULI_TIMESTAMP (170)
#define GTP_IE_MBMS_FLAGS (171)
#define GTP_IE_RAN_NAS_CAUSE (172)
#define GTP_IE_CN_OPERATOR_SELECTION_ENTITY (173)
#define GTP_IE_TRUSTED_WLAN_MODE_INDICATION (174)
#define GTP_IE_NODE_NUMBER (175)
#define GTP_IE_NODE_IDENTIFIER (176)
#define GTP_IE_PRESENCE_REPORTING_AREA_ACTION (177)
#define GTP_IE_PRESENCE_REPORTING_AREA_INFORMATION (178)
#define GTP_IE_TWAN_IDENTIFIER_TIMESTAMP (179)
#define GTP_IE_OVERLOAD_CONTROL_INFORMATION (180)
#define GTP_IE_LOAD_CONTROL_INFORMATION (181)
#define GTP_IE_METRIC (182)
#define GTP_IE_SEQUENCE_NUMBER (183)
#define GTP_IE_APN_AND_RELATIVE_CAPACITY (184)
#define GTP_IE_PAGING_AND_SERVICE_INFORMATION (186)
#define GTP_IE_INTEGER_NUMBER (187)
#define GTP_IE_MILLISECOND_TIME_STAMP (188)
#define GTP_IE_MONITORING_EVENT_INFORMATION (189)
#define GTP_IE_ECGI_LIST (190)
#define GTP_IE_REMOTE_UE_CONTEXT (191)
#define GTP_IE_REMOTE_USER_ID (192)
#define GTP_IE_REMOTE_UE_IP_INFORMATION (193)
#define GTP_IE_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION (194)
#define GTP_IE_SCEF_PDN_CONNECTION (195)
#define GTP_IE_HEADER_COMPRESSION_CONFIGURATION (196)
#define GTP_IE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS (197)
#define GTP_IE_SERVING_PLMN_RATE_CONTROL (198)
#define GTP_IE_COUNTER (199)
#define GTP_IE_MAPPED_UE_USAGE_TYPE (200)
#define GTP_IE_SECONDARY_RAT_USAGE_DATA_REPORT (201)
#define GTP_IE_UP_FUNCTION_SELECTION_INDICATION_FLAGS (202)
#define GTP_IE_PRIVATE_EXTENSION (255)
#define GTP_IE_WLAN_OFFLOADABILITY_INDICATION (1000)
#define GTP_IE_MAXIMUM_PACKET_LOSS (1001)

#define GTP_ECHO_REQUEST (1)
#define GTP_ECHO_RESPONSE (2)
#define GTP_VERSION_NOT_SUPPORTED_INDICATION (3)
#define GTP_CREATE_SESSION_REQUEST (32)
#define GTP_CREATE_SESSION_RESPONSE (33)
#define GTP_MODIFY_BEARER_REQUEST (34)
#define GTP_MODIFY_BEARER_RESPONSE (35)
#define GTP_DELETE_SESSION_REQUEST (36)
#define GTP_DELETE_SESSION_RESPONSE (37)
#define GTP_CHANGE_NOTIFICATION_REQUEST (38)
#define GTP_CHANGE_NOTIFICATION_RESPONSE (39)
#define GTP_REMOTE_UE_REPORT_NOTIFICATION (40)
#define GTP_REMOTE_UE_REPORT_ACKNOWLEDGE (41)
#define GTP_MODIFY_BEARER_COMMAND (64)
#define GTP_MODIFY_BEARER_FAILURE_INDICATION (65)
#define GTP_DELETE_BEARER_COMMAND (66)
#define GTP_DELETE_BEARER_FAILURE_INDICATION (67)
#define GTP_BEARER_RESOURCE_COMMAND (68)
#define GTP_BEARER_RESOURCE_FAILURE_INDICATION (69)
#define GTP_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION (70)
#define GTP_TRACE_SESSION_ACTIVATION (71)
#define GTP_TRACE_SESSION_DEACTIVATION (72)
#define GTP_STOP_PAGING_INDICATION (73)
#define GTP_CREATE_BEARER_REQUEST (95)
#define GTP_CREATE_BEARER_RESPONSE (96)
#define GTP_UPDATE_BEARER_REQUEST (97)
#define GTP_UPDATE_BEARER_RESPONSE (98)
#define GTP_DELETE_BEARER_REQUEST (99)
#define GTP_DELETE_BEARER_RESPONSE (100)
#define GTP_DELETE_PDN_CONNECTION_SET_REQUEST (101)
#define GTP_DELETE_PDN_CONNECTION_SET_RESPONSE (102)
#define GTP_PGW_DOWNLINK_TRIGGERING_NOTIFICATION (103)
#define GTP_PGW_DOWNLINK_TRIGGERING_ACKNOWLEDGE (104)
#define GTP_IDENTIFICATION_REQUEST (128)
#define GTP_IDENTIFICATION_RESPONSE (129)
#define GTP_CONTEXT_REQUEST (130)
#define GTP_CONTEXT_RESPONSE (131)
#define GTP_CONTEXT_ACKNOWLEDGE (132)
#define GTP_FORWARD_RELOCATION_REQUEST (133)
#define GTP_FORWARD_RELOCATION_RESPONSE (134)
#define GTP_FORWARD_RELOCATION_COMPLETE_NOTIFICATION (135)
#define GTP_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE (136)
#define GTP_FORWARD_ACCESS_CONTEXT_NOTIFICATION (137)
#define GTP_FORWARD_ACCESS_CONTEXT_ACKNOWLEDGE (138)
#define GTP_RELOCATION_CANCEL_REQUEST (139)
#define GTP_RELOCATION_CANCEL_RESPONSE (140)
#define GTP_CONFIGURATION_TRANSFER_TUNNEL_MESSAGE (141)
#define GTP_DETACH_NOTIFICATION (149)
#define GTP_DETACH_ACKNOWLEDGE (150)
#define GTP_CS_PAGING_INDICATION (151)
#define GTP_RAN_INFORMATION_RELAY (152)
#define GTP_ALERT_MME_NOTIFICATION (153)
#define GTP_ALERT_MME_ACKNOWLEDGE (154)
#define GTP_UE_ACTIVITY_NOTIFICATION (155)
#define GTP_UE_ACTIVITY_ACKNOWLEDGE (156)
#define GTP_ISR_STATUS_INDICATION (157)
#define GTP_UE_REGISTRATION_QUERY_REQUEST (158)
#define GTP_UE_REGISTRATION_QUERY_RESPONSE (159)
#define GTP_CREATE_FORWARDING_TUNNEL_REQUEST (160)
#define GTP_CREATE_FORWARDING_TUNNEL_RESPONSE (161)
#define GTP_SUSPEND_NOTIFICATION (162)
#define GTP_SUSPEND_ACKNOWLEDGE (163)
#define GTP_RESUME_NOTIFICATION (164)
#define GTP_RESUME_ACKNOWLEDGE (165)
#define GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST (166)
#define GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE (167)
#define GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST (168)
#define GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE (169)
#define GTP_RELEASE_ACCESS_BEARERS_REQUEST (170)
#define GTP_RELEASE_ACCESS_BEARERS_RESPONSE (171)
#define GTP_DOWNLINK_DATA_NOTIFICATION (176)
#define GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE (177)
#define GTP_PGW_RESTART_NOTIFICATION (179)
#define GTP_PGW_RESTART_NOTIFICATION_ACKNOWLEDGE (180)
#define GTP_UPDATE_PDN_CONNECTION_SET_REQUEST (200)
#define GTP_UPDATE_PDN_CONNECTION_SET_RESPONSE (201)
#define GTP_MODIFY_ACCESS_BEARERS_REQUEST (211)
#define GTP_MODIFY_ACCESS_BEARERS_RESPONSE (212)
#define GTP_MBMS_SESSION_START_REQUEST (231)
#define GTP_MBMS_SESSION_START_RESPONSE (232)
#define GTP_MBMS_SESSION_UPDATE_REQUEST (233)
#define GTP_MBMS_SESSION_UPDATE_RESPONSE (234)
#define GTP_MBMS_SESSION_STOP_REQUEST (235)
#define GTP_MBMS_SESSION_STOP_RESPONSE (236)

}  // namespace gtpv2c

//------------------------------------------------------------------------------
// 8.3 International Mobile Subscriber Identity (IMSI)
struct imsi_s {
  union {
    struct {
      uint8_t digit1 : 4;
      uint8_t digit2 : 4;
      uint8_t digit3 : 4;
      uint8_t digit4 : 4;
      uint8_t digit5 : 4;
      uint8_t digit6 : 4;
      uint8_t digit7 : 4;
      uint8_t digit8 : 4;
      uint8_t digit9 : 4;
      uint8_t digit10 : 4;
      uint8_t digit11 : 4;
      uint8_t digit12 : 4;
      uint8_t digit13 : 4;
      uint8_t digit14 : 4;
      uint8_t digit15 : 4;
      uint8_t filler : 4;
    } digits;
#define IMSI_BCD8_SIZE 8
    uint8_t b[IMSI_BCD8_SIZE];
  } u1;
  uint num_digits;

  imsi_s() : num_digits(0) { memset(u1.b, 0, sizeof(u1.b)); }

  imsi_s(const imsi_s& i) : num_digits(i.num_digits) {
    memcpy(u1.b, i.u1.b, sizeof(u1.b));
  }

  std::string toString() const {
    std::string s = {};
    int l_i       = 0;
    int l_j       = 0;
    while (l_i < IMSI_BCD8_SIZE) {
      if ((u1.b[l_i] & 0xf) > 9) break;
      s.append(std::to_string(u1.b[l_i] & 0xf));
      l_j++;
      if (((u1.b[l_i] & 0xf0) >> 4) > 9) break;
      s.append(std::to_string((u1.b[l_i] & 0xf0) >> 4));
      l_j++;
      l_i++;
    }
    return s;
  }

  //------------------------------------------------------------------------------
  imsi64_t to_imsi64() const {
    imsi64_t imsi64 = 0;
    for (int i = 0; i < IMSI_BCD8_SIZE; i++) {
      uint8_t d1 = u1.b[i];
      uint8_t d2 = (d1 & 0xf0) >> 4;
      d1         = d1 & 0x0f;
      if (10 > d1) {
        imsi64 = imsi64 * 10 + d1;
        if (10 > d2) {
          imsi64 = imsi64 * 10 + d2;
        } else {
          break;
        }
      } else {
        break;
      }
    }
    return imsi64;
  }

  imsi_s& operator++()  // prefix ++
  {
    int l_i       = IMSI_BCD8_SIZE - 1;
    uint8_t carry = 1;
    while (l_i > 5) {
      uint8_t b  = u1.b[l_i];
      uint8_t d0 = b & 0x0f;
      uint8_t d1 = b & 0xf0;
      if (d0 <= 9) {
        d0 += carry;
        if (d0 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d0        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      if (d1 <= 9) {
        d1 += carry;
        if (d1 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d1        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      l_i++;
    }
    return (*this);
  }
};
typedef struct imsi_s imsi_t;

//-------------------------------------
// 8.4 Cause
enum cause_value_e {
  /* Request / Initial message */
  LOCAL_DETACH                = 2,
  COMPLETE_DETACH             = 3,
  RAT_CHANGE_3GPP_TO_NON_3GPP = 4,  ///< RAT changed from 3GPP to Non-3GPP
  ISR_DEACTIVATION            = 5,
  ERROR_IND_FROM_RNC_ENB_SGSN = 6,
  IMSI_DETACH_ONLY            = 7,
  REACTIVATION_REQUESTED      = 8,
  PDN_RECONNECTION_TO_THIS_APN_DISALLOWED = 9,
  ACCESS_CHANGED_FROM_NON_3GPP_TO_3GPP    = 10,
  PDN_CONNECTION_INACTIVITY_TIMER_EXPIRES = 11,
  PGW_NOT_RESPONDING                      = 12,
  NETWORK_FAILURE                         = 13,
  QOS_PARAMETER_MISMATCH                  = 14,

  /* Acceptance in a Response/Triggered message */
  REQUEST_ACCEPTED           = 16,
  REQUEST_ACCEPTED_PARTIALLY = 17,
  NEW_PDN_TYPE_DUE_TO_NETWORK_PREFERENCE =
      18,  ///< New PDN type due to network preference
  NEW_PDN_TYPE_DUE_TO_SINGLE_ADDRESS_BEARER_ONLY =
      19,  ///< New PDN type due to single address bearer only
  /* Rejection in a Response triggered message. */
  CONTEXT_NOT_FOUND                                                      = 64,
  INVALID_MESSAGE_FORMAT                                                 = 65,
  VERSION_NOT_SUPPORTED_BY_NEXT_PEER                                     = 66,
  INVALID_LENGTH                                                         = 67,
  SERVICE_NOT_SUPPORTED                                                  = 68,
  MANDATORY_IE_INCORRECT                                                 = 69,
  MANDATORY_IE_MISSING                                                   = 70,
  SYSTEM_FAILURE                                                         = 72,
  NO_RESOURCES_AVAILABLE                                                 = 73,
  SEMANTIC_ERROR_IN_THE_TFT_OPERATION                                    = 74,
  SYNTACTIC_ERROR_IN_THE_TFT_OPERATION                                   = 75,
  SEMANTIC_ERRORS_IN_PACKET_FILTER                                       = 76,
  SYNTACTIC_ERRORS_IN_PACKET_FILTER                                      = 77,
  MISSING_OR_UNKNOWN_APN                                                 = 78,
  GRE_KEY_NOT_FOUND                                                      = 80,
  RELOCATION_FAILURE                                                     = 81,
  DENIED_IN_RAT                                                          = 82,
  PREFERRED_PDN_TYPE_NOT_SUPPORTED                                       = 83,
  ALL_DYNAMIC_ADDRESSES_ARE_OCCUPIED                                     = 84,
  UE_CONTEXT_WITHOUT_TFT_ALREADY_ACTIVATED                               = 85,
  PROTOCOL_TYPE_NOT_SUPPORTED                                            = 86,
  UE_NOT_RESPONDING                                                      = 87,
  UE_REFUSES                                                             = 88,
  SERVICE_DENIED                                                         = 89,
  UNABLE_TO_PAGE_UE                                                      = 90,
  NO_MEMORY_AVAILABLE                                                    = 91,
  USER_AUTHENTICATION_FAILED                                             = 92,
  APN_ACCESS_DENIED_NO_SUBSCRIPTION                                      = 93,
  REQUEST_REJECTED                                                       = 94,
  P_TMSI_SIGNATURE_MISMATCH                                              = 95,
  IMSI_IMEI_NOT_KNOWN                                                    = 96,
  SEMANTIC_ERROR_IN_THE_TAD_OPERATION                                    = 97,
  SYNTACTIC_ERROR_IN_THE_TAD_OPERATION                                   = 98,
  REMOTE_PEER_NOT_RESPONDING                                             = 100,
  COLLISION_WITH_NETWORK_INITIATED_REQUEST                               = 101,
  UNABLE_TO_PAGE_UE_DUE_TO_SUSPENSION                                    = 102,
  CONDITIONAL_IE_MISSING                                                 = 103,
  APN_RESTRICTION_TYPE_INCOMPATIBLE_WITH_CURRENTLY_ACTIVE_PDN_CONNECTION = 104,
  INVALID_OVERALL_LENGTH_OF_THE_TRIGGERED_RESPONSE_MESSAGE_AND_A_PIGGYBACKED_INITIAL_MESSAGE =
      105,
  DATA_FORWARDING_NOT_SUPPORTED                            = 106,
  INVALID_REPLY_FROM_REMOTE_PEER                           = 107,
  FALLBACK_TO_GTPV1                                        = 108,
  INVALID_PEER                                             = 109,
  TEMPORARILY_REJECTED_DUE_TO_TAU_HO_PROCEDURE_IN_PROGRESS = 110,
  MODIFICATIONS_NOT_LIMITED_TO_S1_U_BEARERS                = 111,
  REQUEST_REJECTED_FOR_PMIPv6_REASON                       = 112,
  APN_CONGESTION                                           = 113,
  BEARER_HANDLING_NOT_SUPPORTED                            = 114,
  UE_ALREADY_RE_ATTACHED                                   = 115,
  MULTIPLE_PDN_CONNECTIONS_FOR_A_GIVEN_APN_NOT_ALLOWED     = 116,
  TARGET_ACCESS_RESTRICTED_FOR_THE_SUBSCRIBER              = 117,
  MME_SGSN_REFUSES_DUE_TO_VPLMN_POLICY                     = 119,
  GTP_C_ENTITY_CONGESTION                                  = 120,
  LATE_OVERLAPPING_REQUEST                                 = 121,
  TIMED_OUT_REQUEST                                        = 122,
  UE_IS_TEMPORARILY_NOT_REACHABLE_DUE_TO_POWER_SAVING      = 123,
  RELOCATION_FAILURE_DUE_TO_NAS_MESSAGE_REDIRECTION        = 124,
  UE_NOT_AUTHORISED_BY_OCS_OR_EXTERNAL_AAA_SERVER          = 125,
  MULTIPLE_ACCESSES_TO_A_PDN_CONNECTION_NOT_ALLOWED        = 126,
  REQUEST_REJECTED_DUE_TO_UE_CAPABILITY                    = 127,
  S1_U_PATH_FAILURE                                        = 128
};

typedef struct cause_s {
  uint8_t cause_value;
  uint8_t pce : 1;
  uint8_t bce : 1;
  uint8_t cs : 1;
  uint8_t offending_ie_instance : 4;
  uint8_t filler : 1;
  uint8_t offending_ie_type;
  uint16_t offending_ie_length;
} cause_t;
//------------------------------------------------------------------------------
// 8.5 recovery
// typedef struct recovery_s {
//  uint8_t recovery;
//} recovery_t;
//------------------------------------------------------------------------------
// 8.6 Access Point Name
typedef struct apn_s {
  std::string access_point_name;
} apn_t;
//------------------------------------------------------------------------------
// 8.7  Aggregate Maximum Bit Rate (AMBR)
typedef struct ambr_s {
  uint32_t br_ul;
  uint32_t br_dl;
} ambr_t;
//------------------------------------------------------------------------------
// 8.8 EPS Bearer ID (EBI)
#define EPS_BEARER_IDENTITY_UNASSIGNED (uint8_t) 0
#define EPS_BEARER_IDENTITY_RESERVED1 (uint8_t) 1
#define EPS_BEARER_IDENTITY_RESERVED2 (uint8_t) 2
#define EPS_BEARER_IDENTITY_RESERVED3 (uint8_t) 3
#define EPS_BEARER_IDENTITY_RESERVED4 (uint8_t) 4
#define EPS_BEARER_IDENTITY_FIRST (uint8_t) 5
#define EPS_BEARER_IDENTITY_LAST (uint8_t) 15

typedef struct ebi_s {
  uint8_t ebi;
  ebi_s() : ebi(EPS_BEARER_IDENTITY_UNASSIGNED) {}
  ebi_s(const uint8_t& e) : ebi(e) {}
  ebi_s(const struct ebi_s& e) : ebi(e.ebi) {}

  inline bool operator==(const struct ebi_s& rhs) const {
    return ebi == rhs.ebi;
  }
  inline bool operator!=(const struct ebi_s& rhs) const {
    return !(ebi == rhs.ebi);
  }
} ebi_t;
//-------------------------------------
// 8.9 IP Address
typedef struct ip_address_s {
  bool is_ipv4;  // if not ipv4, then it is ipv6
  union {
    struct in_addr ipv4_address;
    struct in6_addr ipv6_address;
  } address;
} ip_address_t;
//-------------------------------------
// 8.10 Mobile Equipment Identity (MEI)
// The ME Identity field contains either the IMEI or the IMEISV as defined in
// subclause 6.2 of 3GPP TS 23.003
struct mei_s {
#define MEI_MIN_LENGTH (15)
#define MEI_MAX_LENGTH (16)
  union {
    struct {
      uint8_t digit1 : 4;
      uint8_t digit2 : 4;
      uint8_t digit3 : 4;
      uint8_t digit4 : 4;
      uint8_t digit5 : 4;
      uint8_t digit6 : 4;
      uint8_t digit7 : 4;
      uint8_t digit8 : 4;
      uint8_t digit9 : 4;
      uint8_t digit10 : 4;
      uint8_t digit11 : 4;
      uint8_t digit12 : 4;
      uint8_t digit13 : 4;
      uint8_t digit14 : 4;
      uint8_t digit15 : 4;
      uint8_t filler : 4;
    } digits;
    uint8_t b[MEI_MAX_LENGTH / 2];
  } u1;
  uint num_digits;

  mei_s() : num_digits(0) { memset(u1.b, 0, sizeof(u1.b)); }

  mei_s(const mei_s& i) : num_digits(i.num_digits) {
    memcpy(u1.b, i.u1.b, sizeof(u1.b));
  }

  std::string toString() const {
    std::string s = {};
    int l_i       = 0;
    int l_j       = 0;
    while (l_i < MEI_MAX_LENGTH / 2) {
      if ((u1.b[l_i] & 0xf) > 9) break;
      s.append(std::to_string(u1.b[l_i] & 0xf));
      l_j++;
      if (((u1.b[l_i] & 0xf0) >> 4) > 9) break;
      s.append(std::to_string((u1.b[l_i] & 0xf0) >> 4));
      l_j++;
      l_i++;
    }
    return s;
  }

  mei_s& operator++()  // prefix ++
  {
    int l_i =
        MEI_MAX_LENGTH / 2 - 1 - 1;  // depends if imei or imei_sv -1 again
    uint8_t carry = 1;
    while (l_i) {
      uint8_t b  = u1.b[l_i];
      uint8_t d0 = b & 0x0f;
      uint8_t d1 = b & 0xf0;
      if (d0 <= 9) {
        d0 += carry;
        if (d0 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d0        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      if (d1 <= 9) {
        d1 += carry;
        if (d1 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d1        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      l_i++;
    }
    return (*this);
  }
};

typedef struct mei_s mei_t;
//-------------------------------------
// 8.11 MSISDN
struct msisdn_s {
#define MSISDN_MAX_LENGTH (15)
  union {
    struct {
      uint8_t digit1 : 4;
      uint8_t digit2 : 4;
      uint8_t digit3 : 4;
      uint8_t digit4 : 4;
      uint8_t digit5 : 4;
      uint8_t digit6 : 4;
      uint8_t digit7 : 4;
      uint8_t digit8 : 4;
      uint8_t digit9 : 4;
      uint8_t digit10 : 4;
      uint8_t digit11 : 4;
      uint8_t digit12 : 4;
      uint8_t digit13 : 4;
      uint8_t digit14 : 4;
      uint8_t digit15 : 4;
    } digits;
    uint8_t b[MSISDN_MAX_LENGTH / 2 + 1];
  } u1;
  uint num_digits;

  msisdn_s() : num_digits(0) { memset(u1.b, 0, sizeof(u1.b)); }

  msisdn_s(const msisdn_s& i) : num_digits(i.num_digits) {
    memcpy(u1.b, i.u1.b, sizeof(u1.b));
  }

  std::string toString() const {
    std::string s = {};
    int l_i       = 0;
    int l_j       = 0;
    while (l_i < sizeof(u1.b)) {
      if ((u1.b[l_i] & 0xf) > 9) break;
      s.append(std::to_string(u1.b[l_i] & 0xf));
      l_j++;
      if (((u1.b[l_i] & 0xf0) >> 4) > 9) break;
      s.append(std::to_string((u1.b[l_i] & 0xf0) >> 4));
      l_j++;
      l_i++;
    }
    return s;
  }

  // Should be refined see spec
  msisdn_s& operator++()  // prefix ++
  {
    int l_i       = sizeof(u1.b) - 1;
    uint8_t carry = 1;
    while (l_i > 5) {
      uint8_t b  = u1.b[l_i];
      uint8_t d0 = b & 0x0f;
      uint8_t d1 = b & 0xf0;
      if (d0 <= 9) {
        d0 += carry;
        if (d0 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d0        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      if (d1 <= 9) {
        d1 += carry;
        if (d1 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d1        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      l_i++;
    }
    return (*this);
  }
};

typedef struct msisdn_s msisdn_t;

//-------------------------------------
// 8.12 Indication
typedef struct indication_s {
  uint8_t daf : 1;
  uint8_t dtf : 1;
  uint8_t hi : 1;
  uint8_t dfi : 1;
  uint8_t oi : 1;
  uint8_t isrsi : 1;
  uint8_t israi : 1;
  uint8_t sgwci : 1;

  uint8_t sqci : 1;
  uint8_t uimsi : 1;
  uint8_t cfsi : 1;
  uint8_t crsi : 1;
  uint8_t p : 1;
  uint8_t pt : 1;
  uint8_t si : 1;
  uint8_t msv : 1;

  uint8_t retloc : 1;
  uint8_t pbic : 1;
  uint8_t srni : 1;
  uint8_t s6af : 1;
  uint8_t s4af : 1;
  uint8_t mbmdt : 1;
  uint8_t israu : 1;
  uint8_t ccrsi : 1;

  uint8_t cprai : 1;
  uint8_t arrl : 1;
  uint8_t ppof : 1;
  uint8_t ppon : 1;
  uint8_t ppsi : 1;
  uint8_t csfbi : 1;
  uint8_t clii : 1;
  uint8_t cpsr : 1;

  uint8_t nsi : 1;
  uint8_t uasi : 1;
  uint8_t dtci : 1;
  uint8_t bdwi : 1;
  uint8_t psci : 1;
  uint8_t pcri : 1;
  uint8_t aosi : 1;
  uint8_t aopi : 1;

  uint8_t roaai : 1;
  uint8_t epcosi : 1;
  uint8_t cpopci : 1;
  uint8_t pmtsmi : 1;
  uint8_t s11tf : 1;
  uint8_t pnsi : 1;
  uint8_t unaccsi : 1;
  uint8_t wpmsi : 1;

  uint8_t spare1 : 1;
  uint8_t spare2 : 1;
  uint8_t spare3 : 1;
  uint8_t eevrsi : 1;
  uint8_t ltemui : 1;
  uint8_t ltempi : 1;
  uint8_t enbcpi : 1;
  uint8_t tspcmi : 1;
} indication_t;
//-------------------------------------
// 8.13 Protocol Configuration Options (PCO)
//-------------------------------------
// 8.34 PDN Type
enum pdn_type_e {
  PDN_TYPE_E_IPV4   = 1,
  PDN_TYPE_E_IPV6   = 2,
  PDN_TYPE_E_IPV4V6 = 3,
  PDN_TYPE_E_NON_IP = 4,
};
static const std::vector<std::string> pdn_type_e2str = {"Error", "IPV4", "IPV6",
                                                        "IPV4V6", "NON_IP"};

typedef struct pdn_type_s {
  uint8_t pdn_type;
  pdn_type_s() : pdn_type(PDN_TYPE_E_IPV4) {}
  pdn_type_s(const uint8_t& p) : pdn_type(p) {}
  pdn_type_s(const struct pdn_type_s& p) : pdn_type(p.pdn_type) {}
  bool operator==(const struct pdn_type_s& p) const {
    return (p.pdn_type == pdn_type);
  }
  //------------------------------------------------------------------------------
  bool operator==(const pdn_type_e& p) const { return (p == pdn_type); }
  //------------------------------------------------------------------------------
  const std::string& toString() const { return pdn_type_e2str.at(pdn_type); }
} pdn_type_t;

//-------------------------------------
// 8.14 PDN Address Allocation (PAA)
struct paa_s {
  pdn_type_t pdn_type;
  uint8_t ipv6_prefix_length;
  struct in6_addr ipv6_address;
  struct in_addr ipv4_address;
  //------------------------------------------------------------------------------
  bool is_ip_assigned() {
    switch (pdn_type.pdn_type) {
      case PDN_TYPE_E_IPV4:
        if (ipv4_address.s_addr) return true;
        return false;
        break;
      case PDN_TYPE_E_IPV6:
        if (ipv6_address.s6_addr32[0] | ipv6_address.s6_addr32[1] |
            ipv6_address.s6_addr32[2] | ipv6_address.s6_addr32[3])
          return true;
        return false;
        break;
      case PDN_TYPE_E_IPV4V6:
        // TODO
        if (ipv4_address.s_addr) return true;
        if (ipv6_address.s6_addr32[0] | ipv6_address.s6_addr32[1] |
            ipv6_address.s6_addr32[2] | ipv6_address.s6_addr32[3])
          return true;
        return false;
        break;
      case PDN_TYPE_E_NON_IP:
      default:
        return false;
    }
  }
};

typedef struct paa_s paa_t;
//-------------------------------------
// 8.15 Bearer Quality of Service (Bearer QoS)
#define PRE_EMPTION_CAPABILITY_ENABLED (0x0)
#define PRE_EMPTION_CAPABILITY_DISABLED (0x1)
#define PRE_EMPTION_VULNERABILITY_ENABLED (0x0)
#define PRE_EMPTION_VULNERABILITY_DISABLED (0x1)

typedef struct bearer_qos_s {
  uint8_t spare1 : 1;
  uint8_t pci : 1;
  uint8_t pl : 4;
  uint8_t spare2 : 1;
  uint8_t pvi : 1;
  uint8_t label_qci;
  uint64_t maximum_bit_rate_for_uplink;
  uint64_t maximum_bit_rate_for_downlink;
  uint64_t guaranted_bit_rate_for_uplink;
  uint64_t guaranted_bit_rate_for_downlink;

  bool operator==(const struct bearer_qos_s& q) const {
    return (
        (q.label_qci == label_qci) && (q.pl == pl) && (q.pvi == pvi) &&
        (q.pci == pci));
  }
  //------------------------------------------------------------------------------
  std::string toString() const {
    std::string s = {};
    s.append("MBR UL=").append(std::to_string(maximum_bit_rate_for_uplink));
    s.append(", MBR DL=").append(std::to_string(maximum_bit_rate_for_downlink));
    s.append(", GBR UL=").append(std::to_string(guaranted_bit_rate_for_uplink));
    s.append(", GBR DL=")
        .append(std::to_string(guaranted_bit_rate_for_downlink));
    s.append(", PCI=").append(std::to_string(pci));
    s.append(", PL=").append(std::to_string(pl));
    s.append(", PVI=").append(std::to_string(pvi));
    s.append(", QCI=").append(std::to_string(label_qci));
    return s;
  }
} bearer_qos_t;

//-------------------------------------
// 8.16 Flow Quality of Service (Flow QoS)
typedef struct flow_qos_s {
  uint8_t label_qci;
  uint64_t maximum_bit_rate_for_uplink;
  uint64_t maximum_bit_rate_for_downlink;
  uint64_t guaranted_bit_rate_for_uplink;
  uint64_t guaranted_bit_rate_for_downlink;
} flow_qos_t;
//-------------------------------------
// 8.17 RAT Type
enum rat_type_e {
  RAT_TYPE_E_RESERVED         = 0,
  RAT_TYPE_E_UTRAN            = 1,
  RAT_TYPE_E_GERAN            = 2,
  RAT_TYPE_E_WLAN             = 3,
  RAT_TYPE_E_GAN              = 4,
  RAT_TYPE_E_HSPA_EVOLUTION   = 5,
  RAT_TYPE_E_EUTRAN_WB_EUTRAN = 6,
  RAT_TYPE_E_VIRTUAL          = 7,
  RAT_TYPE_E_EUTRAN_NB_IOT    = 8,
  RAT_TYPE_E_LTE_M            = 9,
  RAT_TYPE_E_NR               = 10,
};
struct rat_type_s {
  uint8_t rat_type;
  rat_type_s() : rat_type(RAT_TYPE_E_EUTRAN_WB_EUTRAN) {}
  rat_type_s(const rat_type_e r) : rat_type(r) {}
  rat_type_s(const rat_type_s& i) : rat_type(i.rat_type) {}
  //------------------------------------------------------------------------------
  std::string toString() const {
    switch (rat_type) {
      case RAT_TYPE_E_EUTRAN_WB_EUTRAN:
        return std::string("EUTRAN_WB_EUTRAN");
      case RAT_TYPE_E_EUTRAN_NB_IOT:
        return std::string("EUTRAN_NB_IOT");
      case RAT_TYPE_E_LTE_M:
        return std::string("LTE_M");
      case RAT_TYPE_E_NR:
        return std::string("NR");
      case RAT_TYPE_E_VIRTUAL:
        return std::string("VIRTUAL");
      case RAT_TYPE_E_RESERVED:
        return std::string("RESERVED");
      case RAT_TYPE_E_UTRAN:
        return std::string("UTRAN");
      case RAT_TYPE_E_GERAN:
        return std::string("GERAN");
      case RAT_TYPE_E_WLAN:
        return std::string("WLAN");
      case RAT_TYPE_E_GAN:
        return std::string("GAN");
      case RAT_TYPE_E_HSPA_EVOLUTION:
        return std::string("HSPA_EVOLUTION");
      default:
        return std::to_string(rat_type);
    }
  }
};
typedef struct rat_type_s rat_type_t;
//-------------------------------------
// 8.18 Serving Network
typedef struct serving_network_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
} serving_network_t;
//-------------------------------------
// 8.19 EPS Bearer Level Traffic Flow Template (Bearer TFT)

//-------------------------------------
// 8.20 Traffic Aggregate Description (TAD)
//-------------------------------------
// 8.21.1 CGI field
typedef struct gtpc2c_cgi_field_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
  uint16_t location_area_code;
  uint16_t cell_identity;
} cgi_field_t;
//-------------------------------------
// 8.21.2 SAI field
typedef struct gtpc2c_sai_field_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
  uint16_t location_area_code;
  uint16_t service_area_code;
} sai_field_t;
//-------------------------------------
// 8.21.3 RAI field
typedef struct gtpc2c_rai_field_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
  uint16_t location_area_code;
  uint16_t routing_area_code;
} rai_field_t;
//-------------------------------------
// 8.21.4 TAI field
typedef struct gtpc2c_tai_field_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
  uint16_t tracking_area_code;
} tai_field_t;
//-------------------------------------
// 8.21.5 ECGI field
typedef struct gtpc2c_ecgi_field_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
  uint8_t spare : 4;
  uint8_t eci : 4;
  uint8_t e_utran_cell_identifier[3];

  //------------------------------------------------------------------------------
  std::string toString() const {
    std::string s    = {};
    std::string mccs = conv::mccToString(mcc_digit_1, mcc_digit_2, mcc_digit_3);
    std::string mncs = conv::mncToString(mnc_digit_1, mnc_digit_2, mnc_digit_3);
    s.append("mcc=").append(mccs).append(", MNC=").append(mncs);
    s.append(", ECI=").append(std::to_string(eci));
    uint32_t we_utran_cell_identifier =
        static_cast<uint32_t>(e_utran_cell_identifier[0]) << 16;
    we_utran_cell_identifier |=
        (static_cast<uint32_t>(e_utran_cell_identifier[1]) << 8);
    we_utran_cell_identifier |=
        static_cast<uint32_t>(e_utran_cell_identifier[2]);
    s.append(", EUCI=").append(std::to_string(we_utran_cell_identifier));
    return s;
  }
} ecgi_field_t;

//-------------------------------------
// 8.21.6 LAI field
typedef struct gtpc2c_lai_field_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
  uint16_t location_area_code;
} lai_field_t;
//-------------------------------------
// 8.21.7 Macro eNodeB ID field
typedef struct gtpc2c_macro_enodeb_id_field_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
  uint32_t spare : 4;
  uint32_t macro_enodeb_id : 20;
  uint32_t lost_bits : 8;
} macro_enodeb_id_field_t;
//-------------------------------------
// 8.21.8 Extended Macro eNodeB ID field
typedef struct gtpc2c_extended_macro_enodeb_id_field_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
  uint32_t smenb : 1;
  uint32_t spare : 2;
  uint32_t extended_macro_enodeb_id : 21;
  uint32_t lost_bits : 8;
} extended_macro_enodeb_id_field_t;
//-------------------------------------
// 8.21 User Location Information (ULI)
typedef struct user_location_information_s {
  struct user_location_information_ie_hdr_t {
    uint8_t extended_macro_enodeb_id : 1;
    uint8_t macro_enodeb_id : 1;
    uint8_t lai : 1;
    uint8_t ecgi : 1;
    uint8_t tai : 1;
    uint8_t rai : 1;
    uint8_t sai : 1;
    uint8_t cgi : 1;
  } user_location_information_ie_hdr;
  cgi_field_t cgi1;
  sai_field_t sai1;
  rai_field_t rai1;
  tai_field_t tai1;
  ecgi_field_t ecgi1;
  lai_field_t lai1;
  macro_enodeb_id_field_t macro_enodeb_id1;
  extended_macro_enodeb_id_field_t extended_macro_enodeb_id1;
} user_location_information_t;

typedef user_location_information_t uli_t;

//-------------------------------------
// 8.22 Fully Qualified TEID (F-TEID)

/* WARNING: not complete... */
enum interface_type_e {
  INTERFACE_TYPE_MIN               = 0,
  S1_U_ENODEB_GTP_U                = INTERFACE_TYPE_MIN,
  S1_U_SGW_GTP_U                   = 1,
  S12_RNC_GTP_U                    = 2,
  S12_SGW_GTP_U                    = 3,
  S5_S8_SGW_GTP_U                  = 4,
  S5_S8_PGW_GTP_U                  = 5,
  S5_S8_SGW_GTP_C                  = 6,
  S5_S8_PGW_GTP_C                  = 7,
  S5_S8_SGW_PMIPv6                 = 8,
  S5_S8_PGW_PMIPv6                 = 9,
  S11_MME_GTP_C                    = 10,
  S11_S4_SGW_GTP_C                 = 11,
  S10_MME_GTP_C                    = 12,
  S3_MME_GTP_C                     = 13,
  S3_SGSN_GTP_C                    = 14,
  S4_SGSN_GTP_U                    = 15,
  S4_SGW_GTP_U                     = 16,
  S4_SGSN_GTP_C                    = 17,
  S16_SGSN_GTP_C                   = 18,
  ENODEB_GTP_U_DL_DATA_FORWARDING  = 19,
  ENODEB_GTP_U_UL_DATA_FORWARDING  = 20,
  RNC_GTP_U_DATA_FORWARDING        = 21,
  SGSN_GTP_U_DATA_FORWARDING       = 22,
  SGW_UPF_GTP_U_DL_DATA_FORWARDING = 23,
  SM_MBMS_GW_GTP_C                 = 24,
  SN_MBMS_GW_GTP_C                 = 25,
  SM_MME_GTP_C                     = 26,
  SN_SGSN_GTP_C                    = 27,
  SGW_GTP_U_UL_DATA_FORWARDING     = 28,
  SN_SGSN_GTP_U                    = 29,
  S2B_EPDG_GTP_C                   = 30,
  S2B_U_EPDG_GTP_U                 = 31,
  S2B_PGW_GTP_C                    = 32,
  S2B_U_PGW_GTP_U                  = 33,
  S2A_TWAN_GTP_U                   = 34,
  S2A_TWAN_GTP_C                   = 35,
  S2A_PGW_GTP_C                    = 36,
  S2A_PGW_GTP_U                    = 37,
  S11_MME_GTP_U                    = 38,
  S11_SGW_GTP_U                    = 39,
  INTERFACE_TYPE_MAX               = S11_SGW_GTP_U
};

struct interface_type_s {
  interface_type_e interface_type;

  interface_type_s() : interface_type(INTERFACE_TYPE_MIN) {}
  interface_type_s(interface_type_e t) : interface_type(t) {}

  //------------------------------------------------------------------------------
  std::string toString() const {
    if ((interface_type >= INTERFACE_TYPE_MIN) &&
        (interface_type <= INTERFACE_TYPE_MAX)) {
      return std::string(interface_type2char[interface_type]);
    } else {
      return std::to_string(interface_type);
    }
  }
};

typedef struct interface_type_s interface_type_t;

struct fully_qualified_tunnel_endpoint_identifier_s {
  uint8_t v4 : 1;
  uint8_t v6 : 1;
  uint8_t interface_type : 6;
  uint32_t teid_gre_key;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;

  bool operator<(
      const struct fully_qualified_tunnel_endpoint_identifier_s& f) const {
    return (teid_gre_key < f.teid_gre_key) or
           (ipv4_address.s_addr < f.ipv4_address.s_addr) or
           (interface_type < f.interface_type) or (v4 == f.v4) or
           (v6 == f.v6) or
           (ipv6_address.s6_addr32[0] == f.ipv6_address.s6_addr32[0]) or
           (ipv6_address.s6_addr32[1] == f.ipv6_address.s6_addr32[1]) or
           (ipv6_address.s6_addr32[2] == f.ipv6_address.s6_addr32[2]) or
           (ipv6_address.s6_addr32[3] == f.ipv6_address.s6_addr32[3]);
  }

  bool operator==(
      const struct fully_qualified_tunnel_endpoint_identifier_s& f) const {
    return (teid_gre_key == f.teid_gre_key) and
           (ipv4_address.s_addr == f.ipv4_address.s_addr) and
           (interface_type == f.interface_type) and
           (ipv6_address.s6_addr32[0] == f.ipv6_address.s6_addr32[0]) and
           (ipv6_address.s6_addr32[1] == f.ipv6_address.s6_addr32[1]) and
           (ipv6_address.s6_addr32[2] == f.ipv6_address.s6_addr32[2]) and
           (ipv6_address.s6_addr32[3] == f.ipv6_address.s6_addr32[3]) and
           (v4 == f.v4) and (v6 == f.v6);
  }
  //------------------------------------------------------------------------------
  std::string toString() const {
    std::string s = {};
    interface_type_t iface_type((interface_type_e) interface_type);
    if ((v4) || (v6)) {
      s.append("Interface type=").append(iface_type.toString());
      s.append(", TEID=").append(std::to_string(teid_gre_key));
      if (v4) {
        s.append(", IPv4=").append(conv::toString(ipv4_address));
      }
      if (v6) {
        s.append(", IPv6=").append(conv::toString(ipv6_address));
      }
    } else {
      s.append("null_fteid");
    }
    return s;
  }
  bool is_zero() const { return ((!v4) and (!v6)); }
};

typedef struct fully_qualified_tunnel_endpoint_identifier_s
    fully_qualified_tunnel_endpoint_identifier_t;
typedef struct fully_qualified_tunnel_endpoint_identifier_s fteid_t;

//-------------------------------------
// 8.24 Global CN-Id
// typedef struct global_cn_id_s {
//  uint8_t mcc_digit_2 :4;
//  uint8_t mcc_digit_1 :4;
//  uint8_t mnc_digit_3 :4;
//  uint8_t mcc_digit_3 :4;
//  uint8_t mnc_digit_2 :4;
//  uint8_t mnc_digit_1 :4;
//  cn_id_t cn_id;
//} global_cn_id_t;

//-------------------------------------
// 8.27 Delay Value
typedef struct delay_value_s {
  uint8_t delay_value;
} delay_value_t;

//-------------------------------------
// 8.28 Bearer Context
//-------------------------------------
// 8.29 Charging ID
typedef struct charging_id_s {
  uint32_t charging_id_value;
} charging_id_t;

//-------------------------------------
// 8.30 Charging Characteristics
typedef struct charging_characteristics_s {
  uint16_t charging_characteristics_value;
} charging_characteristics_t;

//-------------------------------------
// 8.31 Trace Information
// typedef struct trace_information_s {
//  uint8_t mcc_digit_2 :4;
//  uint8_t mcc_digit_1 :4;
//  uint8_t mnc_digit_3 :4;
//  uint8_t mcc_digit_3 :4;
//  uint8_t mnc_digit_2 :4;
//  uint8_t mnc_digit_1 :4;
//  uint8_t trace_id[3];
//  uint8_t triggering_events[9];
//  uint16_t list_of_ne_types;
//  uint8_t session_trace_depth;
//  uint16_t list_of_interfaces;
//  char* ip_address_of_trace_collection_entity_;
//} trace_information_t;

//-------------------------------------
// 8.32 Bearer Flags
typedef struct bearer_flags_s {
  uint8_t spare1 : 4;
  uint8_t asi : 1;
  uint8_t vind : 1;
  uint8_t vb : 1;
  uint8_t ppc : 1;
} bearer_flags_t;

//-------------------------------------
// 8.35 Procedure Transaction ID (PTI)
typedef struct procedure_transaction_id_s {
  uint8_t procedure_transaction_id;
} procedure_transaction_id_t;

//-------------------------------------
// 8.38 MM Context
// 8.38 MM EPS Context

// typedef struct mm_context_eps_s {
//  // todo: better structure for flags
////  uint32_t                  mm_context_flags:24;
//  uint8_t                   sec_mode:3;
//  // todo: uint8_t                   drxi:1;
//  uint8_t                   ksi:3;
//  uint8_t                   num_quit:3;
//  uint8_t                   num_quad:3;
//  // todo: osci 0 --> old stuff (everything from s to s+64 in 29.274 -->
//  8-38.5 not present uint8_t                   nas_int_alg:3; uint8_t
//  nas_cipher_alg:4;
////  uint32_t                   nas_dl_count[3]; // todo: or directly uint32_t?
////  uint8_t                   nas_ul_count[3]; // todo: or directly uint32_t?
//  count_t                   nas_dl_count;
//  count_t                   nas_ul_count;
//  uint8_t                   k_asme[32];
//  mm_ue_eps_authentication_quadruplet_t* auth_quadruplet[5];
//  mm_ue_eps_authentication_quintuplet_t* auth_quintuplet[5];
//  // todo : drx_t*                    drx;
//  uint8_t                   nh[32];
//  uint8_t                   ncc:3;
//  uint32_t                  ul_subscribed_ue_ambr;
//  uint32_t                  dl_subscribed_ue_ambr;
//  uint32_t                  ul_used_ue_ambr;
//  uint32_t                  dl_used_ue_ambr;
//  uint8_t                   ue_nc_length;
//  ue_network_capability_t   ue_nc;
//  uint8_t                   ms_nc_length;
//  ms_network_capability_t   ms_nc;
//  uint8_t                   mei_length;
//  Mei_t                     mei;
//  uint8_t                   vdp_lenth;
//  uint8_t                   vdp; // todo: ??
//  uint8_t                   access_restriction_flags;
//} mm_context_eps_t;

//-------------------------------------------------
// 8.44 UE Time Zone
typedef struct ue_time_zone_s {
  uint8_t time_zone;
  uint8_t spare1 : 6;
  uint8_t daylight_saving_time : 2;
} ue_time_zone_t;

//-------------------------------------------------
// 8.49 Fully Qualified Cause (F-Cause)
// typedef struct fully_qualified_cause_s {
//  uint8_t spare1 :4;
//  uint8_t cause_type :4;
//  char* f-cause_field;
//} fully_qualified_cause_t;

//-------------------------------------------------
// 8.50 PLMN ID
typedef struct plmn_id_s {
  union {
    struct {
      uint8_t mcc_digit_2 : 4;
      uint8_t mcc_digit_1 : 4;
      uint8_t mnc_digit_1 : 4;
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
      uint8_t mnc_digit_2 : 4;
    } mnc3;
    struct {
      uint8_t mcc_digit_2 : 4;
      uint8_t mcc_digit_1 : 4;
      uint8_t filler1111 : 4;
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_2 : 4;
      uint8_t mnc_digit_1 : 4;
    } mnc2;
    uint8_t b;
  } u;
} plmn_id_t;

//-------------------------------------
// 8.51 Target Identification
enum target_type_e {
  TARGET_TYPE_E_RNC_ID                   = 0,
  TARGET_TYPE_E_MACRO_ENODEB_ID          = 1,
  TARGET_TYPE_E_CELL_IDENTIFIER          = 2,
  TARGET_TYPE_E_HOME_ENODEB_ID           = 3,
  TARGET_TYPE_E_EXTENDED_MACRO_ENODEB_ID = 4,
  TARGET_TYPE_E_GNODEB_ID                = 5,
  TARGET_TYPE_E_MACRO_NG_ENODEB_ID       = 6,
  TARGET_TYPE_E_EXTENDED_NG_ENODEB_ID    = 7,
};

typedef struct target_id_for_type_rnc_id_s {
  struct target_id_for_type_rnc_id_hdr_s {
    uint8_t mcc_digit_2 : 4;
    uint8_t mcc_digit_1 : 4;
    uint8_t mnc_digit_3 : 4;
    uint8_t mcc_digit_3 : 4;
    uint8_t mnc_digit_2_ : 4;
    uint8_t mnc_digit_1 : 4;
    uint16_t lac;
    uint8_t rac;
    uint16_t rnc_id;
  } target_id_for_type_rnc_id_hdr;
  char* extended_rnc_id;
} target_id_for_type_rnc_id_t;

typedef struct target_id_for_type_macro_enodeb_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2_ : 4;
  uint8_t mnc_digit_1 : 4;
  uint32_t spare1 : 4;
  uint32_t macro_enodeb_id : 20;
  uint16_t tracking_area_code;
} target_id_for_type_macro_enodeb_t;

typedef struct target_id_for_type_cell_identifier_s {
  uint8_t cell_identifier[8];
} target_id_for_type_cell_identifier_t;

typedef struct target_id_for_type_home_enodeb_t {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
  uint32_t spare1 : 4;
  uint32_t home_enodeb_id : 28;
  uint16_t tracking_area_code;
} target_id_for_type_home_enodeb_t;

typedef struct target_id_for_type_extended_macro_enodeb_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2_ : 4;
  uint8_t mnc_digit_1 : 4;
  uint32_t smenb_ : 1;
  uint32_t spare1 : 2;
  uint32_t extended_macro_enodeb_id : 21;
  uint16_t tracking_area_code;
} target_id_for_type_extended_macro_enodeb_t;

typedef struct target_id_for_type_gnode_id_t {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2_ : 4;
  uint8_t mnc_digit_1 : 4;
  uint8_t spare : 2;
  uint8_t gnodeb_id_length : 6;
  uint32_t gnodeb_id;
  uint8_t five_gs_tracking_area_code[3];
} target_id_for_type_gnode_id;

typedef struct target_id_for_type_macro_ng_enodeb_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2_ : 4;
  uint8_t mnc_digit_1 : 4;
  uint8_t spare1 : 4;
  uint8_t macro_enodeb_id : 4;
  uint16_t macro_ng_enodeb_id;
  uint32_t five_gs_tracking_area_code;
} target_id_for_type_macro_ng_enodeb_t;

typedef struct target_id_for_type_extended_macro_ng_enodeb_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2_ : 4;
  uint8_t mnc_digit_1 : 4;
  uint8_t smenb_ : 1;
  uint8_t spare : 2;
  uint8_t extended_macro_enodeb_id : 5;
  uint16_t extended_macro_ng_enodeb_id;
  uint8_t five_gs_tracking_area_code[3];
} target_id_for_type_extended_macro_ng_enodeb_t;

typedef struct target_identification_s {
  uint8_t target_type;
  union {
    target_id_for_type_rnc_id_t rnc_id;
    target_id_for_type_macro_enodeb_t macro_enodeb_id;
    target_id_for_type_cell_identifier_t cell_identifier;
    target_id_for_type_home_enodeb_t home_enodeb_id;
    target_id_for_type_extended_macro_enodeb_t extended_macro_enodeb_id;
    target_id_for_type_gnode_id_t gnodeb_id;
    target_id_for_type_macro_ng_enodeb_t macro_ng_enodeb_id;
    target_id_for_type_extended_macro_ng_enodeb_t extended_ng_enodeb_id;
  } target_id;
} target_identification_t;

//-------------------------------------
// 8.53 Packet Flow ID
typedef struct packet_flow_id_s {
  uint8_t spare1 : 4;
  uint8_t ebi : 4;
  char* packet_flow_id;
} packet_flow_id_t;

//-------------------------------------
// 8.56 Port Number
typedef struct port_number_s {
  uint16_t port_number;
} port_number_t;

//-------------------------------------
// 8.57 APN Restriction
typedef struct access_point_name_restriction_s {
  uint8_t restriction_type_value;
} access_point_name_restriction_t;
typedef access_point_name_restriction_t apn_restriction_t;

//-------------------------------------
// 8.58 Selection Mode
enum selection_mode_e {
  SELECTION_MODE_E_MS_OR_NETWORK_PROVIDED_APN_SUBSCRIPTION_VERIFIED = 0,
  SELECTION_MODE_E_MS_PROVIDED_APN_SUBSCRIPTION_NOT_VERIFIED        = 1,
  SELECTION_MODE_E_NETWORK_PROVIDED_APN_SUBSCRIPTION_NOT_VERIFIED   = 2,
  SELECTION_MODE_E_FOR_FUTURE_USE                                   = 3,
};
typedef struct selection_mode_s {
  uint8_t spare1 : 6;
  uint8_t selec_mode : 2;
} selection_mode_t;

//-------------------------------------
// 8.62 Fully qualified PDN Connection Set Identifier (FQ-CSID)

enum node_id_type_e {
  GLOBAL_UNICAST_IPv4 = 0,
  GLOBAL_UNICAST_IPv6 = 1,
  TYPE_EXOTIC = 2,  ///< (MCC * 1000 + MNC) << 12 + Integer value assigned to
                    ///< MME by operator
};

// Values of Number of CSID other than 1 are only employed in the Delete PDN
// Connection Set Request.
typedef struct fq_csid_s {
  struct fq_csid_ie_hdr_t {
    uint8_t node_id_type : 4;
    uint8_t number_of_csids : 4;
    union {
      struct in_addr unicast_ipv4;
      struct in6_addr unicast_ipv6;
      struct {
        uint16_t mcc;
        uint16_t mnc;
        uint16_t operator_specific_id;
      } exotic;
    } node_id;
  } fq_csid_ie_hdr;
#define PDN_CONNECTION_SET_IDENTIFIER_MAX 15
  uint16_t pdn_connection_set_identifier[PDN_CONNECTION_SET_IDENTIFIER_MAX];
} fq_csid_t;

//-------------------------------------
// 8.65 Node Type
enum node_type_e { NODE_TYPE_E_MME = 0, NODE_TYPE_E_SGSN = 1 };

typedef struct node_type_s {
  uint8_t node_type;
} node_type_t;
//-------------------------------------
// 8.66 Fully Qualified Domain Name (FQDN)
typedef struct fully_qualified_domain_name_s {
  std::string fqdn;
} fully_qualified_domain_name_t;

typedef fully_qualified_domain_name_t fqdn_t;

//-------------------------------------
// 8.67 Private Extension
typedef struct private_extension_s {
  uint16_t enterprise_id;
  std::string proprietary_value;
} private_extension_t;

//-------------------------------------
// 8.68 Transaction Identifier (TI)
typedef struct transaction_identifier_s {
  uint8_t transaction_identifier;
} transaction_identifier_t;

//-------------------------------------
// 8.75 User CSG Information (UCI)
#define ACCESS_MODE_E_CLOSED_MODE (0)
#define ACCESS_MODE_E_HYBRID_MODE (1)
#define ACCESS_MODE_E_RESERVED2 (2)
#define ACCESS_MODE_E_RESERVED3 (3)

typedef struct user_csg_information_s {
  uint8_t mcc_digit_2 : 4;
  uint8_t mcc_digit_1 : 4;
  uint8_t mnc_digit_3 : 4;
  uint8_t mcc_digit_3 : 4;
  uint8_t mnc_digit_2 : 4;
  uint8_t mnc_digit_1 : 4;
  uint32_t spare1 : 5;
  uint32_t csg_id : 27;
  uint8_t access_mode : 2;
  uint8_t spare2 : 4;
  uint8_t lcsg : 1;
  uint8_t cmi : 1;
} user_csg_information_t;
typedef user_csg_information_t uci_t;

//-------------------------------------
// 8.76 CSG Information Reporting Action
typedef struct csg_reporting_action_s {
  uint8_t spare1 : 5;
  uint8_t uciuhc : 1;
  uint8_t ucishc : 1;
  uint8_t ucicsg : 1;
} csg_reporting_action_t;

//-------------------------------------
// 8.77 RFSP Index
typedef struct rfsp_index_s {
  uint8_t rfsp_index;
} rfsp_index_t;

//-------------------------------------
// 8.78 CSG ID
typedef struct csg_id_s {
  uint32_t spare1 : 5;
  uint32_t csg_id : 27;
} csg_id_t;

//-------------------------------------
// 8.79 CSG Membership Indication (CMI)
#define CSG_MEMBERSHIP_INDICATION_E_NON_CSG_MEMBERSHIP (0)
#define CSG_MEMBERSHIP_INDICATION_E_CSG_MEMBERSHIP (1)
typedef struct csg_membership_indication_s {
  uint8_t spare1 : 7;
  uint8_t cmi : 1;
} csg_membership_indication_t;

//-------------------------------------
// 8.80 Service indicator
#define SERVICE_INDICATOR_E_CS_CALL (1)
#define SERVICE_INDICATOR_E_SMS (2)
typedef struct service_indicator_s {
  uint8_t service_indicator;
} service_indicator_t;

//-------------------------------------
// 8.81 Detach Type
#define DETACH_TYPE_E_PS (1)
#define DETACH_TYPE_E_COMBINED_PS_CS (2)
typedef struct detach_type_s {
  uint8_t detach_type;
} detach_type_t;

//-------------------------------------
// 8.82 Local Distinguished Name (LDN)
typedef struct local_distinguished_name_s {
  std::string ldn;
} local_distinguished_name_t;

//-------------------------------------
// 8.83 Node Features
enum support_features_e {
  SUPPORT_FEATURES_E_PRN  = 1,
  SUPPORT_FEATURES_E_MABR = 2,
  SUPPORT_FEATURES_E_NTSR = 4,
  SUPPORT_FEATURES_E_CIOT = 8,
  SUPPORT_FEATURES_E_S1UN = 16,
};

typedef struct node_features_s {
  uint8_t prn : 1;
  uint8_t mabr : 1;
  uint8_t ntsr : 1;
  uint8_t ciot : 1;
  uint8_t s1un : 1;
  uint8_t spare : 3;
} node_features_t;

//-------------------------------------
// 8.85 Throttling
enum throttling_unit_e {
  THROTTLING_UNIT_E_SECONDS_2   = 0,
  THROTTLING_UNIT_E_MINUTES_1   = 1,
  THROTTLING_UNIT_E_MINUTES_10  = 2,
  THROTTLING_UNIT_E_HOURS_1     = 3,
  THROTTLING_UNIT_E_HOURS_10    = 4,
  THROTTLING_UNIT_E_DEACTIVATED = 7,
};

typedef struct throttling_s {
  uint8_t throttling_delay_unit : 3;
  uint8_t throttling_delay_value : 5;
  uint8_t throttling_factor;
} throttling_t;

//-------------------------------------
// 8.86 Allocation/Retention Priority (ARP)
typedef struct allocation_retention_priority_s {
  uint8_t spare1 : 1;
  uint8_t pci : 1;
  uint8_t pl : 4;
  uint8_t spare2 : 1;
  uint8_t pvi : 1;
} allocation_retention_priority_t;
typedef allocation_retention_priority_t arp_t;

//-------------------------------------
// 8.87 EPC Timer
enum timer_unit_e {
  TIMER_UNIT_E_SECONDS_2   = 0,
  TIMER_UNIT_E_MINUTES_1   = 1,
  TIMER_UNIT_E_MINUTES_10  = 2,
  TIMER_UNIT_E_HOURS_1     = 3,
  TIMER_UNIT_E_HOURS_10    = 4,
  TIMER_UNIT_E_DEACTIVATED = 7,
};

typedef struct epc_timer_s {
  uint8_t timer_unit : 3;
  uint8_t timer_value : 5;
} epc_timer_t;

//-------------------------------------
// 8.88 Signalling Priority Indication
typedef struct signalling_priority_indication_s {
  uint8_t spare1 : 7;
  uint8_t lapi : 1;
} signalling_priority_indication_t;

//-------------------------------------
// 8.90 Additional MM context for SRVCC
typedef struct additional_mm_context_for_srvcc_s {
  uint8_t length_of_the_mobile_station_classmark_2;
  std::string mobile_station_classmark_2;
  uint8_t length_of_the_mobile_station_classmark_3;
  std::string mobile_station_classmark_3;
  uint8_t length_of_the_supported_codec_list;
  std::string supported_codec_list;
} additional_mm_context_for_srvcc_t;

//-------------------------------------
// 8.91 Additional flags for SRVCC
typedef struct additional_flags_for_srvcc_s {
  uint8_t spare1 : 6;
  uint8_t vf : 1;
  uint8_t ics : 1;
} additional_flags_for_srvcc_t;

//-------------------------------------
// 8.94 Additional Protocol Configuration Options (APCO)
typedef struct additional_protocol_configuration_options_s {
  std::string additional_protocol_configuration_options;
} additional_protocol_configuration_options_t;

//-------------------------------------
// 8.96 H(e)NB Information Reporting
typedef struct h_e_nb_information_reporting_s {
  uint8_t spare1 : 7;
  uint8_t fti : 1;
} h_e_nb_information_reporting_t;

//-------------------------------------
// 8.97 IPv4 Configuration Parameters (IP4CP)
typedef struct ipv4_configuration_parameters_s {
  uint8_t subnet_prefix_length;
  uint32_t ipv4_default_router_address;
} ipv4_configuration_parameters_t;

//-------------------------------------
// 8.98 Change to Report Flags
typedef struct change_to_report_flags_s {
  uint8_t spare1 : 6;
  uint8_t tzcr : 1;
  uint8_t sncr : 1;
} change_to_report_flags_t;

//-------------------------------------
// 8.99 Action Indication
enum indication_e {
  INDICATION_E_NO_ACTION    = 0,
  INDICATION_E_DEACTIVATION = 1,
  INDICATION_E_PAGING       = 2,
  INDICATION_E_PAGING_STOP  = 3,
};
typedef struct action_indication_ie_t {
  uint8_t spare1 : 5;
  uint8_t indication : 3;
} action_indication_ie;

//-------------------------------------
// 8.100 TWAN Identifier
#define RELAY_IDENTITY_TYPE_E_IPV4_OR_IPV6_ADDRESS (0)
#define RELAY_IDENTITY_TYPE_E_FQDN (1)
typedef struct twan_identifier_ie_t {
  uint8_t spare1 : 3;
  uint8_t laii : 1;
  uint8_t opnai : 1;
  uint8_t plmni : 1;
  uint8_t civai : 1;
  uint8_t bssidi : 2;
  uint8_t ssid_length;
  std::string ssid;
  uint8_t bssid[7];
  uint8_t civic_address_length;
  std::string civic_address_information;
  uint8_t twan_plmn_id_[3];
  uint8_t twan_operator_name_length;
  std::string twan_operator_name;
  uint8_t relay_identity_type;
  uint8_t relay_identity_length;
  std::string relay_identity;
  uint8_t circuit_id_length;
  std::string circuit_id;
} twan_identifier_ie;

//-------------------------------------
// 8.101 ULI Timestamp
typedef struct uli_timestamp_s {
  uint32_t uli_timestamp_value;
} uli_timestamp_t;

//-------------------------------------
// 8.103 RAN/NAS Cause
enum protocol_type_e {
  PROTOCOL_TYPE_E_S1AP     = 0,
  PROTOCOL_TYPE_E_EMM      = 1,
  PROTOCOL_TYPE_E_ESM      = 2,
  PROTOCOL_TYPE_E_DIAMETER = 3,
  PROTOCOL_TYPE_E_IKEV2    = 4,
};
enum ran_nas_cause_type_e {
  RAN_NAS_CAUSE_TYPE_E_RADIO_NETWORK_LAYER = 0,
  RAN_NAS_CAUSE_TYPE_E_TRANSPORT_LAYER     = 1,
  RAN_NAS_CAUSE_TYPE_E_NAS                 = 2,
  RAN_NAS_CAUSE_TYPE_E_PROTOCOL            = 3,
  RAN_NAS_CAUSE_TYPE_E_MISCELLANEOUS       = 4,
};
typedef struct ran_nas_cause_s {
  uint8_t protocol_type : 4;
  uint8_t cause_type : 4;
  union {
    uint16_t s1ap;  // TODO
    uint8_t emm;
    uint8_t esm;
    uint16_t diameter;
    uint16_t ikev2;
  } cause_value;
} ran_nas_cause_t;

//-------------------------------------
// 8.104 CN Operator Selection Entity
#define SELECTION_ENTITY_E_UE_SELECTED (0)
#define SELECTION_ENTITY_E_NETWORK_SELECTED (1)
typedef struct cn_operator_selection_entity_s {
  uint8_t spare1 : 6;
  uint8_t selection_entity : 2;
} cn_operator_selection_entity_t;

//-------------------------------------
// 8.105 Trusted WLAN Mode Indication
typedef struct trusted_wlan_mode_indication_s {
  uint8_t spare1 : 6;
  uint8_t mcm : 2;
  uint8_t scm : 1;
} trusted_wlan_mode_indication_t;

//-------------------------------------
// 8.108 Presence Reporting Area Action
#define PRA_ACTION_E_START_REPORTING_CHANGES_OF_UE_PRESENCE_IN_PRA (1)
#define PRA_ACTION_E_STOP_REPORTING_CHANGES_OF_UE_PRESENCE_IN_PRA (2)
#define PRA_ACTION_E_MODIFY_ELEMENTS_COMPOSING_PRA (3)
typedef struct presence_reporting_area_action_s {
  struct presence_reporting_area_action_hdr_s {
    uint8_t spare1 : 4;
    uint8_t inapra : 1;
    uint8_t action : 3;
    uint8_t presence_reporting_area_identifier[3];
    uint8_t number_of_tai : 4;
    uint8_t number_of_rai : 4;
    uint8_t spare2 : 2;
    uint8_t number_of_macro_enodeb : 6;
    uint8_t spare3 : 2;
    uint8_t number_of_home_enodeb : 6;
    uint8_t spare4 : 2;
    uint8_t number_of_ecgi : 6;
    uint8_t spare5 : 2;
    uint8_t number_of_sai : 6;
    uint8_t spare6 : 2;
    uint8_t number_of_cgi : 6;
  } presence_reporting_area_action_hdr;
  char** tais;
  char** macro_enb_ids;
  char** home_enb_ids;
  char** ecgis;
  char** rais;
  char** sais;
  char** cgis;
  uint8_t spare7 : 2;
  uint8_t number_of_extended_macro_enodeb : 6;
  char** extended_macro_enb_ids;
} presence_reporting_area_action_t;

//-------------------------------------
// 8.109 Presence Reporting Area Information
typedef struct presence_reporting_area_information_s {
  uint8_t pra_identifier[3];
  uint8_t spare1 : 4;
  uint8_t inapra : 2;
  uint8_t apra : 1;
  uint8_t opra : 1;
  uint8_t ipra : 1;
} presence_reporting_area_information_t;

//-------------------------------------
// 8.110 TWAN Identifier Timestamp
typedef struct twan_identifier_timestamp_s {
  uint32_t twan_identifier_timestamp_value;
} twan_identifier_timestamp_t;

//-------------------------------------
// 8.113 Metric
typedef struct metric_ie_s {
  uint8_t metric;
} metric_t;

//-------------------------------------
// 8.114 Sequence Number
typedef struct sequence_number_s {
  uint32_t sequence_number;
} sequence_number_t;

//-------------------------------------
// 8.115 APN and Relative Capacity
typedef struct apn_and_relative_capacity_s {
  uint8_t relative_capacity;
  uint8_t apn_length;
  std::string access_point_name;
} apn_and_relative_capacity_t;

//-------------------------------------
// 8.116 WLAN Offloadability Indication
typedef struct wlan_offloadability_indication_s {
  uint8_t spare1 : 6;
  uint8_t eutran_indication : 1;
  uint8_t utran_indication_ : 1;
} wlan_offloadability_indication_t;

//-------------------------------------
// 8.117 Paging and Service Information
typedef struct paging_and_service_information_s {
  uint8_t spare1 : 4;
  uint8_t epc_bearer_id : 4;
  uint8_t spare2 : 7;
  uint8_t ppi : 1;
  uint8_t spare3 : 2;
  uint8_t paging_policy_indication_value : 6;
} paging_and_service_information_t;

//-------------------------------------
// 8.118 Integer Number
typedef struct integer_number_s {
  union {
    uint8_t i8;
    uint16_t i16;
    uint32_t i32;
  } u;
  uint8_t length;
} integer_number_t;

//-------------------------------------
// 8.119 Millisecond Time Stamp
typedef struct millisecond_time_stamp_s {
  uint8_t millisecond_time_stamp_value[6];
} millisecond_time_stamp_t;

//-------------------------------------
// 8.123 Remote User ID
typedef struct remote_user_id_s {
  uint8_t spare1 : 6;
  uint8_t imeif : 1;
  uint8_t msisdnf : 1;
  uint8_t length_of_imsi;
  std::string imsi;
  uint8_t length_of_msisdn;
  std::string msisdn;
  uint8_t length_of_imei;
  std::string imei;
} remote_user_id_t;

//-------------------------------------
// 8.124 Remote UE IP Information
typedef struct remote_ue_ip_information_ie_t {
  std::string remote_ue_ip_information;
} remote_ue_ip_information_ie;

//-------------------------------------
// 8.125 CIoT Optimizations Support Indication
typedef struct ciot_optimizations_support_indication_s {
  uint8_t spare1 : 4;
  uint8_t ihcsi : 1;
  uint8_t awopdn : 1;
  uint8_t scnipdn : 1;
  uint8_t sgnipdn : 1;
} ciot_optimizations_support_indication_t;

//-------------------------------------
// 8.128 Extended Protocol Configuration Options (ePCO)
typedef struct extended_protocol_configuration_options_s {
  std::string extended_protocol_configuration_options;
} extended_protocol_configuration_options_t;

//-------------------------------------
// 8.129 Serving PLMN Rate Control
typedef struct serving_plmn_rate_control_s {
  uint32_t uplink_rate_limit;
  uint32_t downlink_rate_limit;
} serving_plmn_rate_control_t;

//-------------------------------------
// 8.130 Counter
typedef struct counter_s {
  uint32_t timestamp_value;
  uint8_t counter_value;
} counter_t;

//-------------------------------------
// 8.133 UP Function Selection Indication Flags
typedef struct up_function_selection_indication_flags_s {
  uint8_t spare1 : 7;
  uint8_t dcnr : 1;
} up_function_selection_indication_flags_t;

//-------------------------------------
// 8.134 Maximum Packet Loss Rate
typedef struct maximum_packet_loss_rate_s {
  uint8_t spare1 : 6;
  uint8_t dl : 1;
  uint8_t ul : 1;
  uint16_t maximum_packet_loss_rate_ul;
  uint16_t maximum_packet_loss_rate_dl;
} maximum_packet_loss_rate_t;

//-------------------------------------
// 8.135 APN Rate Control Status
// 8.136 Extended Trace Information

namespace std {

template<>
class hash<fteid_t> {
 public:
  size_t operator()(const fteid_t& k) const {
    using std::hash;
    using std::size_t;
    std::size_t h1 = std::hash<uint32_t>()(k.interface_type);
    std::size_t h2 = std::hash<uint32_t>()(k.teid_gre_key) ^ h1;

    if (k.v4) {
      h2 ^= std::hash<uint32_t>()(k.ipv4_address.s_addr);
    }
    if (k.v6) {
      h2 ^= std::hash<uint32_t>()(k.ipv6_address.s6_addr32[0]);
      h2 ^= std::hash<uint32_t>()(k.ipv6_address.s6_addr32[1]);
      h2 ^= std::hash<uint32_t>()(k.ipv6_address.s6_addr32[2]);
      h2 ^= std::hash<uint32_t>()(k.ipv6_address.s6_addr32[3]);
    }
    return h2;
  }
};
}  // namespace std
#endif /* FILE_3GPP_29_274_SEEN */
