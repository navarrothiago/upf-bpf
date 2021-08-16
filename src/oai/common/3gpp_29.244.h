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

/*! \file 3gpp_129.244.h
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_3GPP_129_244_H_SEEN
#define FILE_3GPP_129_244_H_SEEN
#include "3gpp_29.274.h"
#include "common_root_types.h"
#include "conversions.hpp"
#include "logger.hpp"  // for fmt::format in spdlog

#include <arpa/inet.h>
#include <stdint.h>
#include <string>
#include <vector>

namespace pfcp {

struct pfcp_exception : public std::exception {
  pfcp_exception() throw() {
    cause = 0;
    phrase.assign("PFCP Exception unknown cause");
  }

  pfcp_exception(int acause) throw() {
    cause  = acause;
    phrase = fmt::format("PFCP Exception cause {}", cause);
  }
  const char* what() const throw() { return phrase.c_str(); }

 public:
  int cause;
  std::string phrase;
};

struct pfcp_msg_bad_length_exception : public pfcp_exception {
 public:
  pfcp_msg_bad_length_exception(
      const uint8_t msg_type, const uint16_t hdr_size, const uint16_t ie_size,
      const uint16_t check_ie_size, const char* file, const int line) throw() {
    phrase = fmt::format(
        "PFCP msg {} Bad Length hdr.length {}/ sum ie {} / check sum ie {} "
        "Exception {}:{}",
        msg_type, hdr_size, ie_size, check_ie_size, file, line);
  }
  pfcp_msg_bad_length_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~pfcp_msg_bad_length_exception() throw() {}
};

struct pfcp_msg_unimplemented_ie_exception : public pfcp_exception {
 public:
  pfcp_msg_unimplemented_ie_exception(
      const uint8_t msg_type, const uint16_t ie_type,
      const uint8_t instance = 0) throw() {
    phrase = fmt::format(
        "PFCP msg {} Unimplemented {} IE Instance {} Exception", msg_type,
        ie_type, instance);
  }
  pfcp_msg_unimplemented_ie_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~pfcp_msg_unimplemented_ie_exception() throw() {}
};

struct pfcp_msg_illegal_ie_exception : public pfcp_exception {
 public:
  pfcp_msg_illegal_ie_exception(
      const uint8_t msg_type, const uint16_t ie_type, const char* file,
      const int line) throw() {
    phrase = fmt::format(
        "PFCP msg {} Illegal IE {} Exception {}:{}", msg_type, ie_type, file,
        line);
  }
  pfcp_msg_illegal_ie_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~pfcp_msg_illegal_ie_exception() throw() {}
};

struct pfcp_ie_exception : public pfcp_exception {
 public:
  pfcp_ie_exception(uint16_t ie_type) throw() {
    phrase = fmt::format("PFCP IE {} Exception", ie_type);
  }
  pfcp_ie_exception(std::string& aphrase) throw() { phrase = aphrase; }
  virtual ~pfcp_ie_exception() throw() {}
};

struct pfcp_ie_unimplemented_exception : public pfcp_ie_exception {
 public:
  pfcp_ie_unimplemented_exception(uint16_t ie_type) throw()
      : pfcp_ie_exception(ie_type) {
    phrase = fmt::format("PFCP IE {} Unimplemented Exception", ie_type);
  }
  virtual ~pfcp_ie_unimplemented_exception() throw() {}
};

struct pfcp_tlv_exception : public pfcp_ie_exception {
 public:
  pfcp_tlv_exception(uint16_t ie_type) throw() : pfcp_ie_exception(ie_type) {
    phrase = fmt::format("PFCP IE TLV {} Exception", ie_type);
  }
  virtual ~pfcp_tlv_exception() throw() {}
};

struct pfcp_tlv_bad_length_exception : public pfcp_tlv_exception {
 public:
  pfcp_tlv_bad_length_exception(
      uint16_t ie_type, uint16_t ie_length, const char* file,
      const int line) throw()
      : pfcp_tlv_exception(ie_type) {
    phrase = fmt::format(
        "PFCP IE TLV {} Bad Length {} Exception {}:{}", ie_type, ie_length,
        file, line);
  }
  virtual ~pfcp_tlv_bad_length_exception() throw() {}
};

struct pfcp_ie_value_exception : public pfcp_ie_exception {
 public:
  pfcp_ie_value_exception(uint16_t ie_type, const char* field) throw()
      : pfcp_ie_exception(ie_type) {
    phrase =
        fmt::format("PFCP IE {} Bad Value of {} Exception", ie_type, field);
  }
  virtual ~pfcp_ie_value_exception() throw() {}
};

#define PFCP_IE_CREATE_PDR (1)
#define PFCP_IE_PDI (2)
#define PFCP_IE_CREATE_FAR (3)
#define PFCP_IE_FORWARDING_PARAMETERS (4)
#define PFCP_IE_DUPLICATING_PARAMETERS (5)
#define PFCP_IE_CREATE_URR (6)
#define PFCP_IE_CREATE_QER (7)
#define PFCP_IE_CREATED_PDR (8)
#define PFCP_IE_UPDATE_PDR (9)
#define PFCP_IE_UPDATE_FAR (10)
#define PFCP_IE_UPDATE_FORWARDING_PARAMETERS (11)
#define PFCP_IE_UPDATE_BAR_WITHIN_PFCP_SESSION_REPORT_RESPONSE (12)
#define PFCP_IE_UPDATE_URR (13)
#define PFCP_IE_UPDATE_QER (14)
#define PFCP_IE_REMOVE_PDR (15)
#define PFCP_IE_REMOVE_FAR (16)
#define PFCP_IE_REMOVE_URR (17)
#define PFCP_IE_REMOVE_QER (18)
#define PFCP_IE_CAUSE (19)
#define PFCP_IE_SOURCE_INTERFACE (20)
#define PFCP_IE_F_TEID (21)
#define PFCP_IE_NETWORK_INSTANCE (22)
#define PFCP_IE_SDF_FILTER (23)
#define PFCP_IE_APPLICATION_ID (24)
#define PFCP_IE_GATE_STATUS (25)
#define PFCP_IE_MBR (26)
#define PFCP_IE_GBR (27)
#define PFCP_IE_QER_CORRELATION_ID (28)
#define PFCP_IE_PRECEDENCE (29)
#define PFCP_IE_TRANSPORT_LEVEL_MARKING (30)
#define PFCP_IE_VOLUME_THRESHOLD (31)
#define PFCP_IE_TIME_THRESHOLD (32)
#define PFCP_IE_MONITORING_TIME (33)
#define PFCP_IE_SUBSEQUENT_VOLUME_THRESHOLD (34)
#define PFCP_IE_SUBSEQUENT_TIME_THRESHOLD (35)
#define PFCP_IE_INACTIVITY_DETECTION_TIME (36)
#define PFCP_IE_REPORTING_TRIGGERS (37)
#define PFCP_IE_REDIRECT_INFORMATION (38)
#define PFCP_IE_REPORT_TYPE (39)
#define PFCP_IE_OFFENDING_IE (40)
#define PFCP_IE_FORWARDING_POLICY (41)
#define PFCP_IE_DESTINATION_INTERFACE (42)
#define PFCP_IE_UP_FUNCTION_FEATURES (43)
#define PFCP_IE_APPLY_ACTION (44)
#define PFCP_IE_DOWNLINK_DATA_SERVICE_INFORMATION (45)
#define PFCP_IE_DOWNLINK_DATA_NOTIFICATION_DELAY (46)
#define PFCP_IE_DL_BUFFERING_DURATION (47)
#define PFCP_IE_DL_BUFFERING_SUGGESTED_PACKET_COUNT (48)
#define PFCP_IE_PFCPSMREQ_FLAGS (49)
#define PFCP_IE_PFCPSRRSP_FLAGS (50)
#define PFCP_IE_LOAD_CONTROL_INFORMATION (51)
#define PFCP_IE_SEQUENCE_NUMBER (52)
#define PFCP_IE_METRIC (53)
#define PFCP_IE_OVERLOAD_CONTROL_INFORMATION (54)
#define PFCP_IE_TIMER (55)
#define PFCP_IE_PACKET_DETECTION_RULE_ID (56)
#define PFCP_IE_F_SEID (57)
#define PFCP_IE_APPLICATION_IDS_PFDS (58)
#define PFCP_IE_PFD (59)
#define PFCP_IE_NODE_ID (60)
#define PFCP_IE_PFD_CONTENTS (61)
#define PFCP_IE_MEASUREMENT_METHOD (62)
#define PFCP_IE_USAGE_REPORT_TRIGGER (63)
#define PFCP_IE_MEASUREMENT_PERIOD (64)
#define PFCP_IE_FQ_CSID (65)
#define PFCP_IE_VOLUME_MEASUREMENT (66)
#define PFCP_IE_DURATION_MEASUREMENT (67)
#define PFCP_IE_APPLICATION_DETECTION_INFORMATION (68)
#define PFCP_IE_TIME_OF_FIRST_PACKET (69)
#define PFCP_IE_TIME_OF_LAST_PACKET (70)
#define PFCP_IE_QUOTA_HOLDING_TIME (71)
#define PFCP_IE_DROPPED_DL_TRAFFIC_THRESHOLD (72)
#define PFCP_IE_VOLUME_QUOTA (73)
#define PFCP_IE_TIME_QUOTA (74)
#define PFCP_IE_START_TIME (75)
#define PFCP_IE_END_TIME (76)
#define PFCP_IE_QUERY_URR (77)
#define PFCP_IE_USAGE_REPORT_WITHIN_SESSION_MODIFICATION_RESPONSE (78)
#define PFCP_IE_USAGE_REPORT_WITHIN_SESSION_DELETION_RESPONSE (79)
#define PFCP_IE_USAGE_REPORT_WITHIN_SESSION_REPORT_REQUEST (80)
#define PFCP_IE_URR_ID (81)
#define PFCP_IE_LINKED_URR_ID (82)
#define PFCP_IE_DOWNLINK_DATA_REPORT (83)
#define PFCP_IE_OUTER_HEADER_CREATION (84)
#define PFCP_IE_CREATE_BAR (85)
#define PFCP_IE_UPDATE_BAR_WITHIN_PFCP_SESSION_MODIFICATION_REQUEST (86)
#define PFCP_IE_REMOVE_BAR (87)
#define PFCP_IE_BAR_ID (88)
#define PFCP_IE_CP_FUNCTION_FEATURES (89)
#define PFCP_IE_USAGE_INFORMATION (90)
#define PFCP_IE_APPLICATION_INSTANCE_ID (91)
#define PFCP_IE_FLOW_INFORMATION (92)
#define PFCP_IE_UE_IP_ADDRESS (93)
#define PFCP_IE_PACKET_RATE (94)
#define PFCP_IE_OUTER_HEADER_REMOVAL (95)
#define PFCP_IE_RECOVERY_TIME_STAMP (96)
#define PFCP_IE_DL_FLOW_LEVEL_MARKING (97)
#define PFCP_IE_HEADER_ENRICHMENT (98)
#define PFCP_IE_ERROR_INDICATION_REPORT (99)
#define PFCP_IE_MEASUREMENT_INFORMATION (100)
#define PFCP_IE_NODE_REPORT_TYPE (101)
#define PFCP_IE_USER_PLANE_PATH_FAILURE_REPORT (102)
#define PFCP_IE_REMOTE_GTP_U_PEER (103)
#define PFCP_IE_UR_SEQN (104)
#define PFCP_IE_UPDATE_DUPLICATING_PARAMETERS (105)
#define PFCP_IE_ACTIVATE_PREDEFINED_RULES (106)
#define PFCP_IE_DEACTIVATE_PREDEFINED_RULES (107)
#define PFCP_IE_FAR_ID (108)
#define PFCP_IE_QER_ID (109)
#define PFCP_IE_OCI_FLAGS (110)
#define PFCP_IE_PFCP_ASSOCIATION_RELEASE_REQUEST (111)
#define PFCP_IE_GRACEFUL_RELEASE_PERIOD (112)
#define PFCP_IE_PDN_TYPE (113)
#define PFCP_IE_FAILED_RULE_ID (114)
#define PFCP_IE_TIME_QUOTA_MECHANISM (115)
#define PFCP_IE_USER_PLANE_IP_RESOURCE_INFORMATION (116)
#define PFCP_IE_USER_PLANE_INACTIVITY_TIMER (117)
#define PFCP_IE_AGGREGATED_URRS (118)
#define PFCP_IE_MULTIPLIER (119)
#define PFCP_IE_AGGREGATED_URR_ID (120)
#define PFCP_IE_SUBSEQUENT_VOLUME_QUOTA (121)
#define PFCP_IE_SUBSEQUENT_TIME_QUOTA (122)
#define PFCP_IE_RQI (123)
#define PFCP_IE_QFI (124)
#define PFCP_IE_QUERY_URR_REFERENCE (125)
#define PFCP_IE_ADDITIONAL_USAGE_REPORTS_INFORMATION (126)
#define PFCP_IE_CREATE_TRAFFIC_ENDPOINT (127)
#define PFCP_IE_CREATED_TRAFFIC_ENDPOINT (128)
#define PFCP_IE_UPDATE_TRAFFIC_ENDPOINT (129)
#define PFCP_IE_REMOVE_TRAFFIC_ENDPOINT (130)
#define PFCP_IE_TRAFFIC_ENDPOINT_ID (131)
#define PFCP_IE_ETHERNET_PACKET_FILTER (132)
#define PFCP_IE_MAC_ADDRESS (133)
#define PFCP_IE_C_TAG (134)
#define PFCP_IE_S_TAG (135)
#define PFCP_IE_ETHERTYPE (136)
#define PFCP_IE_PROXYING (137)
#define PFCP_IE_ETHERNET_FILTER_ID (138)
#define PFCP_IE_ETHERNET_FILTER_PROPERTIES (139)
#define PFCP_IE_SUGGESTED_BUFFERING_PACKETS_COUNT (140)
#define PFCP_IE_USER_ID (141)
#define PFCP_IE_ETHERNET_PDU_SESSION_INFORMATION (142)
#define PFCP_IE_ETHERNET_TRAFFIC_INFORMATION (143)
#define PFCP_IE_MAC_ADDRESSES_DETECTED (144)
#define PFCP_IE_MAC_ADDRESSES_REMOVED (145)
#define PFCP_IE_ETHERNET_INACTIVITY_TIMER (146)
#define PFCP_IE_ADDITIONAL_MONITORING_TIME (147)
#define PFCP_IE_EVENT_INFORMATION (148)
#define PFCP_IE_EVENT_REPORTING (149)
#define PFCP_IE_EVENT_ID (150)
#define PFCP_IE_EVENT_THRESHOLD (151)
#define PFCP_IE_TRACE_INFORMATION (152)
#define PFCP_IE_FRAMED_ROUTE (153)
#define PFCP_IE_FRAMED_ROUTING (154)
#define PFCP_IE_FRAMED_IPV6_ROUTE (155)

#define PFCP_MESSAGE_RESERVED (0)
// PFCP_NODE_RELATED_MESSAGES
#define PFCP_HEARTBEAT_REQUEST (1)
#define PFCP_HEARTBEAT_RESPONSE (2)
#define PFCP_PFCP_PFD_MANAGEMENT_REQUEST (3)
#define PFCP_PFCP_PFD_MANAGEMENT_RESPONSE (4)
#define PFCP_ASSOCIATION_SETUP_REQUEST (5)
#define PFCP_ASSOCIATION_SETUP_RESPONSE (6)
#define PFCP_ASSOCIATION_UPDATE_REQUEST (7)
#define PFCP_ASSOCIATION_UPDATE_RESPONSE (8)
#define PFCP_ASSOCIATION_RELEASE_REQUEST (9)
#define PFCP_ASSOCIATION_RELEASE_RESPONSE (10)
#define PFCP_VERSION_NOT_SUPPORTED_RESPONSE (11)
#define PFCP_NODE_REPORT_REQUEST (12)
#define PFCP_NODE_REPORT_RESPONSE (13)
#define PFCP_SESSION_SET_DELETION_REQUEST (14)
#define PFCP_SESSION_SET_DELETION_RESPONSE (15)
// PFCP_SESSION_RELATED_MESSAGES
#define PFCP_SESSION_ESTABLISHMENT_REQUEST (50)
#define PFCP_SESSION_ESTABLISHMENT_RESPONSE (51)
#define PFCP_SESSION_MODIFICATION_REQUEST (52)
#define PFCP_SESSION_MODIFICATION_RESPONSE (53)
#define PFCP_SESSION_DELETION_REQUEST (54)
#define PFCP_SESSION_DELETION_RESPONSE (55)
#define PFCP_SESSION_REPORT_REQUEST (56)
#define PFCP_SESSION_REPORT_RESPONSE (57)
}  // namespace pfcp

namespace pfcp {
//-------------------------------------
// 8.2.1 Cause
enum cause_value_e {
  /* Request / Initial message */
  CAUSE_VALUE_RESERVED                           = 0,
  CAUSE_VALUE_REQUEST_ACCEPTED                   = 1,
  CAUSE_VALUE_REQUEST_REJECTED                   = 64,
  CAUSE_VALUE_SESSION_CONTEXT_NOT_FOUND          = 65,
  CAUSE_VALUE_MANDATORY_IE_MISSING               = 66,
  CAUSE_VALUE_CONDITIONAL_IE_MISSING             = 67,
  CAUSE_VALUE_INVALID_LENGTH                     = 68,
  CAUSE_VALUE_MANDATORY_IE_INCORRECT             = 69,
  CAUSE_VALUE_INVALID_FORWARDING_POLICY          = 70,
  CAUSE_VALUE_INVALID_FTEID_ALLOCATION_OPTION    = 71,
  CAUSE_VALUE_NO_ESTABLISHED_PFCP_ASSOCIATION    = 72,
  CAUSE_VALUE_RULE_CREATION_MODIFICATION_FAILURE = 73,
  CAUSE_VALUE_PFCP_ENTITY_IN_CONGESTION          = 74,
  CAUSE_VALUE_NO_RESOURCES_AVAILABLE             = 75,
  CAUSE_VALUE_SERVICE_NOT_SUPPORTED              = 76,
  CAUSE_VALUE_SYSTEM_FAILURE                     = 77
};

