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

/*! \file 3gpp_24.008.h
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_3GPP_24_008_SEEN
#define FILE_3GPP_24_008_SEEN

#include <stdint.h>
#include <string>

//------------------------------------------------------------------------------
// 10.5.1.3 Location Area Identification
//------------------------------------------------------------------------------
#define LOCATION_AREA_IDENTIFICATION_IE_TYPE 3
#define LOCATION_AREA_IDENTIFICATION_IE_MIN_LENGTH 6
#define LOCATION_AREA_IDENTIFICATION_IE_MAX_LENGTH 6

#define INVALID_LAC_0000                                                       \
  (uint16_t) 0x0000 /*!< \brief  This LAC can be coded using a full            \
                       hexadecimal representation except for the following     \
                       reserved hexadecimal values: 0000, and FFFE.   */

#define INVALID_LAC_FFFE                                                       \
  (uint16_t) 0xFFFE /*!< \brief  This LAC can be coded using a full            \
                       hexadecimal representation except for the following     \
                       reserved hexadecimal values: 0000, and FFFE.   */

//------------------------------------------------------------------------------
// 10.5.6.3 Protocol configuration options
//------------------------------------------------------------------------------
#define PROTOCOL_CONFIGURATION_OPTIONS_MAX_LENGTH 250
#define PCO_CONFIGURATION_PROTOCOL_PPP_FOR_USE_WITH_IP_PDP_TYPE_OR_IP_PDN_TYPE \
  0b000

// Protocol identifiers defined in RFC 3232
#define PCO_PROTOCOL_IDENTIFIER_LCP (0xC021)
#define PCO_PROTOCOL_IDENTIFIER_PAP (0xC023)
#define PCO_PROTOCOL_IDENTIFIER_CHAP (0xC223)
#define PCO_PROTOCOL_IDENTIFIER_IPCP (0x8021)

/* CONTAINER IDENTIFIER MS to network direction:*/
#define PCO_CONTAINER_IDENTIFIER_P_CSCF_IPV6_ADDRESS_REQUEST (0x0001)
#define PCO_CONTAINER_IDENTIFIER_IM_CN_SUBSYSTEM_SIGNALING_FLAG (0x0002)
#define PCO_CONTAINER_IDENTIFIER_DNS_SERVER_IPV6_ADDRESS_REQUEST (0x0003)
// NOT SUPPORTED                                                        (0x0004)
#define PCO_CONTAINER_IDENTIFIER_MS_SUPPORT_OF_NETWORK_REQUESTED_BEARER_CONTROL_INDICATOR \
  (0x0005)
// RESERVED                                                             (0x0006)
#define PCO_CONTAINER_IDENTIFIER_DSMIPV6_HOME_AGENT_ADDRESS_REQUEST (0x0007)
#define PCO_CONTAINER_IDENTIFIER_DSMIPV6_HOME_NETWORK_PREFIX_REQUEST (0x0008)
#define PCO_CONTAINER_IDENTIFIER_DSMIPV6_IPV4_HOME_AGENT_ADDRESS_REQUEST       \
  (0x0009)
#define PCO_CONTAINER_IDENTIFIER_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING      \
  (0x000A)
#define PCO_CONTAINER_IDENTIFIER_IPV4_ADDRESS_ALLOCATION_VIA_DHCPV4 (0x000B)
#define PCO_CONTAINER_IDENTIFIER_P_CSCF_IPV4_ADDRESS_REQUEST (0x000C)
#define PCO_CONTAINER_IDENTIFIER_DNS_SERVER_IPV4_ADDRESS_REQUEST (0x000D)
#define PCO_CONTAINER_IDENTIFIER_MSISDN_REQUEST (0x000E)
#define PCO_CONTAINER_IDENTIFIER_IFOM_SUPPORT_REQUEST (0x000F)
#define PCO_CONTAINER_IDENTIFIER_IPV4_LINK_MTU_REQUEST (0x0010)
#define PCO_CONTAINER_IDENTIFIER_MS_SUPPORT_OF_LOCAL_ADDRESS_IN_TFT_INDICATOR  \
  (0X0011)
