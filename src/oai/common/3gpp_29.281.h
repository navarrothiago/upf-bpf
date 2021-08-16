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

/*! \file 3gpp_29.281.h
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_3GPP_29_281_SEEN
#define FILE_3GPP_29_281_SEEN
#include "3gpp_29.274.h"
#include "3gpp_commons.h"
#include "common_root_types.h"
#include "logger.hpp"  // for fmt::format in spdlog

#include <arpa/inet.h>
#include <stdint.h>
#include <string>
#include <vector>

namespace gtpv1u {

struct gtpu_exception : public std::exception {
  gtpu_exception() throw() {
    cause = 0;
    phrase.assign("GTPV1-U Exception unknown cause");
  }

  gtpu_exception(int acause) throw() {
    cause  = acause;
    phrase = fmt::format("GTPV1-U Exception cause {}", cause);
  }
  const char* what() const throw() { return phrase.c_str(); }

 public:
  int cause;
  std::string phrase;
};

struct gtpu_msg_bad_length_exception : public gtpu_exception {
 public:
  gtpu_msg_bad_length_exception(
      const uint8_t msg_type, const uint16_t msg_size) throw() {
    phrase = fmt::format(
        "GTPV1-U msg {} Bad Length {} Exception", msg_type, msg_size);
  }
  gtpu_msg_bad_length_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~gtpu_msg_bad_length_exception() throw() {}
};

struct gtpu_msg_unimplemented_ie_exception : public gtpu_exception {
 public:
  gtpu_msg_unimplemented_ie_exception(
      const uint8_t msg_type, const uint8_t ie_type,
      const uint8_t instance = 0) throw() {
    phrase = fmt::format(
        "GTPV1-U msg {} Unimplemented {} IE Instance {} Exception", msg_type,
        ie_type, instance);
  }
  gtpu_msg_unimplemented_ie_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~gtpu_msg_unimplemented_ie_exception() throw() {}
};

struct gtpu_msg_illegal_ie_exception : public gtpu_exception {
 public:
  gtpu_msg_illegal_ie_exception(
      const uint8_t msg_type, const uint8_t ie_type) throw() {
    phrase =
        fmt::format("GTPV1-U msg {} Illegal {} Exception", msg_type, ie_type);
  }
  gtpu_msg_illegal_ie_exception(std::string& aphrase) throw() {
    phrase = aphrase;
  }
  virtual ~gtpu_msg_illegal_ie_exception() throw() {}
};

struct gtpu_ie_exception : public gtpu_exception {
 public:
  gtpu_ie_exception(uint8_t ie_type) throw() {
    phrase = fmt::format("GTPV1-U IE {} Exception", ie_type);
  }
  gtpu_ie_exception(std::string& aphrase) throw() { phrase = aphrase; }
  virtual ~gtpu_ie_exception() throw() {}
};

struct gtpu_ie_unimplemented_exception : public gtpu_ie_exception {
 public:
  gtpu_ie_unimplemented_exception(uint8_t ie_type) throw()
      : gtpu_ie_exception(ie_type) {
    phrase = fmt::format("GTPV1-U IE {} Unimplemented Exception", ie_type);
  }
  virtual ~gtpu_ie_unimplemented_exception() throw() {}
};

struct gtpu_tlv_exception : public gtpu_ie_exception {
 public:
  gtpu_tlv_exception(uint8_t ie_type) throw() : gtpu_ie_exception(ie_type) {
    phrase = fmt::format("GTPV1-U IE TLV {} Exception", ie_type);
  }
  virtual ~gtpu_tlv_exception() throw() {}
};

struct gtpu_tlv_bad_length_exception : public gtpu_tlv_exception {
 public:
  gtpu_tlv_bad_length_exception(uint8_t ie_type, uint16_t ie_length) throw()
      : gtpu_tlv_exception(ie_type) {
    phrase = fmt::format("GTPV1-U IE TLV {} Bad Length {} Exception", ie_type);
  }
  virtual ~gtpu_tlv_bad_length_exception() throw() {}
};

struct gtpu_ie_value_exception : public gtpu_ie_exception {
 public:
  gtpu_ie_value_exception(uint8_t ie_type, const char* field) throw()
      : gtpu_ie_exception(ie_type) {
    phrase =
        fmt::format("GTPV1-U IE {} Bad Value of {} Exception", ie_type, field);
  }
  virtual ~gtpu_ie_value_exception() throw() {}
};

#define GTPU_NO_MORE_EXTENSION_HEADER (0)
#define GTPU_IE_RECOVERY 14
#define GTPU_IE_TUNNEL_ENDPOINT_IDENTIFIER_DATA_I 16
#define GTPU_IE_GTP_U_PEER_ADDRESS 133
#define GTPU_IE_EXTENSION_HEADER_TYPE_LIST 141
#define GTPU_IE_PRIVATE_EXTENSION 255

#define GTPU_ECHO_REQUEST (1)
#define GTPU_ECHO_RESPONSE (2)
#define GTPU_ERROR_INDICATION (26)
#define GTPU_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION (31)
#define GTPU_END_MARKER (254)
#define GTPU_G_PDU (255)

#define GTPU_DL_PDU_SESSION_INFORMATION (0)
#define GTPU_UL_PDU_SESSION_INFORMATION (1)
#define GTPU_PDU_SESSION_CONTAINER (133)

}  // namespace gtpv1u

// 8.2 Recovery

// 8.3 Tunnel Endpoint Identifier Data I
typedef struct tunnel_endpoint_identifier_data_i_s {
  uint32_t tunnel_endpoint_identifier_data_i;
} tunnel_endpoint_identifier_data_i_t;

// 8.4 GTP-U Peer Address
typedef struct gtp_u_peer_address_s {
  // may use variant if can stay with C++17
  struct in_addr ipv4_address;
  struct in6_addr ipv6_address;
  bool is_v4;
} gtp_u_peer_address_t;

// 8.5 Extension Header Type List
typedef struct extension_header_type_list_s {
  uint8_t length;
  std::vector<uint8_t> extension_types_list;
} extension_header_type_list_t;

// 8.6 Private Extension defined in 3gpp_29.274.h
// typedef struct private_extension_s {
//  uint16_t  extension_identifier;
//  std::string  extension_value;
//} private_extension_t;

#endif /* FILE_3GPP_29_281_SEEN */