typedef struct cause_s {
  uint8_t cause_value;

  bool operator==(const struct cause_s& i) const {
    return (i.cause_value == cause_value);
  };
  bool operator==(const uint8_t& cvalue) const {
    return (cvalue == cause_value);
  };
} cause_t;

//-------------------------------------
// 8.2.2 Source Interface
//  enum source_interface_value_e {
//    /* Request / Initial message */
//    ACCESS                        = 0,
//    CORE                          = 1,
//    SGI_LAN_N6_LAN                = 2,
//    CP_FUNCTION                   = 3
//  };
// see @  enum destination_interface_value_e

typedef struct source_interface_s {
  uint8_t interface_value;
  bool operator==(const struct source_interface_s& i) const {
    return (i.interface_value == interface_value);
  };
} source_interface_t;

//-------------------------------------
// 8.2.3 F-TEID
struct fteid_s {
  uint8_t chid : 1;
  uint8_t ch : 1;
  uint8_t v4 : 1;
  uint8_t v6 : 1;
  teid_t teid;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  uint8_t choose_id;

  bool operator==(const struct fteid_s& f) const {
    return (teid == f.teid) and
           (ipv4_address.s_addr == f.ipv4_address.s_addr) and
           (chid == f.chid) and (ch == f.ch) and (choose_id == f.choose_id) and
           (ipv6_address.s6_addr32[0] == f.ipv6_address.s6_addr32[0]) and
           (ipv6_address.s6_addr32[1] == f.ipv6_address.s6_addr32[1]) and
           (ipv6_address.s6_addr32[2] == f.ipv6_address.s6_addr32[2]) and
           (ipv6_address.s6_addr32[3] == f.ipv6_address.s6_addr32[3]) and
           (v4 == f.v4) and (v6 == f.v6);
  }
  bool is_zero() const { return ((!v4) and (!v6)); }
};