#define PCO_CONTAINER_IDENTIFIER_P_CSCF_RE_SELECTION_SUPPORT (0X0012)
#define PCO_CONTAINER_IDENTIFIER_NBIFOM_REQUEST_INDICATOR (0X0013)
#define PCO_CONTAINER_IDENTIFIER_NBIFOM_MODE (0X0014)
#define PCO_CONTAINER_IDENTIFIER_NON_IP_LINK_MTU_REQUEST (0X0015)
#define PCO_CONTAINER_IDENTIFIER_APN_RATE_CONTROL_SUPPORT_INDICATOR (0X0016)
#define PCO_CONTAINER_IDENTIFIER_3GPP_PS_DATA_OFF_UE_STATUS (0X0017)
#define PCO_CONTAINER_IDENTIFIER_RELIABLE_DATA_SERVICE_REQUEST_INDICATOR       \
  (0X0018)
#define PCO_CONTAINER_IDENTIFIER_ADDITIONAL_APN_RATE_CONTROL_FOR_EXCEPTION_DATA_SUPPORT_INDICATOR \
  (0X0019)
#define PCO_CONTAINER_IDENTIFIER_PDU_SESSION_ID (0X001A)
#define PCO_CONTAINER_IDENTIFIER_ETHERNET_FRAME_PAYLOAD_MTU_REQUEST (0X0020)
#define PCO_CONTAINER_IDENTIFIER_UNSTRUCTURED_LINK_MTU_REQUEST (0X0021)
// RESERVED (0xFF00..FFFF)

/* CONTAINER IDENTIFIER Network to MS direction:*/
#define PCO_CONTAINER_IDENTIFIER_P_CSCF_IPV6_ADDRESS (0x0001)
#define PCO_CONTAINER_IDENTIFIER_DNS_SERVER_IPV6_ADDRESS (0x0003)
#define PCO_CONTAINER_IDENTIFIER_POLICY_CONTROL_REJECTION_CODE (0x0004)
#define PCO_CONTAINER_IDENTIFIER_SELECTED_BEARER_CONTROL_MODE (0x0005)
// RESERVED                                                             (0x0006)
#define PCO_CONTAINER_IDENTIFIER_DSMIPV6_HOME_AGENT_ADDRESS (0x0007)
#define PCO_CONTAINER_IDENTIFIER_DSMIPV6_HOME_NETWORK_PREFIX (0x0008)
#define PCO_CONTAINER_IDENTIFIER_DSMIPV6_IPV4_HOME_AGENT_ADDRESS (0x0009)
// RESERVED                                                             (0x000A)
// RESERVED                                                             (0x000B)
#define PCO_CONTAINER_IDENTIFIER_P_CSCF_IPV4_ADDRESS (0x000C)
#define PCO_CONTAINER_IDENTIFIER_DNS_SERVER_IPV4_ADDRESS (0x000D)
#define PCO_CONTAINER_IDENTIFIER_MSISDN (0x000E)
#define PCO_CONTAINER_IDENTIFIER_IFOM_SUPPORT (0x000F)
#define PCO_CONTAINER_IDENTIFIER_IPV4_LINK_MTU (0x0010)
// RESERVED (0xFF00..FFFF)
#define PCO_CONTAINER_IDENTIFIER_NETWORK_SUPPORT_OF_LOCAL_ADDRESS_IN_TFT_INDICATOR \
  (0x0011)
#define PCO_CONTAINER_IDENTIFIER_NBIFOM_ACCEPTED_INDICATOR (0x0013)
//#define PCO_CONTAINER_IDENTIFIER_NBIFOM_MODE (0x0014)
#define PCO_CONTAINER_IDENTIFIER_NON_IP_LINK_MTU (0x0015)
#define PCO_CONTAINER_IDENTIFIER_APN_RATE_CONTROL_PARAMETERS (0x0016)
#define PCO_CONTAINER_IDENTIFIER_3GPP_PS_DATA_OFF_SUPPORT_INDICATION (0x0017)
#define PCO_CONTAINER_IDENTIFIER_RELIABLE_DATA_SERVICE_ACCEPTED_INDICATOR      \
  (0x0018)
#define PCO_CONTAINER_IDENTIFIER_ADDITIONAL_APN_RATE_CONTROL_FOR_EXCEPTION_DATA_PARAMETERS \
  (0x0019)
