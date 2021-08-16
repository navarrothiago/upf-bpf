/*
 * Copyright (c) 2017 Sprint
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __EPC_H
#define __EPC_H

#include <stdio.h>

#include <algorithm>
#include <string>
#include <sstream>
#include <list>
#include <vector>

/*
   MCC digit 1 - low order nibble octet 1
   MCC digit 2 - high order nibble octet 1
   MCC digit 3 - low order nibble octet 2

   MCC = [MCC digit 1][MCC digit 2][MCC digit 3]


   MNC digit 1 - low order nibble octet 3
   MNC digit 2 - high order nibble octet 3
   MNC digit 3 - high order nibble octet 2, nibble set to 1111 if MNC len is 2

   MNC = [MNC digit 1][MNC digit 2][MNC digit 3]
*/

#define LOW_NIBBLE(a) (((unsigned char) a) & 0x0f)
#define HIGH_NIBBLE(a) ((((unsigned char) a) & 0xf0) >> 4)

#define PARSE_MNC(a, b)                                                        \
  {                                                                            \
    if (HIGH_NIBBLE(b[1]) == 0x0f) {                                           \
      a[0] = '0';                                                              \
      a[1] = LOW_NIBBLE(b[2]) + '0';                                           \
      a[2] = HIGH_NIBBLE(b[2]) + '0';                                          \
      a[3] = '\0';                                                             \
    } else {                                                                   \
      a[0] = LOW_NIBBLE(b[2]) + '0';                                           \
      a[1] = HIGH_NIBBLE(b[2]) + '0';                                          \
      a[2] = HIGH_NIBBLE(b[1]) + '0';                                          \
      a[3] = '\0';                                                             \
    }                                                                          \
  }

#define PARSE_MCC(a, b)                                                        \
  {                                                                            \
    a[0] = LOW_NIBBLE(b[0]) + '0';                                             \
    a[1] = HIGH_NIBBLE(b[0]) + '0';                                            \
    a[2] = LOW_NIBBLE(b[1]) + '0';                                             \
    a[3] = '\0';                                                               \
  }

#define PARSE_PLMNID(a)                                                        \
  char mnc[4], mcc[4];                                                         \
  PARSE_MNC(mnc, a);                                                           \
  PARSE_MCC(mcc, a);

#define APPEND_MNC(a) append("mnc").append(a).append(".")
#define APPEND_MCC(a) append("mcc").append(a).append(".")
#define APPEND_3GPPNETWORK append("3gppnetwork.org")