typedef struct fteid_s fteid_t;
//-------------------------------------
// 8.2.4 Network Instance
typedef struct network_instance_s {
  std::string network_instance;
} network_instance_t;

//-------------------------------------
// 8.2.5 SDF Filter
typedef struct sdf_filter_s {
  uint8_t spare : 3;
  uint8_t bid : 1;
  uint8_t fl : 1;
  uint8_t spi : 1;
  uint8_t ttc : 1;
  uint8_t fd : 1;
  uint16_t length_of_flow_description;
  std::string flow_description;
  std::string tos_traffic_class;         // 2 octets
  std::string security_parameter_index;  // 4 octets
  std::string flow_label;                // 3 octets
  uint32_t sdf_filter_id;
} sdf_filter_t;

//-------------------------------------
//  8.2.6 Application ID
typedef struct application_id_s {
  std::string application_id;
} application_id_t;

//-------------------------------------
//  8.2.7 Gate Status
enum gate_status_e {
  /* Request / Initial message */
  OPEN   = 0,
  CLOSED = 1
};
typedef struct gate_status_s {
  uint8_t ul_gate : 2;
  uint8_t dl_gate : 2;
} gate_status_t;

//-------------------------------------
// 8.2.8 MBR
typedef struct mbr_s {
  uint64_t ul_mbr;
  uint64_t dl_mbr;
} mbr_t;

//-------------------------------------
// 8.2.9 GBR
typedef struct gbr_s {
  uint64_t ul_gbr;
  uint64_t dl_gbr;
} gbr_t;

//-------------------------------------
// 8.2.10 QER Correlation ID
typedef struct qer_correlation_id_s {
  uint32_t qer_correlation_id;
} qer_correlation_id_t;

//-------------------------------------
// 8.2.11 Precedence
typedef struct precedence_s {
  uint32_t precedence;
  bool operator==(const struct precedence_s& i) const {
    return (i.precedence == precedence);
  };
} precedence_t;

//-------------------------------------
// 8.2.12 Transport Level Marking
typedef struct transport_level_marking_s {
  std::string transport_level_marking;  // 2 octets
} transport_level_marking_t;

//-------------------------------------
// 8.2.13 Volume Threshold
typedef struct volume_threshold_s {
  uint8_t tovol : 1;
  uint8_t ulvol : 1;
  uint8_t dlvol : 1;
  uint8_t spare : 5;
  uint64_t total_volume;
  uint64_t uplink_volume;
  uint64_t downlink_volume;
} volume_threshold_t;

//-------------------------------------
// 8.2.14 Time Threshold
typedef struct time_threshold_s {
  uint32_t time_threshold;
  bool operator==(const struct time_threshold_s& i) const {
    return (i.time_threshold == time_threshold);
  };
} time_threshold_t;

//-------------------------------------
// 8.2.15 Monitoring Time
typedef struct monitoring_time_s {
  uint32_t monitoring_time;
} monitoring_time_t;

//-------------------------------------
// 8.2.16 Subsequent Volume Threshold
typedef struct subsequent_volume_threshold_s {
  uint8_t tovol : 1;
  uint8_t ulvol : 1;
  uint8_t dlvol : 1;
  uint8_t spare : 5;
  uint64_t total_volume;
  uint64_t uplink_volume;
  uint64_t downlink_volume;
} subsequent_volume_threshold_t;

//-------------------------------------
// 8.2.17 Subsequent Time Threshold
typedef struct subsequent_time_threshold_s {
  uint32_t subsequent_time_threshold;
  bool operator==(const struct subsequent_time_threshold_s& i) const {
    return (i.subsequent_time_threshold == subsequent_time_threshold);
  };
} subsequent_time_threshold_t;

//-------------------------------------
// 8.2.18 Inactivity Detection Time
typedef struct inactivity_detection_time_s {
  uint32_t inactivity_detection_time;
  bool operator==(const struct inactivity_detection_time_s& i) const {
    return (i.inactivity_detection_time == inactivity_detection_time);
  };
} inactivity_detection_time_t;

//-------------------------------------
// 8.2.19 Reporting Triggers
typedef struct reporting_triggers_s {
  uint16_t liusa : 1;
  uint16_t droth : 1;
  uint16_t stop : 1;
  uint16_t start : 1;
  uint16_t quhti : 1;
  uint16_t timth : 1;
  uint16_t volth : 1;
  uint16_t perio : 1;
  uint16_t spare1 : 1;
  uint16_t spare2 : 1;
  uint16_t spare3 : 1;
  uint16_t eveth : 1;
  uint16_t macar : 1;
  uint16_t envcl : 1;
  uint16_t timqu : 1;
  uint16_t volqu : 1;
} reporting_triggers_t;

//-------------------------------------
// 8.2.20 Redirect Information
typedef struct redirect_information_s {
  uint8_t redirect_address_type : 4;
  uint8_t spare : 4;
  uint16_t redirect_server_address_length;
} redirect_information_t;

enum redirect_address_type_e {
  IPV4_ADDRESS = 0,
  IPV6_ADDRESS = 1,
  URL          = 2,
  SIP_URI      = 3
};

//-------------------------------------
// 8.2.21 Report Type
typedef struct report_type_s {
  uint8_t upir : 1;
  uint8_t erir : 1;
  uint8_t usar : 1;
  uint8_t dldr : 1;
  uint8_t spare : 4;
} report_type_t;

//-------------------------------------
// 8.2.22 Offending IE
typedef struct offending_ie_s {
  uint16_t offending_ie;
  bool operator==(const struct offending_ie_s& i) const {
    return (i.offending_ie == offending_ie);
  };
} offending_ie_t;

//-------------------------------------
// 8.2.23 Forwarding Policy
typedef struct forwarding_policy_s {
  uint8_t forwarding_policy_identifier_length;
  std::string forwarding_policy_identifier;  // TODO CHECK TYPE
} forwarding_policy_t;

//-------------------------------------
// 8.2.24 Destination Interface
enum destination_interface_value_e {
  /* Request / Initial message */
  INTERFACE_VALUE_ACCESS         = 0,
  INTERFACE_VALUE_CORE           = 1,
  INTERFACE_VALUE_SGI_LAN_N6_LAN = 2,
  INTERFACE_VALUE_CP_FUNCTION    = 3,
  INTERFACE_VALUE_LI_FUNCTION    = 4
};

typedef struct destination_interface_s {
  uint8_t interface_value;
  bool operator==(const struct destination_interface_s& i) const {
    return (i.interface_value == interface_value);
  };
} destination_interface_t;

//-------------------------------------
// 8.2.25 UP Function Features
struct up_function_features_s {
  uint16_t bucp : 1;
  uint16_t ddnd : 1;
  uint16_t dlbd : 1;
  uint16_t trst : 1;
  uint16_t ftup : 1;
  uint16_t pfdm : 1;
  uint16_t heeu : 1;
  uint16_t treu : 1;

  uint16_t empu : 1;
  uint16_t pdiu : 1;
  uint16_t udbc : 1;
  uint16_t quoac : 1;
  uint16_t trace : 1;
  uint16_t frrt : 1;
  uint16_t spare : 2;

  // up_function_features_s& operator=(up_function_features_s i)
  //{
  //  ...
  //  return *this;
  //}
  up_function_features_s()
      : bucp(0),
        ddnd(0),
        dlbd(0),
        trst(0),
        ftup(0),
        pfdm(0),
        heeu(0),
        treu(0),
        empu(0),
        pdiu(0),
        udbc(0),
        quoac(0),
        trace(0),
        frrt(0),
        spare(0) {}

  up_function_features_s(const up_function_features_s& i) {
    bucp = i.bucp;
    ddnd = i.ddnd;
    dlbd = i.dlbd;
    trst = i.trst;
    ftup = i.ftup;
    pfdm = i.pfdm;
    heeu = i.heeu;
    treu = i.treu;

    empu  = i.empu;
    pdiu  = i.pdiu;
    udbc  = i.udbc;
    quoac = i.quoac;
    trace = i.trace;
    frrt  = i.frrt;
    spare = i.spare;
  }
};
// typedef struct up_function_features_s up_function_features_t;

//-------------------------------------
// 8.2.26 Apply Action
typedef struct apply_action_s {
  uint8_t spare : 3;
  uint8_t dupl : 1;
  uint8_t nocp : 1;
  uint8_t buff : 1;
  uint8_t forw : 1;
  uint8_t drop : 1;
} apply_action_t;

//-------------------------------------
// 8.2.27 Downlink Data Service Information
typedef struct downlink_data_service_information_s {
  uint8_t spare : 6;
  uint8_t qfii : 1;
  uint8_t ppi : 1;
  uint8_t Paging_Policy_Indication;
  uint8_t qfi;
} downlink_data_service_information_t;