#define PCO_CONTAINER_IDENTIFIER_RESERVED (0x001A)
#define PCO_CONTAINER_IDENTIFIER_S_NSSAI (0x001B)
#define PCO_CONTAINER_IDENTIFIER_QOS_RULES (0x001C)
#define PCO_CONTAINER_IDENTIFIER_SESSION_AMBR (0x001D)
#define PCO_CONTAINER_IDENTIFIER_PDU_SESSION_ADDRESS_LIFETIME (0x001E)
#define PCO_CONTAINER_IDENTIFIER_QOS_FLOW_DESCRIPTIONS (0x001F)
#define PCO_CONTAINER_IDENTIFIER_ETHERNET_FRAME_PAYLOAD_MTU (0x0020)
#define PCO_CONTAINER_IDENTIFIER_UNSTRUCTURED_LINK_MTU (0x0021)
/*_Both_directions:*/
#define PCO_CI_IM_CN_SUBSYSTEM_SIGNALING_FLAG (0x0002)

typedef struct pco_protocol_or_container_id_s {
  uint16_t protocol_id;
  uint8_t length_of_protocol_id_contents;
  std::string protocol_id_contents;
} pco_protocol_or_container_id_t;

typedef struct protocol_configuration_options_s {
  uint8_t ext : 1;
  uint8_t spare : 4;
  uint8_t configuration_protocol : 3;
  uint8_t num_protocol_or_container_id;
  // arbitrary value, can be greater than defined (250/3)
#define PCO_UNSPEC_MAXIMUM_PROTOCOL_ID_OR_CONTAINER_ID 8
  pco_protocol_or_container_id_t
      protocol_or_container_ids[PCO_UNSPEC_MAXIMUM_PROTOCOL_ID_OR_CONTAINER_ID];
} protocol_configuration_options_t;

//------------------------------------------------------------------------------
// 10.5.6.11 Packet Flow Identifier
//------------------------------------------------------------------------------
#define PACKET_FLOW_IDENTIFIER_IE_TYPE 4
#define PACKET_FLOW_IDENTIFIER_IE_MIN_LENGTH 3
#define PACKET_FLOW_IDENTIFIER_IE_MAX_LENGTH 3

typedef uint8_t packet_flow_identifier_t;

int encode_packet_flow_identifier_ie(
    packet_flow_identifier_t* packetflowidentifier, const bool iei_present,
    uint8_t* buffer, const uint32_t len);
int decode_packet_flow_identifier_ie(
    packet_flow_identifier_t* packetflowidentifier, const bool iei_present,
    uint8_t* buffer, const uint32_t len);

//------------------------------------------------------------------------------
// 10.5.6.12 Traffic Flow Template
//------------------------------------------------------------------------------
#define TRAFFIC_FLOW_TEMPLATE_MINIMUM_LENGTH 3
#define TRAFFIC_FLOW_TEMPLATE_MAXIMUM_LENGTH 257

/*
 * ----------------------------------------------------------------------------
 *        Packet filter list
 * ----------------------------------------------------------------------------
 */

#define TRAFFIC_FLOW_TEMPLATE_IPV4_REMOTE_ADDR 0b00010000
#define TRAFFIC_FLOW_TEMPLATE_IPV4_LOCAL_ADDR 0b00010001
#define TRAFFIC_FLOW_TEMPLATE_IPV6_REMOTE_ADDR 0b00100000
#define TRAFFIC_FLOW_TEMPLATE_IPV6_REMOTE_ADDR_PREFIX 0b00100001
#define TRAFFIC_FLOW_TEMPLATE_IPV6_LOCAL_ADDR_PREFIX 0b00100011
#define TRAFFIC_FLOW_TEMPLATE_PROTOCOL_NEXT_HEADER 0b00110000
#define TRAFFIC_FLOW_TEMPLATE_SINGLE_LOCAL_PORT 0b01000000
#define TRAFFIC_FLOW_TEMPLATE_LOCAL_PORT_RANGE 0b01000001
#define TRAFFIC_FLOW_TEMPLATE_SINGLE_REMOTE_PORT 0b01010000
#define TRAFFIC_FLOW_TEMPLATE_REMOTE_PORT_RANGE 0b01010001
#define TRAFFIC_FLOW_TEMPLATE_SECURITY_PARAMETER_INDEX 0b01100000
#define TRAFFIC_FLOW_TEMPLATE_TYPE_OF_SERVICE_TRAFFIC_CLASS 0b01110000
#define TRAFFIC_FLOW_TEMPLATE_FLOW_LABEL 0b10000000

// The description and valid combinations of packet filter component type
// identifiers in a packet filter are defined in 3GPP TS 23.060
// subclause 15.3.2.

/*
 * Packet filter content
 * ---------------------
 */