namespace EPC {
enum AppServiceEnum {
  x_3gpp_unknown,
  x_3gpp_pgw,
  x_3gpp_sgw,
  x_3gpp_ggsn,
  x_3gpp_sgsn,
  x_3gpp_mme,
  x_3gpp_msc
};

enum AppProtocolEnum {
  x_unknown,
  x_gn,
  x_gp,
  x_nq,
  x_nqprime,
  x_s10,
  x_s11,
  x_s12,
  x_s1_mme,
  x_s1_u,
  x_s16,
  x_s2a_gtp,
  x_s2a_mipv4,
  x_s2a_pmip,
  x_s2b_gtp,
  x_s2b_pmip,
  x_s2c_dsmip,
  x_s3,
  x_s4,
  x_s5_gtp,
  x_s5_pmip,
  x_s6a,
  x_s8_gtp,
  x_s8_pmip,
  x_sv
};

enum PGWAppProtocolEnum {
  pgw_x_gn,
  pgw_x_gp,
  pgw_x_s2a_gtp,
  pgw_x_s2a_mipv4,
  pgw_x_s2a_pmip,
  pgw_x_s2b_gtp,
  pgw_x_s2b_pmip,
  pgw_x_s2c_dsmip,
  pgw_x_s5_gtp,
  pgw_x_s5_pmip,
  pgw_x_s8_gtp,
  pgw_x_s8_pmip
};

enum SGWAppProtocolEnum {
  sgw_x_s11,
  sgw_x_s12,
  sgw_x_s1_u,
  sgw_x_s2a_pmip,
  sgw_x_s2b_pmip,
  sgw_x_s4,
  sgw_x_s5_gtp,
  sgw_x_s5_pmip,
  sgw_x_s8_gtp,
  sgw_x_s8_pmip
};

enum GGSNAppProtocolEnum { ggsn_x_gn, ggsn_x_gp };

enum SGSNAppProtocolEnum {
  sgsn_x_gn,
  sgsn_x_gp,
  sgsn_x_nqprime,
  sgsn_x_s16,
  sgsn_x_s3,
  sgsn_x_s4,
  sgsn_x_sv
};

enum MMEAppProtocolEnum {
  mme_x_gn,
  mme_x_gp,
  mme_x_nq,
  mme_x_s10,
  mme_x_s11,
  mme_x_s1_mme,
  mme_x_s3,
  mme_x_s6a,
  mme_x_sv
};

enum MSCAppProtocolEnum { msc_x_sv };

enum DiameterApplicationEnum {
  dia_app_unknown,
  /*
  +------------------+----------------------------+
  | Tag              | Diameter Application       |
  +------------------+----------------------------+
  | aaa+ap1          | NASREQ [RFC3588]           |
  | aaa+ap2          | Mobile IPv4 [RFC4004]      |
  | aaa+ap3          | Base Accounting [RFC3588]  |
  | aaa+ap4          | Credit Control [RFC4006]   |
  | aaa+ap5          | EAP [RFC4072]              |
  | aaa+ap6          | SIP [RFC4740]              |
  | aaa+ap7          | Mobile IPv6 IKE [RFC5778]  |
  | aaa+ap8          | Mobile IPv6 Auth [RFC5778] |
  | aaa+ap9          | QoS [RFC5866]              |
  | aaa+ap4294967295 | Relay [RFC3588]            |
  +------------------+----------------------------+
  */
  dia_app_nasreq,
  dia_app_mobile_ipv4,
  dia_app_base_accounting,
  dia_app_credit_control,
  dia_app_eap,
  dia_app_sip6,
  dia_app_mobile_ipv6_ike,
  dia_app_mobile_ipv6_auth,
  dia_app_qos,
  dia_app_relay,
  /*
  +----------------+----------------------+
  | Tag            | Diameter Application |
  +----------------+----------------------+
  | aaa+ap16777250 | 3GPP STa [TS29.273]  |
  | aaa+ap16777251 | 3GPP S6a [TS29.272]  |
  | aaa+ap16777264 | 3GPP SWm [TS29.273]  |
  | aaa+ap16777267 | 3GPP S9 [TS29.215]   |
  +----------------+----------------------+
  */
  dia_app_3gpp_sta,
  dia_app_3gpp_s6a,
  dia_app_3gpp_swm,
  dia_app_3gpp_s9,
  /*
  +----------------+--------------------------------------------------+
  | Tag            | Diameter Application                             |
  +----------------+--------------------------------------------------+
  | aaa+ap16777281 | WiMAX Network Access Authentication and          |
  |                | Authorization Diameter Application (WNAAADA)     |
  |                | [WiMAX-BASE]                                     |
  | aaa+ap16777282 | WiMAX Network Accounting Diameter Application    |
  |                | (WNADA) [WiMAX-BASE]                             |
  | aaa+ap16777283 | WiMAX MIP4 Diameter Application (WM4DA)          |
  |                | [WiMAX-BASE]                                     |
  | aaa+ap16777284 | WiMAX MIP6 Diameter Application (WM6DA)          |
  |                | [WiMAX-BASE]                                     |
  | aaa+ap16777285 | WiMAX DHCP Diameter Application (WDDA)           |
  |                | [WiMAX-BASE]                                     |
  | aaa+ap16777286 | WiMAX Location Authentication Authorization      |
  |                | Diameter Application (WLAADA) [WiMAX-LBS]        |
  | aaa+ap16777287 | WiMAX Policy and Charging Control R3 Policies    |
  |                | Diameter Application (WiMAX PCC-R3-P)            |
  |                | [WiMAX-PCC]                                      |
  | aaa+ap16777288 | WiMAX Policy and Charging Control R3 Offline     |
  |                | Charging Diameter Application (WiMAX PCC-R3-OFC) |
  |                | [WiMAX-PCC]                                      |
  | aaa+ap16777289 | WiMAX Policy and Charging Control R3 Offline     |
  |                | Charging Prime Diameter Application (WiMAX       |
  |                | PCC-R3-OFC-PRIME) [WiMAX-PCC]                    |
  | aaa+ap16777290 | WiMAX Policy and Charging Control R3 Online      |
  |                | Charging Diameter Application (WiMAX PCC-R3-OC)  |
  |                | [WiMAX-PCC]                                      |
  +----------------+--------------------------------------------------+
  */
  dia_app_wimax_wnaaada,
  dia_app_wimax_wnada,
  dia_app_wimax_wm4da,
  dia_app_wimax_wm6da,
  dia_app_wimax_wdda,
  dia_app_wimax_wlaada,
  dia_app_wimax_pcc_r3_p,
  dia_app_wimax_pcc_r3_ofc,
  dia_app_wimax_pcc_r3_ofc_prime,
  dia_app_wimax_pcc_r3_oc
};

enum DiameterProtocolEnum {
  dia_protocol_unknown,
  dia_protocol_tcp,
  dia_protocol_sctp,
  dia_protocol_tls_tcp
};

class Utility {
 public:
  static std::string home_network(const char* mnc, const char* mcc);
  static std::string home_network(const unsigned char* plmnid);
  static std::string home_network_gprs(const char* mnc, const char* mcc);
  static std::string home_network_gprs(const unsigned char* plmnid);
  static std::string tai_fqdn(
      const char* lb, const char* hb, const char* mnc, const char* mcc);
  static std::string tai_fqdn(
      const char* lb, const char* hb, const unsigned char* plmnid);
  static std::string mme_fqdn(
      const char* mmec, const char* mmegi, const char* mnc, const char* mcc);
  static std::string mme_fqdn(
      const char* mmec, const char* mmegi, const unsigned char* plmnid);
  static std::string mme_pool_fqdn(
      const char* mmegi, const char* mnc, const char* mcc);
  static std::string mme_pool_fqdn(
      const char* mmegi, const unsigned char* plmnid);
  static std::string rai_fqdn(
      const char* rac, const char* lac, const char* mnc, const char* mcc);
  static std::string rai_fqdn(
      const char* rac, const char* lac, const unsigned char* plmnid);
  static std::string rnc_fqdn(
      const char* rnc, const char* mnc, const char* mcc);
  static std::string rnc_fqdn(const char* rnc, const unsigned char* plmnid);
  static std::string sgsn_fqdn(
      const char* nri, const char* rac, const char* lac, const char* mnc,
      const char* mcc);
  static std::string sgsn_fqdn(
      const char* nri, const char* rac, const char* lac,
      const unsigned char* plmnid);
  static std::string epc_nodes_domain_fqdn(const char* mnc, const char* mcc);
  static std::string epc_nodes_domain_fqdn(const unsigned char* plmnid);
  static std::string epc_node_fqdn(
      const char* node, const char* mnc, const char* mcc);
  static std::string epc_node_fqdn(
      const char* node, const unsigned char* plmnid);
  static std::string nonemergency_epdg_oi_fqdn(
      const char* mnc, const char* mcc);
  static std::string nonemergency_epdg_oi_fqdn(const unsigned char* plmnid);
  static std::string nonemergency_epdg_tai_fqdn(
      const char* lb, const char* hb, const char* mnc, const char* mcc);
  static std::string nonemergency_epdg_tai_fqdn(
      const char* lb, const char* hb, const unsigned char* plmnid);
  static std::string nonemergency_epdg_lac_fqdn(
      const char* lac, const char* mnc, const char* mcc);
  static std::string nonemergency_epdg_lac_fqdn(
      const char* lac, const unsigned char* plmnid);
  static std::string nonemergency_epdg_visitedcountry_fqdn(const char* mcc);
  static std::string nonemergency_epdg_visitedcountry_fqdn(
      const unsigned char* plmnid);
  static std::string emergency_epdg_oi_fqdn(const char* mnc, const char* mcc);
  static std::string emergency_epdg_oi_fqdn(const unsigned char* plmnid);
  static std::string emergency_epdg_tai_fqdn(
      const char* lb, const char* hb, const char* mnc, const char* mcc);
  static std::string emergency_epdg_tai_fqdn(
      const char* lb, const char* hb, const unsigned char* plmnid);
  static std::string emergency_epdg_lac_fqdn(
      const char* lac, const char* mnc, const char* mcc);
  static std::string emergency_epdg_lac_fqdn(
      const char* lac, const unsigned char* plmnid);
  static std::string emergency_epdg_visitedcountry_fqdn(const char* mcc);
  static std::string emergency_epdg_visitedcountry_fqdn(
      const unsigned char* plmnid);
  static std::string global_enodeb_id_fqdn(const char* enb, const char* mcc);
  static std::string global_enodeb_id_fqdn(
      const char* enb, const unsigned char* plmnid);
  static std::string local_homenetwork_fqdn(const char* lhn, const char* mcc);
  static std::string local_homenetwork_fqdn(
      const char* lhn, const unsigned char* plmnid);
  static std::string epc(const char* mnc, const char* mcc);
  static std::string epc(const unsigned char* plmnid);
  static std::string apn_fqdn(
      const char* apnoi, const char* mnc, const char* mcc);
  static std::string apn_fqdn(const char* apnoi, const unsigned char* plmnid);
  static std::string apn(const char* apnoi, const char* mnc, const char* mcc);
  static std::string apn(const char* apnoi, const unsigned char* plmnid);
  static std::string apn_label(const std::string& apn);

  static AppServiceEnum getAppService(const std::string& s);
  static AppProtocolEnum getAppProtocol(const std::string& p);

  static const char* getAppService(AppServiceEnum s);
  static const char* getAppProtocol(AppProtocolEnum proto);

  static AppProtocolEnum getAppProtocol(PGWAppProtocolEnum proto);
  static AppProtocolEnum getAppProtocol(SGWAppProtocolEnum proto);
  static AppProtocolEnum getAppProtocol(GGSNAppProtocolEnum proto);
  static AppProtocolEnum getAppProtocol(SGSNAppProtocolEnum proto);
  static AppProtocolEnum getAppProtocol(MMEAppProtocolEnum proto);
  static AppProtocolEnum getAppProtocol(MSCAppProtocolEnum proto);

  static std::string diameter_fqdn(const char* mnc, const char* mcc);
  static std::string diameter_fqdn(const unsigned char* plmnid);

  static uint32_t getDiameterApplication(DiameterApplicationEnum app);
  static const char* getDiameterProtocol(DiameterProtocolEnum protocol);

  static std::string getDiameterService(
      DiameterApplicationEnum app, DiameterProtocolEnum protocol);

 private:
  Utility() {}
};

}  // namespace EPC

#endif  // #ifndef __EPC_H