//-------------------------------------
// 8.2.28 Downlink Data Notification Delay
typedef struct downlink_data_notification_delay_s {
  uint8_t delay;  // Delay Value in integer multiples of 50 millisecs, or zero
} downlink_data_notification_delay_t;

//-------------------------------------
// 8.2.29 DL Buffering Duration
typedef struct dl_buffering_duration_s {
  uint8_t timer_unit : 3;
  uint8_t timer_value : 5;
} dl_buffering_duration_t;

//-------------------------------------
// 8.2.30 DL Buffering Suggested Packet Count
typedef struct dl_buffering_suggested_packet_count_s {
  uint16_t packet_count;
} dl_buffering_suggested_packet_count_t;

//-------------------------------------
// 8.2.31 PFCPSMReq-Flags
typedef struct pfcpsmreq_flags_s {
  uint8_t spare : 5;
  uint8_t qaurr : 1;
  uint8_t sndem : 1;
  uint8_t drobu : 1;
} pfcpsmreq_flags_t;

//-------------------------------------
// 8.2.32 PFCPSRRsp-Flags
typedef struct pfcpsrrsp_flags_s {
  uint8_t spare : 7;
  uint8_t drobu : 1;
} pfcpsrrsp_flags_t;

//-------------------------------------
// 8.2.33 Sequence Number
typedef struct sequence_number_s {
  uint32_t sequence_number;
  bool operator==(const struct sequence_number_s& i) const {
    return (i.sequence_number == sequence_number);
  };
} sequence_number_t;

//-------------------------------------
// 8.2.34 Metric
typedef struct metric_s {
  uint8_t metric;  // It indicates a percentage and may take binary coded
                   // integer values from
  // and including 0 up to and including 100. Other values shall be considered
  // as 0.
  bool operator==(const struct metric_s& i) const {
    return (i.metric == metric);
  };
} metric_t;

//-------------------------------------
// 8.2.35 Timer
typedef struct timer_s {
  uint8_t timer_unit : 3;
  uint8_t timer_value : 5;
} timer_t;

//-------------------------------------
// 8.2.36 Packet Detection Rule ID (PDR ID)
typedef struct pdr_id_s {
  uint16_t rule_id;
  pdr_id_s() : rule_id(0) {}
  pdr_id_s(const uint8_t& p) : rule_id(p) {}
  pdr_id_s(const struct pdr_id_s& p) : rule_id(p.rule_id) {}
  bool operator==(const struct pdr_id_s& i) const {
    return (i.rule_id == rule_id);
  };
} pdr_id_t;

//-------------------------------------
// 8.2.37 F-SEID
typedef struct fseid_s {
  uint8_t spare : 6;
  uint8_t v4 : 1;
  uint8_t v6 : 1;
  uint64_t seid;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;

  bool operator==(const struct fseid_s& i) const {
    if ((i.seid == this->seid) && (i.v4 == this->v4) &&
        (i.ipv4_address.s_addr == this->ipv4_address.s_addr) &&
        (i.v6 == this->v6) &&
        (i.ipv6_address.s6_addr32[0] == this->ipv6_address.s6_addr32[0]) &&
        (i.ipv6_address.s6_addr32[1] == this->ipv6_address.s6_addr32[1]) &&
        (i.ipv6_address.s6_addr32[2] == this->ipv6_address.s6_addr32[2]) &&
        (i.ipv6_address.s6_addr32[3] == this->ipv6_address.s6_addr32[3])) {
      return true;
    } else {
      return false;
    }
  };
  bool operator<(const struct fseid_s& i) const {
    if (i.seid < this->seid)
      return true;
    else if (i.seid > this->seid)
      return false;
    if (i.v4 == this->v4) {
      if (i.ipv4_address.s_addr > this->ipv4_address.s_addr)
        return true;
      else if (i.ipv4_address.s_addr < this->ipv4_address.s_addr)
        return false;
    } else if (this->v4)
      return true;
    if (i.v6 == this->v6) {
      uint64_t i64 = ((uint64_t) i.ipv6_address.s6_addr32[0] << 32) |
                     ((uint64_t) i.ipv6_address.s6_addr32[1]);
      uint64_t this64 = ((uint64_t) this->ipv6_address.s6_addr32[0] << 32) |
                        ((uint64_t) this->ipv6_address.s6_addr32[1]);

      if (i64 < this64)
        return true;
      else if (i64 > this64)
        return false;
      i64 = ((uint64_t) i.ipv6_address.s6_addr32[2] << 32) |
            ((uint64_t) i.ipv6_address.s6_addr32[3]);
      this64 = ((uint64_t) this->ipv6_address.s6_addr32[2] << 32) |
               ((uint64_t) this->ipv6_address.s6_addr32[3]);
      if (i64 < this64)
        return true;
      else if (i64 > this64)
        return false;
    } else if (this->v6)
      return true;
    return false;
  };
} fseid_t;

//-------------------------------------
// 8.2.38 Node ID
enum node_id_type_value_e {
  /* Request / Initial message */
  NODE_ID_TYPE_IPV4_ADDRESS = 0,
  NODE_ID_TYPE_IPV6_ADDRESS = 1,
  NODE_ID_TYPE_FQDN         = 2,
};

struct node_id_s {
  uint8_t spare : 4;
  uint8_t node_id_type : 4;
  union {
    struct in_addr ipv4_address;
    struct in6_addr ipv6_address;
  } u1;
  std::string fqdn;  // should be in union but problem with virtual ~
  bool operator==(const struct node_id_s& i) const {
    if ((i.node_id_type == this->node_id_type) &&
        (i.u1.ipv4_address.s_addr == this->u1.ipv4_address.s_addr) &&
        (i.fqdn == this->fqdn) &&
        (i.u1.ipv6_address.s6_addr32[0] ==
         this->u1.ipv6_address.s6_addr32[0]) &&
        (i.u1.ipv6_address.s6_addr32[1] ==
         this->u1.ipv6_address.s6_addr32[1]) &&
        (i.u1.ipv6_address.s6_addr32[2] ==
         this->u1.ipv6_address.s6_addr32[2]) &&
        (i.u1.ipv6_address.s6_addr32[3] ==
         this->u1.ipv6_address.s6_addr32[3])) {
      return true;
    } else {
      return false;
    }
  };
};
typedef struct node_id_s node_id_t;
//-------------------------------------
// 8.2.39 PFD Contents
typedef struct pfd_contents_s {
  uint8_t spare : 4;
  uint8_t cp : 1;
  uint8_t dn : 1;
  uint8_t url_ : 1;
  uint8_t fd : 1;
  uint16_t length_of_flow_description;
  std::string flow_description;
  uint16_t length_of_url;
  std::string url;
  uint16_t length_of_domain_name;
  std::string domain_name;
  uint16_t length_of_custom_pfd_contents;
  std::string custom_pfd_contents;
} pfd_contents_t;

//-------------------------------------
// 8.2.40 Measurement Method
typedef struct measurement_method_s {
  uint16_t spare : 5;
  uint16_t event : 1;
  uint16_t volum : 1;
  uint16_t durat : 1;
} measurement_method_t;

//-------------------------------------
// 8.2.41 Usage Report Trigger
typedef struct usage_report_trigger_s {
  uint16_t immer : 1;
  uint16_t droth : 1;
  uint16_t stop : 1;
  uint16_t start : 1;
  uint16_t quhti : 1;
  uint16_t timth : 1;
  uint16_t volth : 1;
  uint16_t perio : 1;
  uint16_t eveth : 1;
  uint16_t macar : 1;
  uint16_t envcl : 1;
  uint16_t monit : 1;
  uint16_t termr : 1;
  uint16_t liusa : 1;
  uint16_t timqu : 1;
  uint16_t volqu : 1;
} usage_report_trigger_t;

//-------------------------------------
// 8.2.42 Measurement Period
typedef struct measurement_period_s {
  uint32_t measurement_period;
} measurement_period_t;

//-------------------------------------
// 8.2.43 Fully qualified PDN Connection Set Identifier (FQ-CSID)
// typedef fq_csid_t fq_csid_t;

//-------------------------------------
// 8.2.44 Volume Measurement
typedef struct volume_measurement_s {
  uint8_t spare : 5;
  uint8_t dlvol : 1;
  uint8_t ulvol : 1;
  uint8_t tovol : 1;
  uint64_t total_volume;
  uint64_t uplink_volume;
  uint64_t downlink_volume;
} volume_measurement_t;

//-------------------------------------
// 8.2.45 Duration Measurement
typedef struct duration_measurement_s {
  uint32_t duration;
} duration_measurement_t;

//-------------------------------------
// 8.2.46 Time of First Packet
typedef struct time_of_first_packet_s {
  uint32_t time_of_first_packet;
} time_of_first_packet_t;

//-------------------------------------
// 8.2.47 Time of Last Packet
typedef struct time_of_last_packet_s {
  uint32_t time_of_last_packet;
} time_of_last_packet_t;

//-------------------------------------
// 8.2.48 Quota Holding Time
typedef struct quota_holding_time_s {
  uint32_t quota_holding_time;
} quota_holding_time_t;

//-------------------------------------
// 8.2.49 Dropped DL Traffic Threshold
typedef struct dropped_dl_traffic_threshold_s {
  uint8_t spare : 6;
  uint8_t dlby : 1;
  uint8_t dlpa : 1;
  uint64_t downlink_packets;
  uint64_t number_of_bytes_of_downlink_data;
} dropped_dl_traffic_threshold_t;

//-------------------------------------
// 8.2.50 Volume Quota
typedef struct volume_quota_s {
  uint8_t spare : 5;
  uint8_t dlvol : 1;
  uint8_t ulvol : 1;
  uint8_t tovol : 1;
  uint64_t total_volume;
  uint64_t uplink_volume;
  uint64_t downlink_volume;
} volume_quota_t;