typedef struct packet_filter_contents_s {
#define TRAFFIC_FLOW_TEMPLATE_IPV4_REMOTE_ADDR_FLAG (1 << 0)
#define TRAFFIC_FLOW_TEMPLATE_IPV4_LOCAL_ADDR_FLAG (1 << 1)
#define TRAFFIC_FLOW_TEMPLATE_IPV6_REMOTE_ADDR_FLAG (1 << 2)
#define TRAFFIC_FLOW_TEMPLATE_IPV6_REMOTE_ADDR_PREFIX_FLAG (1 << 3)
#define TRAFFIC_FLOW_TEMPLATE_IPV6_LOCAL_ADDR_PREFIX_FLAG (1 << 4)
#define TRAFFIC_FLOW_TEMPLATE_PROTOCOL_NEXT_HEADER_FLAG (1 << 5)
#define TRAFFIC_FLOW_TEMPLATE_SINGLE_LOCAL_PORT_FLAG (1 << 6)
#define TRAFFIC_FLOW_TEMPLATE_LOCAL_PORT_RANGE_FLAG (1 << 7)
#define TRAFFIC_FLOW_TEMPLATE_SINGLE_REMOTE_PORT_FLAG (1 << 8)
#define TRAFFIC_FLOW_TEMPLATE_REMOTE_PORT_RANGE_FLAG (1 << 9)
#define TRAFFIC_FLOW_TEMPLATE_SECURITY_PARAMETER_INDEX_FLAG (1 << 10)
#define TRAFFIC_FLOW_TEMPLATE_TYPE_OF_SERVICE_TRAFFIC_CLASS_FLAG (1 << 11)
#define TRAFFIC_FLOW_TEMPLATE_FLOW_LABEL_FLAG (1 << 12)
  uint16_t flags;
#define TRAFFIC_FLOW_TEMPLATE_IPV4_ADDR_SIZE 4
  struct {
    uint8_t addr;
    uint8_t mask;
  } ipv4remoteaddr[TRAFFIC_FLOW_TEMPLATE_IPV4_ADDR_SIZE];
  struct {
    uint8_t addr;
    uint8_t mask;
  } ipv4localaddr[TRAFFIC_FLOW_TEMPLATE_IPV4_ADDR_SIZE];
#define TRAFFIC_FLOW_TEMPLATE_IPV6_ADDR_SIZE 16
  struct {
    uint8_t addr;
    uint8_t mask;
  } ipv6remoteaddr[TRAFFIC_FLOW_TEMPLATE_IPV6_ADDR_SIZE];
  struct {
    uint8_t addr[TRAFFIC_FLOW_TEMPLATE_IPV6_ADDR_SIZE];
    uint8_t prefix;
  } ipv6remoteaddrprefix;
  struct {
    uint8_t addr[TRAFFIC_FLOW_TEMPLATE_IPV6_ADDR_SIZE];
    uint8_t prefix;
  } ipv6localaddrprefix;
  uint8_t protocolidentifier_nextheader;
  uint16_t singlelocalport;
  struct {
    uint16_t lowlimit;
    uint16_t highlimit;
  } localportrange;
  uint16_t singleremoteport;
  struct {
    uint16_t lowlimit;
    uint16_t highlimit;
  } remoteportrange;
  uint32_t securityparameterindex;
  struct {
    uint8_t value;
    uint8_t mask;
  } typdeofservice_trafficclass;
  uint32_t flowlabel;
} packet_filter_contents_t;

/*
 * Packet filter list when the TFP operation is "delete existing TFT"
 * and "no TFT operation" shall be empty.
 * ---------------------------------------------------------------
 */
typedef struct {
} no_packet_filter_t;

typedef no_packet_filter_t delete_existing_tft_t;
typedef no_packet_filter_t no_tft_operation_t;

/*
 * Packet filter list when the TFT operation is "delete existing TFT"
 * shall contain a variable number of packet filter identifiers.
 * ------------------------------------------------------------------
 */
#define TRAFFIC_FLOW_TEMPLATE_PACKET_IDENTIFIER_MAX 16
typedef struct packet_filter_identifier_s {
  uint8_t identifier;
} packet_filter_identifier_t;

typedef packet_filter_identifier_t delete_packet_filter_t;

/*
 * Packet filter list when the TFT operation is "create new TFT",
 * "add packet filters to existing TFT" and "replace packet filters
 * in existing TFT" shall contain a variable number of packet filters
 * ------------------------------------------------------------------
 */
