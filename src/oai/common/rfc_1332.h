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

/*! \file rfc_1332.h
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_RFC_1332_SEEN
#define FILE_RFC_1332_SEEN

// 2 A PPP Network Control Protocol (NCP) for IP

// Data Link Layer Protocol Field
// Exactly one IPCP packet is encapsulated in the Information field
// of PPP Data Link Layer frames where the Protocol field indicates
// type hex 8021 (IP Control Protocol)

// Code field
// Only Codes 1 through 7 (Configure-Request, Configure-Ack,
// Configure-Nak, Configure-Reject, Terminate-Request, Terminate-Ack
// and Code-Reject) are used. Other Codes should be treated as
// unrecognized and should result in Code-Rejects.
#define IPCP_CODE_CONFIGURE_REQUEST (0x01)
#define IPCP_CODE_CONFIGURE_ACK (0x02)
#define IPCP_CODE_CONFIGURE_NACK (0x03)
#define IPCP_CODE_CONFIGURE_REJECT (0x04)
#define IPCP_CODE_TERMINATE_REQUEST (0x05)
#define IPCP_CODE_TERMINATE_ACK (0x06)
#define IPCP_CODE_REJECT (0x07)

#endif