//-------------------------------------
// 8.2.51 Time Quota
typedef struct time_quota_s {
  uint32_t time_quota;
} time_quota_t;

//-------------------------------------
// 8.2.52 Start Time
typedef struct start_time_s {
  uint32_t start_time;
} start_time_t;

//-------------------------------------
// 8.2.53 End Time
typedef struct end_time_s {
  uint32_t end_time;
} end_time_t;

//-------------------------------------
// 8.2.54 URR ID
typedef struct urr_id_s {
  uint32_t urr_id;
} urr_id_t;

//-------------------------------------
// 8.2.55 Linked URR ID IE
typedef struct linked_urr_id_s {
  uint32_t linked_urr_id;
} linked_urr_id_t;

//-------------------------------------
// 8.2.56 Outer Header Creation
enum outer_header_creation_description_value_e {
  OUTER_HEADER_CREATION_GTPU_UDP_IPV4 = 0x0100,
  OUTER_HEADER_CREATION_GTPU_UDP_IPV6 = 0x0200,
  OUTER_HEADER_CREATION_UDP_IPV4      = 0x0400,
  OUTER_HEADER_CREATION_UDP_IPV6      = 0x0800
};

typedef struct outer_header_creation_s {
  uint16_t outer_header_creation_description;
  uint32_t teid;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  uint16_t port_number;
} outer_header_creation_t;

//-------------------------------------
// 8.2.57 BAR ID
typedef struct bar_id_s {
  uint8_t bar_id;
} bar_id_t;

//-------------------------------------
// 8.2.58 CP Function Features
struct cp_function_features_s {
  uint8_t spare : 6;
  uint8_t ovrl : 1;
  uint8_t load : 1;
  // cp_function_features_s& operator=(cp_function_features_s i)
  //{
  //  ...
  //  return *this;
  //}
  cp_function_features_s() : spare(0), ovrl(0), load(0) {}
  cp_function_features_s(const cp_function_features_s& i)
      : spare(i.spare), ovrl(i.ovrl), load(i.load) {}
};
typedef struct cp_function_features_s cp_function_features_t;
//-------------------------------------
// 8.2.59 Usage Information
typedef struct usage_information_s {
  uint8_t spare : 4;
  uint8_t ube : 1;
  uint8_t uae : 1;
  uint8_t aft : 1;
  uint8_t bef : 1;
} usage_information_t;

//-------------------------------------
// 8.2.60 Application Instance ID
typedef struct application_instance_id_s {
  std::string application_instance_identifier;
} application_instance_id_t;

//-------------------------------------
// 8.2.61 Flow Information
enum flow_direction_value_e {
  UNSPECIFIED   = 0,
  DOWNLINK      = 1,
  UPLINK        = 2,
  BIDIRECTIONAL = 3
};

typedef struct flow_information_s {
  uint8_t spare : 5;
  uint8_t flow_direction : 3;
  uint16_t length_of_flow_description : 3;
  std::string flow_description;
} flow_information_t;

//-------------------------------------
// 8.2.62 UE IP Address
typedef struct ue_ip_address_s {
  uint8_t ipv6d : 1;  // This bit is only applicable to the UE IP address IE in
                      // the PDI IE and whhen V6 bit is set to "1". If this bit
                      // is set to "1", then the IPv6 Prefix Delegation Bits
                      // field shall be present, otherwise the UP function shall
                      // consider IPv6 prefix is default /64.
  uint8_t
      sd : 1;  // This bit is only applicable to the UE IP Address IE in the PDI
               // IE. It shall be set to "0" and ignored by the receiver in IEs
               // other than PDI IE. In the PDI IE, if this bit is set to "0",
               // this indicates a Source IP address; if this bit is set to "1",
               // this indicates a Destination IP address.
  uint8_t v4 : 1;  // If this bit is set to "1", then the IPv4 address field
                   // shall be present in the UE IP Address, otherwise the IPv4
                   // address field shall not be present.
  uint8_t v6 : 1;  // If this bit is set to "1", then the IPv6 address field
                   // shall be present in the UE IP Address, otherwise the IPv6
                   // address field shall not be present.
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  uint8_t ipv6_prefix_delegation_bits;
} ue_ip_address_t;

//-------------------------------------
// 8.2.63 Packet Rate
typedef struct packet_rate_s {
  uint8_t dlpr : 1;
  uint8_t ulpr : 1;
  uint8_t uplink_time_unit : 3;
  uint8_t downlink_time_unit : 3;
  uint16_t maximum_uplink_packet_rate;
  uint16_t maximum_downlink_packet_rate;
} packet_rate_t;

//-------------------------------------
// 8.2.64 Outer Header Removal
#define OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4 0
#define OUTER_HEADER_REMOVAL_GTPU_UDP_IPV6 1
#define OUTER_HEADER_REMOVAL_UDP_IPV4 2
#define OUTER_HEADER_REMOVAL_UDP_IPV6 3
typedef struct outer_header_removal_s {
  uint8_t outer_header_removal_description;
} outer_header_removal_t;

//-------------------------------------
// 8.2.65 Recovery Time Stamp
typedef struct recovery_time_stamp_s {
  uint32_t recovery_time_stamp;
  bool operator==(const struct recovery_time_stamp_s& a) const {
    return (recovery_time_stamp == a.recovery_time_stamp);
  }
} recovery_time_stamp_t;

//-------------------------------------
// 8.2.66 DL Flow Level Marking
typedef struct dl_flow_level_marking_s {
  uint8_t spare : 6;
  uint8_t sci : 1;
  uint8_t ttc : 1;
  uint16_t tos_traffic_class;
  uint16_t service_class_indicator;
} dl_flow_level_marking_t;

//-------------------------------------
// 8.2.67 Header Enrichment
typedef struct header_enrichment_s {
  uint8_t spare : 3;
  uint8_t header_type : 5;
  uint8_t lenght_of_header_field_name;
  std::string header_field_name;
  uint8_t lenght_of_header_field_value;
  std::string header_field_value;
} header_enrichment_t;

//-------------------------------------
// 8.2.68 Measurement Information
typedef struct measurement_information_s {
  uint8_t spare : 5;
  uint8_t radi : 1;
  uint8_t inam : 1;
  uint8_t mbqe : 1;
} measurement_information_t;

//-------------------------------------
// 8.2.69 Node Report Type
typedef struct node_report_type_s {
  uint8_t spare : 7;
  uint8_t upfr : 1;
} node_report_type_t;

//-------------------------------------
// 8.2.70 Remote GTP-U Peer
typedef struct remote_gtp_u_peer_s {
  uint8_t spare : 6;
  uint8_t v4 : 1;
  uint8_t v6 : 1;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
} remote_gtp_u_peer_t;

//-------------------------------------
// 8.2.71 UR-SEQN
typedef struct ur_seqn_s {
  uint32_t ur_seqn;
} ur_seqn_t;

//-------------------------------------
// 8.2.72 Activate Predefined Rules
typedef struct activate_predefined_rules_s {
  std::string predefined_rules_name;
} activate_predefined_rules_t;

//-------------------------------------
// 8.2.73 Deactivate Predefined Rules
typedef struct deactivate_predefined_rules_s {
  std::string predefined_rules_name;
} deactivate_predefined_rules_t;

//-------------------------------------
// 8.2.74 FAR ID
struct far_id_s {
  uint32_t far_id;
  far_id_s() : far_id(0) {}
  far_id_s(const uint8_t& f) : far_id(f) {}
  far_id_s(const struct far_id_s& f) : far_id(f.far_id) {}
  bool operator==(const struct far_id_s& i) const {
    return (i.far_id == far_id);
  };
};
typedef struct far_id_s far_id_t;
//-------------------------------------
// 8.2.75 QER ID
typedef struct qer_id_s {
  uint32_t qer_id;
} qer_id_t;

//-------------------------------------
// 8.2.76 OCI Flags
typedef struct oci_flags_s {
  uint8_t spare : 7;
  uint8_t aoci : 1;
} oci_flags_t;

//-------------------------------------
// 8.2.77 PFCP Association Release Request
typedef struct pfcp_association_release_request_s {
  uint8_t spare : 7;
  uint8_t sarr : 1;
} pfcp_association_release_request_t;

//-------------------------------------
// 8.2.78 Graceful Release Period
typedef struct graceful_release_period_s {
  uint8_t timer_unit : 3;
  uint8_t timer_value : 5;
} graceful_release_period_t;

//-------------------------------------
// 8.2.79 PDN Type
enum pdn_type_value_e {
  /* Request / Initial message */
  IPV4     = 1,
  IPV6     = 2,
  IPV4V6   = 3,
  NON_IP   = 4,
  ETHERNET = 5
};

typedef struct pdn_type_s {
  uint8_t spare : 5;
  uint8_t pdn_type : 3;
} pdn_type_t;

//-------------------------------------
// 8.2.80 Failed Rule ID
enum rule_id_type_e {
  /* Request / Initial message */
  FAILED_RULE_ID_TYPE_PDR = 0,
  FAILED_RULE_ID_TYPE_FAR,
  FAILED_RULE_ID_TYPE_QER,
  FAILED_RULE_ID_TYPE_URR,
  FAILED_RULE_ID_TYPE_BAR
};
typedef struct failed_rule_id_s {
  uint8_t spare : 3;
  uint8_t rule_id_type : 5;
  uint32_t rule_id_value;
} failed_rule_id_t;

//-------------------------------------
// 8.2.81 Time Quota Mechanism
enum base_time_interval_type_value_e {
  /* Request / Initial message */
  CTP = 0,
  DTP = 1
};