#define TRAFFIC_FLOW_TEMPLATE_NB_PACKET_FILTERS_MAX 4
typedef struct packet_filter_s {
  uint8_t spare : 2;
#define TRAFFIC_FLOW_TEMPLATE_PRE_REL7_TFT_FILTER 0b00
#define TRAFFIC_FLOW_TEMPLATE_DOWNLINK_ONLY 0b01
#define TRAFFIC_FLOW_TEMPLATE_UPLINK_ONLY 0b10
#define TRAFFIC_FLOW_TEMPLATE_BIDIRECTIONAL 0b11
  uint8_t direction : 2;
  uint8_t identifier : 4;
  uint8_t eval_precedence;
  uint8_t length;
  packet_filter_contents_t packetfiltercontents;
} packet_filter_t;

typedef packet_filter_t create_new_tft_t;
typedef packet_filter_t add_packet_filter_t;
typedef packet_filter_t replace_packet_filter_t;
/*
 * Packet filter list
 * ------------------
 */
typedef union {
  create_new_tft_t createnewtft[TRAFFIC_FLOW_TEMPLATE_NB_PACKET_FILTERS_MAX];
  add_packet_filter_t
      addpacketfilter[TRAFFIC_FLOW_TEMPLATE_NB_PACKET_FILTERS_MAX];
  replace_packet_filter_t
      replacepacketfilter[TRAFFIC_FLOW_TEMPLATE_NB_PACKET_FILTERS_MAX];
  delete_packet_filter_t
      deletepacketfilter[TRAFFIC_FLOW_TEMPLATE_PACKET_IDENTIFIER_MAX];
  delete_existing_tft_t deleteexistingtft;
  no_tft_operation_t notftoperation;
} packet_filter_list_t;

typedef struct parameter_s {
#define PARAMETER_IDENTIFIER_AUTHORIZATION_TOKEN 0x01  // Authorization Token
#define PARAMETER_IDENTIFIER_FLOW_IDENTIFIER 0x02      // Flow Identifier
#define PARAMETER_IDENTIFIER_PACKET_FILTER_IDENTIFIER                          \
  0x03  // Packet Filter Identifier
  uint8_t parameteridentifier;
  uint8_t length;
  std::string contents;
} parameter_t;

typedef struct parameters_list_s {
  uint8_t num_parameters;
#define TRAFFIC_FLOW_TEMPLATE_NB_PARAMETERS_MAX 16  // TODO or may use []
  parameter_t parameter[TRAFFIC_FLOW_TEMPLATE_NB_PARAMETERS_MAX];
} parameters_list_t;

typedef struct traffic_flow_template_s {
#define TRAFFIC_FLOW_TEMPLATE_OPCODE_SPARE 0b000
#define TRAFFIC_FLOW_TEMPLATE_OPCODE_CREATE_NEW_TFT 0b001
#define TRAFFIC_FLOW_TEMPLATE_OPCODE_DELETE_EXISTING_TFT 0b010
#define TRAFFIC_FLOW_TEMPLATE_OPCODE_ADD_PACKET_FILTER_TO_EXISTING_TFT 0b011
#define TRAFFIC_FLOW_TEMPLATE_OPCODE_REPLACE_PACKET_FILTERS_IN_EXISTING_TFT    \
  0b100
#define TRAFFIC_FLOW_TEMPLATE_OPCODE_DELETE_PACKET_FILTERS_FROM_EXISTING_TFT   \
  0b101
#define TRAFFIC_FLOW_TEMPLATE_OPCODE_NO_TFT_OPERATION 0b110
#define TRAFFIC_FLOW_TEMPLATE_OPCODE_RESERVED 0b111
  uint8_t tftoperationcode : 3;
#define TRAFFIC_FLOW_TEMPLATE_PARAMETER_LIST_IS_NOT_INCLUDED 0
#define TRAFFIC_FLOW_TEMPLATE_PARAMETER_LIST_IS_INCLUDED 1
  uint8_t ebit : 1;
  uint8_t numberofpacketfilters : 4;
  packet_filter_list_t packetfilterlist;
  parameters_list_t
      parameterslist;  // The parameters list contains a variable number of
                       // parameters that may be transferred. If the parameters
                       // list is included, the E bit is set to 1; otherwise,
                       // the E bit is set to 0.
} traffic_flow_template_t;

#define TFT_ENCODE_IEI_TRUE true
#define TFT_ENCODE_IEI_FALSE false
#define TFT_ENCODE_LENGTH_TRUE true
#define TFT_ENCODE_LENGTH_FALSE false

#endif /* FILE_3GPP_24_008_SEEN */