typedef struct time_quota_mechanism_s {
  uint8_t spare : 6;
  uint8_t btit : 3;
  uint32_t base_time_interval;
} time_quota_mechanism_t;

//-------------------------------------
// 8.2.82 User Plane IP Resource Information
typedef struct user_plane_ip_resource_information_s {
  uint8_t spare : 1;
  uint8_t assosi : 1;
  uint8_t assoni : 1;
  uint8_t teidri : 3;
  uint8_t v6 : 1;
  uint8_t v4 : 1;
  uint8_t teid_range;
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  uint16_t network_instance;
  uint8_t source_interface;
} user_plane_ip_resource_information_t;

//-------------------------------------
// 8.2.83 User Plane Inactivity Timer
typedef struct user_plane_inactivity_timer_s {
  uint32_t user_plane_inactivity_timer;
} user_plane_inactivity_timer_t;

//-------------------------------------
// 8.2.84 Multiplier
typedef struct multiplier_s {
  uint64_t value_digits;
  uint32_t exponent;
} multiplier_t;

//-------------------------------------
// 8.2.85 Aggregated URR ID IE
typedef struct aggregated_urr_id_s {
  uint32_t urr_id;
} aggregated_urr_id_t;

//-------------------------------------
// 8.2.86 Subsequent Volume Quota
typedef struct subsequent_volume_quota_s {
  uint8_t dlvol : 1;
  uint8_t ulvol : 1;
  uint8_t tovol : 1;
  uint64_t total_volume;
  uint64_t uplink_volume;
  uint64_t downlink_volume;
} subsequent_volume_quota_t;

//-------------------------------------
// 8.2.87 Subsequent Time Quota
typedef struct subsequent_time_quota_s {
  uint64_t time_quota;
} subsequent_time_quota_t;

//-------------------------------------
// 8.2.88 RQI
typedef struct rqi_s {
  uint8_t spare : 7;
  uint8_t rqi : 1;
} rqi_t;

//-------------------------------------
// 8.2.89 QFI
typedef struct qfi_s {
  uint8_t spare : 2;
  uint8_t qfi : 6;
} qfi_t;

//-------------------------------------
// 8.2.90 Query URR Reference
typedef struct query_urr_reference_s {
  uint32_t query_urr_reference;
} query_urr_reference_t;

//-------------------------------------
// 8.2.91 Additional Usage Reports Information
typedef struct additional_usage_reports_information_s {
  uint8_t spare : 7;
  uint8_t auri : 1;
  uint16_t number_of_additional_usage_reports_value;
} additional_usage_reports_information_t;

//-------------------------------------
// 8.2.92 Traffic Endpoint ID
typedef struct traffic_endpoint_id_s {
  uint8_t traffic_endpoint_id;
} traffic_endpoint_id_t;

//-------------------------------------
// 8.2.93 MAC address
typedef struct mac_address_s {
  uint8_t spare : 4;
  uint8_t udes : 1;
  uint8_t usou : 1;
  uint8_t dest;
  uint8_t sour;
  uint8_t source_mac_address[6];
  uint8_t destination_mac_address[6];
  uint8_t upper_source_mac_address[6];
  uint8_t upper_destination_mac_address[6];
} mac_address_t;

//-------------------------------------
// 8.2.94 C-TAG (Customer-VLAN tag)
typedef struct c_tag_s {
  uint8_t spare : 1;
  uint8_t vid : 1;
  uint8_t dei : 1;
  uint8_t pcp : 1;
  uint8_t dei_flag : 1;
  uint8_t pcp_value : 3;
  uint16_t cvid_value;
} c_tag_t;

//-------------------------------------
// 8.2.95 S-TAG (Service-VLAN tag)
typedef struct s_tag_s {
  uint8_t spare : 1;
  uint8_t vid : 1;
  uint8_t dei : 1;
  uint8_t pcp : 1;
  uint8_t dei_flag : 1;
  uint8_t pcp_value : 3;
  uint16_t svid_value;
} s_tag_t;

//-------------------------------------
// 8.2.96 Ethertype
typedef struct ethertype_s {
  uint16_t ethertype;
} ethertype_t;

//-------------------------------------
// 8.2.97 Proxying
typedef struct proxying_s {
  uint8_t spare : 6;
  uint8_t ins : 1;
  uint8_t arp : 1;
} proxying_t;

//-------------------------------------
// 8.2.98 Ethernet Filter ID
typedef struct ethernet_filter_id_s {
  uint32_t ethernet_filter_id;
} ethernet_filter_id_t;

//-------------------------------------
// 8.2.99 Ethernet Filter Properties
typedef struct ethernet_filter_properties_s {
  uint8_t spare : 7;
  uint8_t bide : 1;
} ethernet_filter_properties_t;

//-------------------------------------
// 8.2.100 Suggested Buffering Packets Count
typedef struct suggested_buffering_packets_count_s {
  uint8_t packets_count_value;
} suggested_buffering_packets_count_t;

//-------------------------------------
// 8.2.101 User ID
typedef struct user_id_s {
  uint8_t spare : 4;
  uint8_t naif : 1;
  uint8_t msisdnf : 1;
  uint8_t imeif : 1;
  uint8_t imsif : 1;
  uint8_t length_of_imsi;
  imsi_t imsi;
  uint8_t length_of_imei;
  std::string imei;
  uint8_t length_of_msisdn;
  msisdn_t msisdn;
  uint8_t length_of_nai;
  std::string nai;
} user_id_t;

//-------------------------------------
// 8.2.102 Ethernet PDU Session Information
typedef struct ethernet_pdu_session_information_s {
  uint8_t spare : 7;
  uint8_t ethi : 1;
} ethernet_pdu_session_information_t;

//-------------------------------------
// 8.2.103 MAC addresses Detected
typedef struct mac_addresses_detected_s {
  uint8_t number_of_mac_addresses;
  // std::vector<mac_address> mac_addresses;
} mac_addresses_detected_t;

//-------------------------------------
// 8.2.104 MAC addresses Removed
typedef struct mac_addresses_removed_s {
  uint8_t number_of_mac_addresses;
  // std::vector<mac_address> mac_addresses;
} mac_addresses_removed_t;

//-------------------------------------
// 8.2.105 Ethernet Inactivity Timer
typedef struct ethernet_inactivity_timer_s {
  uint32_t ethernet_inactivity_timer;
} ethernet_inactivity_timer_t;

//-------------------------------------
// 8.2.106 Event ID
typedef struct event_id_s {
  uint32_t event_id;
} event_id_t;

//-------------------------------------
// 8.2.107 Event Threshold
typedef struct event_threshold_s {
  uint32_t event_threshold;
} event_threshold_t;

//-------------------------------------
// 8.2.108 Trace Information
typedef struct trace_information_s {
  uint8_t mcc_digit2 : 4;
  uint8_t mcc_digit1 : 4;
  uint8_t mnc_digit3 : 4;
  uint8_t mcc_digit3 : 4;
  uint8_t mnc_digit2 : 4;
  uint8_t mnc_digit1 : 4;
  uint32_t trace_id;
  uint8_t length_of_triggered_events;
  // TODO CONTINUE
} trace_information_t;

//-------------------------------------
// 8.2.109 Framed-Route
typedef struct framed_route_s {
  std::string framed_route;
} framed_route_t;

//-------------------------------------
// 8.2.110 Framed-Routing
typedef struct framed_routing_s {
  uint32_t framed_routing;
} framed_routing_t;

//-------------------------------------
// 8.2.111 Framed-IPv6-Route
typedef struct framed_ipv6_route_s {
  std::string framed_ipv6_route;
} framed_ipv6_route_t;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Specific IEs
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//  // Table 7.4.3.1-3: PFD
//  typedef struct pfd_s {
//    pfd_contents_t pfd_contents;
//  } pfd_t;
//
//  // Table 7.4.3.1-2: Application ID's PFDs
//  typedef struct application_ids_pfds_s {
//    application_id_t application_id;
//    std::vector<pfd_t> pfds;
//  } application_ids_pfds_t;
//
//  // Table 7.4.5.1.2-1: User Plane Path Failure IE within PFCP Node Report
//  Request typedef struct user_plane_path_failure_report_s {
//    remote_gtp_u_peer_t remote_gtp_u_peer;
//  } user_plane_path_failure_report_t;
//
//  // Table 7.5.2.2-3: Ethernet Packet Filter IE within PFCP Session
//  Establishment Request typedef struct ethernet_packet_filter_s {
//    ethernet_filter_id_t        ethernet_filter_id;
//    ethernet_filter_properties_t ethernet_filter_properties;
//    mac_address_t               mac_address;
//    ethertype_t                 ethertype;
//    c_tag_t                     c_tag;
//    s_tag_t                     s_tag;
//    sdf_filter_t                sdf_filter;
//  } ethernet_packet_filter_t;
//
//  // Table 7.5.2.2-2: PDI IE within PFCP Session Establishment Request
//  typedef struct pdi_s {
//    source_interface_t          source_interface;
//    fteid_t                     local_fteid;
//    network_instance_t          network_instance;
//    ue_ip_address_t             ue_ip_address;
//    traffic_endpoint_id_t       traffic_endpoint_id;
//    sdf_filter_t                sdf_filter;
//    application_id_t            application_id;
//    ethernet_packet_filter_t    ethernet_packet_filter;
//    qfi_t                       qfi;
//    framed_route_t              framed_route;
//    framed_routing_t            framed_routing;
//    framed_ipv6_route_t         framed_ipv6_route;
//  } pdi_t;
//
//  // Table 7.5.2.2-1: Create PDR IE within PFCP Session Establishment Request
//  typedef struct create_pdr_s {
//    pdr_id_t                    pdr_id;
//    precedence_t                precedence;
//    pdi_t                       pdi;
//    far_id_t                    far_id;
//    urr_id_t                    urr_id;
//    qer_id_t                    qer_id;
//    activate_predefined_rules_t activate_predefined_rules;
//  } create_pdr_t;
//
//  // Table 7.5.2.3-2: Forwarding Parameters IE in FAR
//  typedef struct forwarding_parameters_s {
//    destination_interface_t     destination_interface;
//    network_instance_t          network_instance;
//    redirect_information_t      redirect_information;
//    outer_header_creation_t     outer_header_creation;
//    transport_level_marking_t   transport_level_marking;
//    forwarding_policy_t         forwarding_policy;
//    header_enrichment_t         header_enrichment;
//    traffic_endpoint_id_t       linked_traffic_endpoint_id_t;
//    proxying_t                  proxying;
//  } forwarding_parameters_t;
//
//  // Table 7.5.2.3-3: Duplicating Parameters IE in FAR
//  typedef struct duplicating_parameters_s {
//    destination_interface_t     destination_interface;
//    outer_header_creation_t     outer_header_creation;
//    transport_level_marking_t   transport_level_marking;
//    forwarding_policy_t         forwarding_policy;
//  } duplicating_parameters_t;
//
//  // Table 7.5.2.3-1: Create FAR IE within PFCP Session Establishment Request
//  typedef struct create_far_s {
//    far_id_t                    far_id;
//    apply_action_t              apply_action;
//    forwarding_parameters_t     forwarding_parameters;
//    duplicating_parameters_t    duplicating_parameters;
//    bar_id_t                    bar_id;
//  } create_far_t;
//
//  // Table 7.5.2.4-2: Aggregated URRs
//  typedef struct aggregated_urrs_s {
//    aggregated_urr_id_t         aggregated_urr_id;
//    multiplier_t                multiplier;
//  } aggregated_urrs_t;
//
//  // Table 7.5.2.4-3: Additional Monitoring Time
//  typedef struct additional_monitoring_time_s {
//    monitoring_time_t           monitoring_time;
//    subsequent_volume_threshold_t subsequent_volume_threshold;
//    subsequent_time_threshold_t subsequent_time_threshold;
//    subsequent_volume_quota_t   subsequent_volume_quota;
//    subsequent_time_quota_t     subsequent_time_quota;
//  } additional_monitoring_time_t;
//
//  // Table 7.5.2.4-4: Event Information
//  typedef struct event_information_s {
//    event_id_t           event_id;
//    event_threshold_t    event_threshold;
//  } event_information_t;
//
//
//  // Table 7.5.2.4-1: Create URR IE within PFCP Session Establishment Request
//  typedef struct create_urr_s {
//    urr_id_t                    urr_id;
//    measurement_method_t        measurement_method;
//    reporting_triggers_t        reporting_triggers;
//    measurement_period_t        measurement_period;
//    volume_threshold_t          volume_threshold;
//    volume_quota_t              volume_quota;
//    time_threshold_t            time_threshold;
//    time_quota_t                time_quota;
//    quota_holding_time_t        quota_holding_time;
//    dropped_dl_traffic_threshold_t  dropped_dl_traffic_threshold;
//    monitoring_time_t           monitoring_time;
//    event_information_t         event_information;
//    subsequent_volume_threshold_t subsequent_volume_threshold;
//    subsequent_time_threshold_t subsequent_time_threshold;
//    subsequent_volume_quota_t   subsequent_volume_quota;
//    subsequent_time_quota_t     subsequent_time_quota;
//    inactivity_detection_time_t inactivity_detection_time;
//    linked_urr_id_t             linked_urr_id;
//    measurement_information_t   measurement_information;
//    time_quota_mechanism_t      time_quota_mechanism;
//    aggregated_urrs_t           aggregated_urrs;
//    far_id_t                    far_id_for_quota_action;
//    ethernet_inactivity_timer_t ethernet_inactivity_timer;
//    additional_monitoring_time_t additional_monitoring_time;
//  } create_urr_t;
//
//  // Table 7.5.2.5-1: Create QER IE within PFCP Session Establishment Request
//  typedef struct create_qer_s {
//    qer_id_t                    qer_id;
//    qer_correlation_id_t        qer_correlation_id;
//    gate_status_t               gate_status;
//    mbr_t                       maximum_bitrate;
//    gbr_t                       guaranteed_bitrate;
//    packet_rate_t               packet_rate;
//    dl_flow_level_marking_t     dl_flow_level_marking;
//    qfi_t                       qos_flow_identifier;
//    rqi_t                       reflective_qos;
//  } create_qer_t;
//
//  // Table 7.5.2.6-1: Create BAR IE within PFCP Session Establishment Request
//  typedef struct create_bar_s {
//    bar_id_t                    bar_id;
//    downlink_data_notification_delay_t  downlink_data_notification_delay;
//    suggested_buffering_packets_count_t suggested_buffering_packets_count;
//  } create_bar_t;
//
//  // Table 7.5.2.7-1: Create Traffic Endpoint IE within PFCP Session
//  Establishment Request typedef struct create_traffic_endpoint_s {
//    traffic_endpoint_id_t       traffic_endpoint_id;
//    fteid_t                     local_fteid;
//    network_instance_t          network_instance;
//    ue_ip_address_t             ue_ip_address;
//    ethernet_pdu_session_information_t ethernet_pdu_session_information;
//    framed_route_t              framed_route;
//    framed_routing_t            framed_routing;
//    framed_ipv6_route_t         framed_ipv6_route;
//  } create_traffic_endpoint_t;
//
//  // Table 7.5.3.2-1: Created PDR IE within PFCP Session Establishment
//  Response typedef struct created_pdr_s {
//    pdr_id_t                    pdr_id;
//    fteid_t                     local_fteid;
//  } created_pdr_t;
//
//  // Table 7.5.3.3-1: Load Control Information IE within PFCP Session
//  Establishment Response typedef struct load_control_information_s {
//    sequence_number_t           load_control_sequence_number;
//    metric_t                    load_metric;
//  } load_control_information_t;
//
//  // Table 7.5.3.4-1: Overload Control Information IE within PFCP Session
//  Establishment Response typedef struct overload_control_information_s {
//    sequence_number_t           overload_control_sequence_number;
//    metric_t                    overload_reduction_metric;
//    timer_t                     period_of_validity;
//    oci_flags_t                 overload_control_information_flags;
//  } overload_control_information_t;
//
//  // Table 7.5.4.2-1: Update PDR IE within PFCP Session Modification Request
//  typedef struct update_pdr_s {
//    pdr_id_t                    pdr_id;
//    outer_header_removal_t      outer_header_removal;
//    precedence_t                precedence;
//    pdi_t                       pdi;
//    far_id_t                    far_id;
//    urr_id_t                    urr_id;
//    qer_id_t                    qer_id;
//    activate_predefined_rules_t activate_predefined_rules;
//    deactivate_predefined_rules_t deactivate_predefined_rules;
//  } update_pdr_t;
//
//  // Table 7.5.4.3-2: Update Forwarding Parameters IE in FAR
//  typedef struct update_forwarding_parameters_s {
//    destination_interface_t     destination_interface;
//    network_instance_t          network_instance;
//    outer_header_creation_t     outer_header_creation;
//    transport_level_marking_t   transport_level_marking;
//    forwarding_policy_t         forwarding_policy;
//    header_enrichment_t         header_enrichment;
//    pfcpsmreq_flags_t           pfcpsmreq_flags;
//    traffic_endpoint_id_t       linked_traffic_endpoint_id;
//  } update_forwarding_parameters_t;
//
//  // Table 7.5.4.3-3: Update Duplicating Parameters IE in FAR
//  typedef struct update_duplicating_parameters_s {
//    destination_interface_t     destination_interface;
//    outer_header_creation_t     outer_header_creation;
//    transport_level_marking_t   transport_level_marking;
//    forwarding_policy_t         forwarding_policy;
//  } update_duplicating_parameters_t;
//
//  // Table 7.5.4.3-1: Update FAR IE within PFCP Session Modification Request
//  typedef struct update_far_s {
//    far_id_t                    far_id;
//    apply_action_t              apply_action;
//    update_forwarding_parameters_t update_forwarding_parameters;
//    update_duplicating_parameters_t update_duplicating_parameters;
//    bar_id_t                    bar_id;
//  } update_far_t;
//
//  // Table 7.5.4.4-1: Update URR IE within PFCP Session Modification Request
//  typedef struct update_urr_s {
//    urr_id_t                    urr_id;
//    measurement_method_t        measurement_method;
//    reporting_triggers_t        reporting_triggers;
//    measurement_period_t        measurement_period;
//    volume_threshold_t          volume_threshold;
//    volume_quota_t              volume_quota;
//    time_threshold_t            time_threshold;
//    time_quota_t                time_quota;
//    quota_holding_time_t        quota_holding_time;
//    dropped_dl_traffic_threshold_t  dropped_dl_traffic_threshold;
//    monitoring_time_t           monitoring_time;
//    event_information_t         event_information;
//    subsequent_volume_threshold_t subsequent_volume_threshold;
//    subsequent_time_threshold_t subsequent_time_threshold;
//    subsequent_volume_quota_t   subsequent_volume_quota;
//    subsequent_time_quota_t     subsequent_time_quota;
//    inactivity_detection_time_t inactivity_detection_time;
//    linked_urr_id_t             linked_urr_id;
//    measurement_information_t   measurement_information;
//    time_quota_mechanism_t      time_quota_mechanism;
//    aggregated_urrs_t           aggregated_urrs;
//    far_id_t                    far_id_for_quota_action;
//    ethernet_inactivity_timer_t ethernet_inactivity_timer;
//    additional_monitoring_time_t additional_monitoring_time;
//  } update_urr_t;
//
//  // Table 7.5.4.5-1: Update QER IE within PFCP Session Modification Request
//  typedef struct update_qer_s {
//    qer_id_t                    qer_id;
//    qer_correlation_id_t        qer_correlation_id;
//    gate_status_t               gate_status;
//    mbr_t                       maximum_bitrate;
//    gbr_t                       guaranteed_bitrate;
//    packet_rate_t               packet_rate;
//    dl_flow_level_marking_t     dl_flow_level_marking;
//    qfi_t                       qos_flow_identifier;
//    rqi_t                       reflective_qos;
//  } update_qer_t;
//
//  // Table 7.5.4.6-1: Remove PDR IE within PFCP Session Modification Request
//  typedef struct remove_pdr_s {
//    pdr_id_t                    pdr_id;
//  } remove_pdr_t;
//
//  // Table 7.5.4.7-1: Remove FAR IE within PFCP Session Modification Request
//  typedef struct remove_far_s {
//    far_id_t                    far_id;
//  } remove_far_t;
//
//  // Table 7.5.4.8-1: Remove URR IE within PFCP Session Modification Request
//  typedef struct remove_urr_s {
//    urr_id_t                    urr_id;
//  } remove_urr_t;
//
//  // Table 7.5.4.9-1: Remove QER IE PFCP Session Modification Request
//  typedef struct remove_qer_s {
//    qer_id_t                    qer_id;
//  } remove_qer_t;
//
//  // Table 7.5.4.10-1: Query URR IE within PFCP Session Modification Request
//  typedef struct query_urr_s {
//    urr_id_t                    urr_id;
//  } query_urr_t;
//
//  // Table 7.5.4.11-1: Update BAR IE within PFCP Session Modification Request
//  typedef struct update_bar_within_pfcp_session_modification_request_s {
//    bar_id_t                            bar_id;
//    downlink_data_notification_delay_t  downlink_data_notification_delay;
//    suggested_buffering_packets_count_t suggested_buffering_packets_count;
//  } update_bar_within_pfcp_session_modification_request_t;
//
//  // Table 7.5.4.12-1: Remove BAR IE within PFCP Session Modification Request
//  typedef struct remove_bar_s {
//    bar_id_t                            bar_id;
//  } remove_bar_t;
//
//
//  // Table 7.5.3.5-1: Created Traffic Endpoint IE within Sx Session
//  Establishment Response typedef struct created_traffic_endpoint_s {
//    traffic_endpoint_id_t       traffic_endpoint_id;
//    fteid_t                     local_fteid;
//  } created_traffic_endpoint_t;
//
//  // Table 7.5.4.13-1: Update Traffic Endpoint IE within Sx Session
//  Modification Request typedef struct update_traffic_endpoint_s {
//    traffic_endpoint_id_t       traffic_endpoint_id;
//    fteid_t                     local_fteid;
//    network_instance_t          network_instance;
//    ue_ip_address_t             ue_ip_address;
//    framed_route_t              framed_route;
//    framed_routing_t            framed_routing;
//    framed_ipv6_route_t         framed_ipv6_route;
//  } update_traffic_endpoint_t;
//
//  // Table 7.5.4.14-1: Remove Traffic Endpoint IE within Sx Session
//  Modification Request typedef struct remove_traffic_endpoint_s {
//    traffic_endpoint_id_t       traffic_endpoint_id;
//  } remove_traffic_endpoint_t;
//
//  // Table 7.5.8.3-3: Ethernet Traffic Information IE within Usage Report IE
//  typedef struct ethernet_traffic_information_s {
//    mac_addresses_detected_t             mac_addresses_detected;
//    mac_addresses_removed_t              mac_addresses_removed;
//  } ethernet_traffic_information_t;
//
//  // Table 7.5.5.2-1: Usage Report IE within PFCP Session Modification
//  Response typedef struct
//  usage_report_within_pfcp_session_modification_response_s {
//    urr_id_t                    urr_id;
//    ur_seqn_t                   ur_seqn;
//    usage_report_trigger_t      usage_report_trigger;
//    start_time_t                start_time;
//    end_time_t                  end_time;
//    volume_measurement_t        volume_measurement;
//    duration_measurement_t      duration_measurement;
//    time_of_first_packet_t      time_of_first_packet;
//    time_of_last_packet_t       time_of_last_packet;
//    usage_information_t         usage_information;
//    query_urr_reference_t       query_urr_reference;
//    ethernet_traffic_information_t ethernet_traffic_information;
//  } usage_report_within_pfcp_session_modification_response_t;
//
//  // Table 7.5.7.2-1: Usage Report IE within PFCP Session Deletion Response
//  typedef struct usage_report_within_pfcp_session_deletion_response_s {
//    urr_id_t                    urr_id;
//    ur_seqn_t                   ur_seqn;
//    usage_report_trigger_t      usage_report_trigger;
//    start_time_t                start_time;
//    end_time_t                  end_time;
//    volume_measurement_t        volume_measurement;
//    duration_measurement_t      duration_measurement;
//    time_of_first_packet_t      time_of_first_packet;
//    time_of_last_packet_t       time_of_last_packet;
//    usage_information_t         usage_information;
//    ethernet_traffic_information_t ethernet_traffic_information;
//  } usage_report_within_pfcp_session_deletion_response_t;
//
//  // Table 7.5.8.2-1: Downlink Data Report IE within PFCP Session Report
//  Request typedef struct downlink_data_report_s {
//    pdr_id_t                    pdr_id;
//    downlink_data_service_information_t  downlink_data_service_information;
//  } downlink_data_report_t;
//
//  // Table 7.5.8.3-2: Application Detection Information IE within Usage Report
//  IE typedef struct application_detection_information_s {
//    application_id_t                    application_id;
//    application_instance_id_t           application_instance_id;
//    flow_information_t                  flow_information;
//  } application_detection_information_t;
//
//  // Table 7.5.8.3-2A: Event Reporting IE within Usage Report IE
//  typedef struct event_reporting_s {
//    event_id_t                    event_id;
//  } event_reporting_t;
//
//  //
//  // Table 7.5.8.3-1: Usage Report IE within PFCP Session Report Request
//  typedef struct usage_report_within_pfcp_session_report_request_s {
//    urr_id_t                    urr_id;
//    ur_seqn_t                   ur_seqn;
//    usage_report_trigger_t      usage_report_trigger;
//    start_time_t                start_time;
//    end_time_t                  end_time;
//    volume_measurement_t        volume_measurement;
//    duration_measurement_t      duration_measurement;
//    application_detection_information_t application_detection_information;
//    ue_ip_address_t             ue_ip_address;
//    network_instance_t          network_instance;
//    time_of_first_packet_t      time_of_first_packet;
//    time_of_last_packet_t       time_of_last_packet;
//    usage_information_t         usage_information;
//    query_urr_reference_t       query_urr_reference;
//    event_reporting_t           event_reporting;
//    ethernet_traffic_information_t ethernet_traffic_information;
//  } usage_report_within_pfcp_session_report_request_t;
//
//  // Table 7.5.8.4-1: Error Indication Report IE within PFCP Session Report
//  Request typedef struct error_indication_report_s {
//    fteid_t                     remote_fteid;
//  } error_indication_report_t;
//
//  // Table 7.5.9.2-1: Update BAR IE in PFCP Session Report Response
//  typedef struct update_bar_within_pfcp_session_report_response_s {
//    bar_id_t                              bar_id;
//    downlink_data_notification_delay_t    downlink_data_notification_delay;
//    dl_buffering_duration_t               dl_buffering_duration;
//    dl_buffering_suggested_packet_count_t
//    dl_buffering_buffering_suggested_packet_count;
//    suggested_buffering_packets_count_t   suggested_buffering_packets_count;
//  } update_bar_within_pfcp_session_report_response_t;
}  // namespace pfcp

namespace std {

template<>
struct hash<pfcp::fseid_t> {
  std::size_t operator()(const pfcp::fseid_t& k) const {
    using std::hash;
    using std::size_t;
    std::size_t h;
    if (k.v4) {
      h = k.ipv4_address.s_addr ^ (k.seid);
      return h;
    }
    if (k.v6) {
      h = k.seid ^ (k.ipv6_address.s6_addr32[0] ^ k.ipv6_address.s6_addr32[1] ^
                    k.ipv6_address.s6_addr32[2] ^ k.ipv6_address.s6_addr32[3]);
      return h;
    }
    return k.seid;
  }
};

template<>
class hash<pfcp::node_id_t> {
 public:
  size_t operator()(const pfcp::node_id_t& k) const {
    using std::hash;
    using std::size_t;
    std::size_t h = 0;
    switch (k.node_id_type) {
      case pfcp::NODE_ID_TYPE_IPV4_ADDRESS:
        h = std::hash<uint32_t>()(k.u1.ipv4_address.s_addr);
        return h;
        break;
      case pfcp::NODE_ID_TYPE_IPV6_ADDRESS:
        h = k.u1.ipv6_address.s6_addr32[0] ^ k.u1.ipv6_address.s6_addr32[1] ^
            k.u1.ipv6_address.s6_addr32[2] ^ k.u1.ipv6_address.s6_addr32[3];
        return h;
        break;
      case pfcp::NODE_ID_TYPE_FQDN:
        h = std::hash<std::string>()(k.fqdn);
        return h;
      default:
        return h;
    }
  }
};
}  // namespace std
#endif /* FILE_3GPP_129_244_H_SEEN */
