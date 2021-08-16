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

/*! \file msg_pfcp.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef MSG_PFCP_HPP_INCLUDED_
#define MSG_PFCP_HPP_INCLUDED_

#include "3gpp_29.244.h"
#include "common_defs.h"

#include <utility>
#include <vector>

// Only Grouped IEs of this namespace
namespace pfcp {
class create_pdr;
class pdi;
class create_far;
class update_bar_within_pfcp_session_report_response;
class create_urr;
class create_qer;
class created_pdr;
class update_pdr;
class update_far;
class update_forwarding_parameters;
class forwarding_parameters;
class update_urr;
class update_qer;
class remove_pdr;
class remove_far;
class remove_urr;
class remove_qer;
class load_control_information;
class overload_control_information;
class application_ids_pfds;
class pfd;
class application_detection_information;
class query_urr;
class usage_report_within_pfcp_session_modification_response;
class usage_report_within_pfcp_session_deletion_response;
class usage_report_within_pfcp_session_report_request;
class downlink_data_report;
class create_bar;
class update_bar_within_pfcp_session_modification_request;
class remove_bar;
class error_indication_report;
class user_plane_path_failure_report;
class update_duplicating_parameters;
class duplicating_parameters;
class aggregated_urrs;
class create_traffic_endpoint;
class created_traffic_endpoint;
class update_traffic_endpoint;
class remove_traffic_endpoint;
class ethernet_traffic_information;
class additional_monitoring_time;
class event_information;
class event_reporting;
class ethernet_packet_filter;
}  // namespace pfcp

namespace pfcp {

// For ease of debugging
class pfcp_ies_container {
 public:
  static const uint8_t msg_id = 0;

  //  PFCP_IE_CREATE_PDR
  virtual bool get(pfcp::create_pdr& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_PDR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::create_pdr& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_PDR, __FILE__, __LINE__);
  }
  // PFCP_IE_PDI
  virtual bool get(pfcp::pdi& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_PDI, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::pdi& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_PDI, __FILE__, __LINE__);
  }
  //  PFCP_IE_CREATE_FAR
  virtual bool get(pfcp::create_far& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_FAR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::create_far& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_FAR, __FILE__, __LINE__);
  }
  //  PFCP_IE_FORWARDING_PARAMETERS
  virtual bool get(pfcp::forwarding_parameters& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FORWARDING_PARAMETERS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::forwarding_parameters& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FORWARDING_PARAMETERS, __FILE__, __LINE__);
  }
  //  PFCP_IE_DUPLICATING_PARAMETERS
  virtual bool get(pfcp::duplicating_parameters& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DUPLICATING_PARAMETERS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::duplicating_parameters& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DUPLICATING_PARAMETERS, __FILE__, __LINE__);
  }
  //  PFCP_IE_CREATE_URR
  virtual bool get(pfcp::create_urr& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_URR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::create_urr& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_URR, __FILE__, __LINE__);
  }
  //  PFCP_IE_CREATE_QER
  virtual bool get(pfcp::create_qer& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_QER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::create_qer& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_QER, __FILE__, __LINE__);
  }
  //  PFCP_IE_CREATED_PDR
  virtual bool get(pfcp::created_pdr& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATED_PDR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::created_pdr& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATED_PDR, __FILE__, __LINE__);
  }
  //  PFCP_IE_UPDATE_PDR
  virtual bool get(pfcp::update_pdr& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_PDR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::update_pdr& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_PDR, __FILE__, __LINE__);
  }
  //  PFCP_IE_UPDATE_FAR
  virtual bool get(pfcp::update_far& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_FAR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::update_far& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_FAR, __FILE__, __LINE__);
  }
  //  PFCP_IE_UPDATE_FORWARDING_PARAMETERS
  virtual bool get(pfcp::update_forwarding_parameters& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_FORWARDING_PARAMETERS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::update_forwarding_parameters& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_FORWARDING_PARAMETERS, __FILE__, __LINE__);
  }
  //  PFCP_IE_UPDATE_BAR_
  virtual bool get(
      pfcp::update_bar_within_pfcp_session_modification_request& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_BAR_WITHIN_PFCP_SESSION_MODIFICATION_REQUEST,
        __FILE__, __LINE__);
  }
  virtual void set(
      const pfcp::update_bar_within_pfcp_session_modification_request& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_BAR_WITHIN_PFCP_SESSION_MODIFICATION_REQUEST,
        __FILE__, __LINE__);
  }
  virtual bool get(
      pfcp::update_bar_within_pfcp_session_report_response& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_BAR_WITHIN_PFCP_SESSION_REPORT_RESPONSE, __FILE__,
        __LINE__);
  }
  virtual void set(
      const pfcp::update_bar_within_pfcp_session_report_response& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_BAR_WITHIN_PFCP_SESSION_REPORT_RESPONSE, __FILE__,
        __LINE__);
  }
  //  PFCP_IE_UPDATE_URR
  virtual bool get(pfcp::update_urr& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_URR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::update_urr& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_URR, __FILE__, __LINE__);
  }
  //  PFCP_IE_UPDATE_QER
  virtual bool get(pfcp::update_qer& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_QER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::update_qer& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_QER, __FILE__, __LINE__);
  }
  //  PFCP_IE_REMOVE_PDR
  virtual bool get(pfcp::remove_pdr& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_PDR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::remove_pdr& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_PDR, __FILE__, __LINE__);
  }
  //  PFCP_IE_REMOVE_FAR
  virtual bool get(pfcp::remove_far& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_FAR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::remove_far& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_FAR, __FILE__, __LINE__);
  }
  //  PFCP_IE_REMOVE_URR
  virtual bool get(pfcp::remove_urr& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_URR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::remove_urr& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_URR, __FILE__, __LINE__);
  }
  //  PFCP_IE_REMOVE_QER
  virtual bool get(pfcp::remove_qer& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_QER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::remove_qer& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_QER, __FILE__, __LINE__);
  }
  //  PFCP_IE_CAUSE
  virtual bool get(pfcp::cause_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_CAUSE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::cause_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_CAUSE, __FILE__, __LINE__);
  }
  //  PFCP_IE_SOURCE_INTERFACE
  virtual bool get(pfcp::source_interface_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SOURCE_INTERFACE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::source_interface_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SOURCE_INTERFACE, __FILE__, __LINE__);
  }
  //  PFCP_IE_F_TEID
  virtual bool get(pfcp::fteid_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_F_TEID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::fteid_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_F_TEID, __FILE__, __LINE__);
  }
  //  PFCP_IE_NETWORK_INSTANCE
  virtual bool get(pfcp::network_instance_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_NETWORK_INSTANCE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::network_instance_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_NETWORK_INSTANCE, __FILE__, __LINE__);
  }
  //  PFCP_IE_SDF_FILTER
  virtual bool get(pfcp::sdf_filter_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SDF_FILTER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::sdf_filter_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SDF_FILTER, __FILE__, __LINE__);
  }
  //  PFCP_IE_APPLICATION_ID
  virtual bool get(pfcp::application_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_APPLICATION_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::application_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_APPLICATION_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_GATE_STATUS
  virtual bool get(pfcp::gate_status_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_GATE_STATUS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::gate_status_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_GATE_STATUS, __FILE__, __LINE__);
  }
  //  PFCP_IE_MBR
  virtual bool get(pfcp::mbr_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_MBR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::mbr_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_MBR, __FILE__, __LINE__);
  }
  //  PFCP_IE_GBR
  virtual bool get(pfcp::gbr_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_GBR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::gbr_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_GBR, __FILE__, __LINE__);
  }
  //  PFCP_IE_QER_CORRELATION_ID
  virtual bool get(pfcp::qer_correlation_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_QER_CORRELATION_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::qer_correlation_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_QER_CORRELATION_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_PRECEDENCE
  virtual bool get(pfcp::precedence_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PRECEDENCE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::precedence_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PRECEDENCE, __FILE__, __LINE__);
  }
  //  PFCP_IE_TRANSPORT_LEVEL_MARKING
  virtual bool get(pfcp::transport_level_marking_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TRANSPORT_LEVEL_MARKING, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::transport_level_marking_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TRANSPORT_LEVEL_MARKING, __FILE__, __LINE__);
  }
  //  PFCP_IE_VOLUME_THRESHOLD
  virtual bool get(pfcp::volume_threshold_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_VOLUME_THRESHOLD, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::volume_threshold_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_VOLUME_THRESHOLD, __FILE__, __LINE__);
  }
  //  PFCP_IE_TIME_THRESHOLD
  virtual bool get(pfcp::time_threshold_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TIME_THRESHOLD, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::time_threshold_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TIME_THRESHOLD, __FILE__, __LINE__);
  }
  //  PFCP_IE_MONITORING_TIME
  virtual bool get(pfcp::monitoring_time_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MONITORING_TIME, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::monitoring_time_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MONITORING_TIME, __FILE__, __LINE__);
  }
  //  PFCP_IE_SUBSEQUENT_VOLUME_THRESHOLD
  virtual bool get(pfcp::subsequent_volume_threshold_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SUBSEQUENT_VOLUME_THRESHOLD, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::subsequent_volume_threshold_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SUBSEQUENT_VOLUME_THRESHOLD, __FILE__, __LINE__);
  }
  //  PFCP_IE_SUBSEQUENT_TIME_THRESHOLD
  virtual bool get(pfcp::subsequent_time_threshold_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SUBSEQUENT_TIME_THRESHOLD, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::subsequent_time_threshold_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SUBSEQUENT_TIME_THRESHOLD, __FILE__, __LINE__);
  }
  //  PFCP_IE_INACTIVITY_DETECTION_TIME
  virtual bool get(pfcp::inactivity_detection_time_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_INACTIVITY_DETECTION_TIME, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::inactivity_detection_time_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_INACTIVITY_DETECTION_TIME, __FILE__, __LINE__);
  }
  //  PFCP_IE_REPORTING_TRIGGERS
  virtual bool get(pfcp::reporting_triggers_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REPORTING_TRIGGERS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::reporting_triggers_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REPORTING_TRIGGERS, __FILE__, __LINE__);
  }
  //  PFCP_IE_REDIRECT_INFORMATION
  virtual bool get(pfcp::redirect_information_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REDIRECT_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::redirect_information_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REDIRECT_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_REPORT_TYPE
  virtual bool get(pfcp::report_type_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REPORT_TYPE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::report_type_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REPORT_TYPE, __FILE__, __LINE__);
  }
  //  PFCP_IE_OFFENDING_IE
  virtual bool get(pfcp::offending_ie_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_OFFENDING_IE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::offending_ie_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_OFFENDING_IE, __FILE__, __LINE__);
  }
  //  PFCP_IE_FORWARDING_POLICY
  virtual bool get(pfcp::forwarding_policy_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FORWARDING_POLICY, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::forwarding_policy_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FORWARDING_POLICY, __FILE__, __LINE__);
  }
  //  PFCP_IE_DESTINATION_INTERFACE
  virtual bool get(pfcp::destination_interface_t v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DESTINATION_INTERFACE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::destination_interface_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DESTINATION_INTERFACE, __FILE__, __LINE__);
  }
  // PFCP_IE_UP_FUNCTION_FEATURES
  virtual bool get(pfcp::up_function_features_s& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UP_FUNCTION_FEATURES, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::up_function_features_s& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UP_FUNCTION_FEATURES, __FILE__, __LINE__);
  }
  //  PFCP_IE_APPLY_ACTION
  virtual bool get(pfcp::apply_action_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_APPLY_ACTION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::apply_action_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_APPLY_ACTION, __FILE__, __LINE__);
  }
  //  PFCP_IE_DOWNLINK_DATA_SERVICE_INFORMATION
  virtual bool get(pfcp::downlink_data_service_information_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DOWNLINK_DATA_SERVICE_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::downlink_data_service_information_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DOWNLINK_DATA_SERVICE_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_DOWNLINK_DATA_NOTIFICATION_DELAY
  virtual bool get(pfcp::downlink_data_notification_delay_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DOWNLINK_DATA_NOTIFICATION_DELAY, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::downlink_data_notification_delay_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DOWNLINK_DATA_NOTIFICATION_DELAY, __FILE__, __LINE__);
  }
  //  PFCP_IE_DL_BUFFERING_DURATION
  virtual bool get(pfcp::dl_buffering_duration_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DL_BUFFERING_DURATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::dl_buffering_duration_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DL_BUFFERING_DURATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_DL_BUFFERING_SUGGESTED_PACKET_COUNT
  virtual bool get(pfcp::dl_buffering_suggested_packet_count_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DL_BUFFERING_SUGGESTED_PACKET_COUNT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::dl_buffering_suggested_packet_count_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DL_BUFFERING_SUGGESTED_PACKET_COUNT, __FILE__, __LINE__);
  }
  //  PFCP_IE_PFCPSMREQ_FLAGS
  virtual bool get(pfcp::pfcpsmreq_flags_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PFCPSMREQ_FLAGS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::pfcpsmreq_flags_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PFCPSMREQ_FLAGS, __FILE__, __LINE__);
  }
  //  PFCP_IE_PFCPSRRSP_FLAGS
  virtual bool get(pfcp::pfcpsrrsp_flags_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PFCPSRRSP_FLAGS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::pfcpsrrsp_flags_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PFCPSRRSP_FLAGS, __FILE__, __LINE__);
  }
  //  PFCP_IE_LOAD_CONTROL_INFORMATION
  virtual bool get(pfcp::load_control_information& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_LOAD_CONTROL_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::load_control_information& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_LOAD_CONTROL_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_SEQUENCE_NUMBER
  virtual bool get(pfcp::sequence_number_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SEQUENCE_NUMBER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::sequence_number_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SEQUENCE_NUMBER, __FILE__, __LINE__);
  }
  //  PFCP_IE_METRIC
  virtual bool get(pfcp::metric_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_METRIC, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::metric_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_METRIC, __FILE__, __LINE__);
  }
  //  PFCP_IE_OVERLOAD_CONTROL_INFORMATION
  virtual bool get(pfcp::overload_control_information& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_OVERLOAD_CONTROL_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::overload_control_information& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_OVERLOAD_CONTROL_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_TIMER
  virtual bool get(pfcp::timer_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_TIMER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::timer_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_TIMER, __FILE__, __LINE__);
  }
  //  PFCP_IE_PACKET_DETECTION_RULE_ID
  virtual bool get(pfcp::pdr_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PACKET_DETECTION_RULE_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::pdr_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PACKET_DETECTION_RULE_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_F_SEID
  virtual bool get(pfcp::fseid_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_F_SEID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::fseid_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_F_SEID, __FILE__, __LINE__);
  }
  // PFCP_IE_APPLICATION_IDS_PFDS
  virtual bool get(pfcp::application_ids_pfds& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_APPLICATION_IDS_PFDS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::application_ids_pfds& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_APPLICATION_IDS_PFDS, __FILE__, __LINE__);
  }
  //  PFCP_IE_PFD
  virtual bool get(pfcp::pfd& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_PFD, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::pfd& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_PFD, __FILE__, __LINE__);
  }
  //  PFCP_IE_NODE_ID
  virtual bool get(pfcp::node_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_NODE_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::node_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_NODE_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_PFD_CONTENTS
  virtual bool get(pfcp::pfd_contents_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PFD_CONTENTS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::pfd_contents_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PFD_CONTENTS, __FILE__, __LINE__);
  }
  //  PFCP_IE_MEASUREMENT_METHOD
  virtual bool get(pfcp::measurement_method_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MEASUREMENT_METHOD, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::measurement_method_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MEASUREMENT_METHOD, __FILE__, __LINE__);
  }
  //  PFCP_IE_USAGE_REPORT_TRIGGER
  virtual bool get(pfcp::usage_report_trigger_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USAGE_REPORT_TRIGGER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::usage_report_trigger_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USAGE_REPORT_TRIGGER, __FILE__, __LINE__);
  }
  //  PFCP_IE_MEASUREMENT_PERIOD
  virtual bool get(pfcp::measurement_period_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MEASUREMENT_PERIOD, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::measurement_period_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MEASUREMENT_PERIOD, __FILE__, __LINE__);
  }
  //  PFCP_IE_FQ_CSID
  virtual bool get(fq_csid_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_FQ_CSID, __FILE__, __LINE__);
  }
  virtual void set(const fq_csid_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_FQ_CSID, __FILE__, __LINE__);
  }
  //  PFCP_IE_VOLUME_MEASUREMENT
  virtual bool get(pfcp::volume_measurement_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_VOLUME_MEASUREMENT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::volume_measurement_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_VOLUME_MEASUREMENT, __FILE__, __LINE__);
  }
  //  PFCP_IE_DURATION_MEASUREMENT
  virtual bool get(pfcp::duration_measurement_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DURATION_MEASUREMENT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::duration_measurement_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DURATION_MEASUREMENT, __FILE__, __LINE__);
  }
  //  PFCP_IE_APPLICATION_DETECTION_INFORMATION
  virtual bool get(pfcp::application_detection_information& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_APPLICATION_DETECTION_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::application_detection_information& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_APPLICATION_DETECTION_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_TIME_OF_FIRST_PACKET
  virtual bool get(pfcp::time_of_first_packet_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TIME_OF_FIRST_PACKET, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::time_of_first_packet_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TIME_OF_FIRST_PACKET, __FILE__, __LINE__);
  }
  //  PFCP_IE_TIME_OF_LAST_PACKET
  virtual bool get(pfcp::time_of_last_packet_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TIME_OF_LAST_PACKET, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::time_of_last_packet_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TIME_OF_LAST_PACKET, __FILE__, __LINE__);
  }
  //  PFCP_IE_QUOTA_HOLDING_TIME
  virtual bool get(pfcp::quota_holding_time_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_QUOTA_HOLDING_TIME, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::quota_holding_time_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_QUOTA_HOLDING_TIME, __FILE__, __LINE__);
  }
  //  PFCP_IE_DROPPED_DL_TRAFFIC_THRESHOLD
  virtual bool get(pfcp::dropped_dl_traffic_threshold_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DROPPED_DL_TRAFFIC_THRESHOLD, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::dropped_dl_traffic_threshold_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DROPPED_DL_TRAFFIC_THRESHOLD, __FILE__, __LINE__);
  }
  //  PFCP_IE_VOLUME_QUOTA
  virtual bool get(pfcp::volume_quota_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_VOLUME_QUOTA, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::volume_quota_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_VOLUME_QUOTA, __FILE__, __LINE__);
  }
  //  PFCP_IE_TIME_QUOTA
  virtual bool get(pfcp::time_quota_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TIME_QUOTA, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::time_quota_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TIME_QUOTA, __FILE__, __LINE__);
  }
  //  PFCP_IE_START_TIME
  virtual bool get(pfcp::start_time_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_START_TIME, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::start_time_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_START_TIME, __FILE__, __LINE__);
  }
  //  PFCP_IE_END_TIME
  virtual bool get(pfcp::end_time_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_END_TIME, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::end_time_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_END_TIME, __FILE__, __LINE__);
  }
  //  PFCP_IE_QUERY_URR
  virtual bool get(pfcp::query_urr& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_QUERY_URR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::query_urr& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_QUERY_URR, __FILE__, __LINE__);
  }
  //  PFCP_IE_USAGE_REPORT_WITHIN_SESSION_MODIFICATION_RESPONSE
  virtual bool get(
      pfcp::usage_report_within_pfcp_session_modification_response& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USAGE_REPORT_WITHIN_SESSION_MODIFICATION_RESPONSE, __FILE__,
        __LINE__);
  }
  virtual void set(
      const pfcp::usage_report_within_pfcp_session_modification_response& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USAGE_REPORT_WITHIN_SESSION_MODIFICATION_RESPONSE, __FILE__,
        __LINE__);
  }
  //  PFCP_IE_USAGE_REPORT_WITHIN_SESSION_DELETION_RESPONSE
  virtual bool get(
      pfcp::usage_report_within_pfcp_session_deletion_response& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USAGE_REPORT_WITHIN_SESSION_DELETION_RESPONSE, __FILE__,
        __LINE__);
  }
  virtual void set(
      const pfcp::usage_report_within_pfcp_session_deletion_response& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USAGE_REPORT_WITHIN_SESSION_DELETION_RESPONSE, __FILE__,
        __LINE__);
  }
  //  PFCP_IE_USAGE_REPORT_SESSION_REPORT_REQUEST
  virtual bool get(
      pfcp::usage_report_within_pfcp_session_report_request& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USAGE_REPORT_WITHIN_SESSION_REPORT_REQUEST, __FILE__,
        __LINE__);
  }
  virtual void set(
      const pfcp::usage_report_within_pfcp_session_report_request& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USAGE_REPORT_WITHIN_SESSION_REPORT_REQUEST, __FILE__,
        __LINE__);
  }
  //  PFCP_IE_URR_ID
  virtual bool get(pfcp::urr_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_URR_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::urr_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_URR_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_LINKED_URR_ID
  virtual bool get(pfcp::linked_urr_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_LINKED_URR_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::linked_urr_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_LINKED_URR_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_DOWNLINK_DATA_REPORT
  virtual bool get(pfcp::downlink_data_report& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DOWNLINK_DATA_REPORT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::downlink_data_report& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DOWNLINK_DATA_REPORT, __FILE__, __LINE__);
  }
  //  PFCP_IE_OUTER_HEADER_CREATION
  virtual bool get(pfcp::outer_header_creation_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_OUTER_HEADER_CREATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::outer_header_creation_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_OUTER_HEADER_CREATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_CREATE_BAR
  virtual bool get(pfcp::create_bar& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_BAR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::create_bar& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_BAR, __FILE__, __LINE__);
  }
  //  PFCP_IE_UPDATE_BAR_SESSION_MODIFICATION_REQUEST
  // TODO virtual bool get(pfcp::update_bar_session_modification_request_t& v)
  // const {throw pfcp_msg_illegal_ie_exception(0,
  // PFCP_IE_UPDATE_BAR_SESSION_MODIFICATION_REQUEST, __FILE__, __LINE__);}
  // TODO virtual void set(const
  // pfcp::update_bar_session_modification_request_t& v) {throw
  // pfcp_msg_illegal_ie_exception(0,
  // PFCP_IE_UPDATE_BAR_SESSION_MODIFICATION_REQUEST, __FILE__, __LINE__);}
  //  PFCP_IE_REMOVE_BAR
  virtual bool get(pfcp::remove_bar& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_BAR, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::remove_bar& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_BAR, __FILE__, __LINE__);
  }
  //  PFCP_IE_BAR_ID
  virtual bool get(pfcp::bar_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_BAR_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::bar_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_BAR_ID, __FILE__, __LINE__);
  }
  // PFCP_IE_CP_FUNCTION_FEATURES
  virtual bool get(pfcp::cp_function_features_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CP_FUNCTION_FEATURES, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::cp_function_features_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CP_FUNCTION_FEATURES, __FILE__, __LINE__);
  }
  //  PFCP_IE_USAGE_INFORMATION
  virtual bool get(pfcp::usage_information_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USAGE_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::usage_information_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USAGE_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_APPLICATION_INSTANCE_ID
  virtual bool get(pfcp::application_instance_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_APPLICATION_INSTANCE_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::application_instance_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_APPLICATION_INSTANCE_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_FLOW_INFORMATION
  virtual bool get(pfcp::flow_information_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FLOW_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::flow_information_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FLOW_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_UE_IP_ADDRESS
  virtual bool get(pfcp::ue_ip_address_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UE_IP_ADDRESS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::ue_ip_address_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UE_IP_ADDRESS, __FILE__, __LINE__);
  }
  //  PFCP_IE_PACKET_RATE
  virtual bool get(pfcp::packet_rate_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PACKET_RATE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::packet_rate_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PACKET_RATE, __FILE__, __LINE__);
  }
  //  PFCP_IE_OUTER_HEADER_REMOVAL
  virtual bool get(pfcp::outer_header_removal_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_OUTER_HEADER_REMOVAL, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::outer_header_removal_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_OUTER_HEADER_REMOVAL, __FILE__, __LINE__);
  }
  // PFCP_IE_RECOVERY_TIME_STAMP
  virtual bool get(pfcp::recovery_time_stamp_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_RECOVERY_TIME_STAMP, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::recovery_time_stamp_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_RECOVERY_TIME_STAMP, __FILE__, __LINE__);
  }
  //  PFCP_IE_DL_FLOW_LEVEL_MARKING
  virtual bool get(pfcp::dl_flow_level_marking_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DL_FLOW_LEVEL_MARKING, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::dl_flow_level_marking_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DL_FLOW_LEVEL_MARKING, __FILE__, __LINE__);
  }
  //  PFCP_IE_HEADER_ENRICHMENT
  virtual bool get(pfcp::header_enrichment_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_HEADER_ENRICHMENT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::header_enrichment_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_HEADER_ENRICHMENT, __FILE__, __LINE__);
  }
  //  PFCP_IE_ERROR_INDICATION_REPORT
  virtual bool get(pfcp::error_indication_report& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ERROR_INDICATION_REPORT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::error_indication_report& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ERROR_INDICATION_REPORT, __FILE__, __LINE__);
  }
  //  PFCP_IE_MEASUREMENT_INFORMATION
  virtual bool get(pfcp::measurement_information_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MEASUREMENT_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::measurement_information_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MEASUREMENT_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_NODE_REPORT_TYPE
  virtual bool get(pfcp::node_report_type_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_NODE_REPORT_TYPE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::node_report_type_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_NODE_REPORT_TYPE, __FILE__, __LINE__);
  }
  //  PFCP_IE_USER_PLANE_PATH_FAILURE_REPORT
  virtual bool get(pfcp::user_plane_path_failure_report& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USER_PLANE_PATH_FAILURE_REPORT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::user_plane_path_failure_report& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USER_PLANE_PATH_FAILURE_REPORT, __FILE__, __LINE__);
  }
  //  PFCP_IE_REMOTE_GTP_U_PEER
  virtual bool get(pfcp::remote_gtp_u_peer_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOTE_GTP_U_PEER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::remote_gtp_u_peer_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOTE_GTP_U_PEER, __FILE__, __LINE__);
  }
  //  PFCP_IE_UR_SEQN
  virtual bool get(pfcp::ur_seqn_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_UR_SEQN, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::ur_seqn_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_UR_SEQN, __FILE__, __LINE__);
  }
  //  PFCP_IE_UPDATE_DUPLICATING_PARAMETERS
  virtual bool get(pfcp::update_duplicating_parameters& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_DUPLICATING_PARAMETERS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::update_duplicating_parameters& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_DUPLICATING_PARAMETERS, __FILE__, __LINE__);
  }
  //  PFCP_IE_ACTIVATE_PREDEFINED_RULES
  virtual bool get(pfcp::activate_predefined_rules_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ACTIVATE_PREDEFINED_RULES, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::activate_predefined_rules_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ACTIVATE_PREDEFINED_RULES, __FILE__, __LINE__);
  }
  //  PFCP_IE_DEACTIVATE_PREDEFINED_RULES
  virtual bool get(pfcp::deactivate_predefined_rules_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DEACTIVATE_PREDEFINED_RULES, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::deactivate_predefined_rules_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_DEACTIVATE_PREDEFINED_RULES, __FILE__, __LINE__);
  }
  //  PFCP_IE_FAR_ID
  virtual bool get(pfcp::far_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_FAR_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::far_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_FAR_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_QER_ID
  virtual bool get(pfcp::qer_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_QER_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::qer_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_QER_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_OCI_FLAGS
  virtual bool get(pfcp::oci_flags_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_OCI_FLAGS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::oci_flags_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_OCI_FLAGS, __FILE__, __LINE__);
  }
  // PFCP_IE_PFCP_ASSOCIATION_RELEASE_REQUEST
  virtual bool get(pfcp::pfcp_association_release_request_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PFCP_ASSOCIATION_RELEASE_REQUEST, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::pfcp_association_release_request_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PFCP_ASSOCIATION_RELEASE_REQUEST, __FILE__, __LINE__);
  }
  // PFCP_IE_GRACEFUL_RELEASE_PERIOD
  virtual bool get(pfcp::graceful_release_period_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_GRACEFUL_RELEASE_PERIOD, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::graceful_release_period_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_GRACEFUL_RELEASE_PERIOD, __FILE__, __LINE__);
  }
  //  PFCP_IE_PDN_TYPE
  virtual bool get(pfcp::pdn_type_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PDN_TYPE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::pdn_type_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PDN_TYPE, __FILE__, __LINE__);
  }
  //  PFCP_IE_FAILED_RULE_ID
  virtual bool get(pfcp::failed_rule_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FAILED_RULE_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::failed_rule_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FAILED_RULE_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_TIME_QUOTA_MECHANISM
  virtual bool get(pfcp::time_quota_mechanism_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TIME_QUOTA_MECHANISM, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::time_quota_mechanism_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TIME_QUOTA_MECHANISM, __FILE__, __LINE__);
  }
  // PFCP_IE_USER_PLANE_IP_RESOURCE_INFORMATION
  virtual bool get(pfcp::user_plane_ip_resource_information_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USER_PLANE_IP_RESOURCE_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::user_plane_ip_resource_information_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USER_PLANE_IP_RESOURCE_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_USER_PLANE_INACTIVITY_TIMER
  virtual bool get(pfcp::user_plane_inactivity_timer_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USER_PLANE_INACTIVITY_TIMER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::user_plane_inactivity_timer_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_USER_PLANE_INACTIVITY_TIMER, __FILE__, __LINE__);
  }
  //  PFCP_IE_AGGREGATED_URRS
  virtual bool get(pfcp::aggregated_urrs& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_AGGREGATED_URRS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::aggregated_urrs& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_AGGREGATED_URRS, __FILE__, __LINE__);
  }
  //  PFCP_IE_MULTIPLIER
  virtual bool get(pfcp::multiplier_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MULTIPLIER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::multiplier_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MULTIPLIER, __FILE__, __LINE__);
  }
  //  PFCP_IE_AGGREGATED_URR_ID
  virtual bool get(pfcp::aggregated_urr_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_AGGREGATED_URR_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::aggregated_urr_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_AGGREGATED_URR_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_SUBSEQUENT_VOLUME_QUOTA
  virtual bool get(pfcp::subsequent_volume_quota_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SUBSEQUENT_VOLUME_QUOTA, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::subsequent_volume_quota_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SUBSEQUENT_VOLUME_QUOTA, __FILE__, __LINE__);
  }
  //  PFCP_IE_SUBSEQUENT_TIME_QUOTA
  virtual bool get(pfcp::subsequent_time_quota_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SUBSEQUENT_TIME_QUOTA, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::subsequent_time_quota_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SUBSEQUENT_TIME_QUOTA, __FILE__, __LINE__);
  }
  //  PFCP_IE_RQI
  virtual bool get(pfcp::rqi_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_RQI, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::rqi_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_RQI, __FILE__, __LINE__);
  }
  //  PFCP_IE_QFI
  virtual bool get(pfcp::qfi_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_QFI, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::qfi_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_QFI, __FILE__, __LINE__);
  }
  //  PFCP_IE_QUERY_URR_REFERENCE
  virtual bool get(pfcp::query_urr_reference_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_QUERY_URR_REFERENCE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::query_urr_reference_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_QUERY_URR_REFERENCE, __FILE__, __LINE__);
  }
  //  PFCP_IE_ADDITIONAL_USAGE_REPORTS_INFORMATION
  virtual bool get(pfcp::additional_usage_reports_information_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ADDITIONAL_USAGE_REPORTS_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::additional_usage_reports_information_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ADDITIONAL_USAGE_REPORTS_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_CREATE_TRAFFIC_ENDPOINT
  virtual bool get(pfcp::create_traffic_endpoint& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_TRAFFIC_ENDPOINT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::create_traffic_endpoint& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATE_TRAFFIC_ENDPOINT, __FILE__, __LINE__);
  }
  //  PFCP_IE_CREATED_TRAFFIC_ENDPOINT
  virtual bool get(pfcp::created_traffic_endpoint& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATED_TRAFFIC_ENDPOINT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::created_traffic_endpoint& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_CREATED_TRAFFIC_ENDPOINT, __FILE__, __LINE__);
  }
  //  PFCP_IE_UPDATE_TRAFFIC_ENDPOINT
  virtual bool get(pfcp::update_traffic_endpoint& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_TRAFFIC_ENDPOINT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::update_traffic_endpoint& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_UPDATE_TRAFFIC_ENDPOINT, __FILE__, __LINE__);
  }
  //  PFCP_IE_REMOVE_TRAFFIC_ENDPOINT
  virtual bool get(pfcp::remove_traffic_endpoint& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_TRAFFIC_ENDPOINT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::remove_traffic_endpoint& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_REMOVE_TRAFFIC_ENDPOINT, __FILE__, __LINE__);
  }
  //  PFCP_IE_TRAFFIC_ENDPOINT_ID
  virtual bool get(pfcp::traffic_endpoint_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TRAFFIC_ENDPOINT_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::traffic_endpoint_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TRAFFIC_ENDPOINT_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_ETHERNET_PACKET_FILTER
  virtual bool get(pfcp::ethernet_packet_filter& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_PACKET_FILTER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::ethernet_packet_filter& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_PACKET_FILTER, __FILE__, __LINE__);
  }
  //  PFCP_IE_MAC_ADDRESS
  virtual bool get(pfcp::mac_address_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MAC_ADDRESS, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::mac_address_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MAC_ADDRESS, __FILE__, __LINE__);
  }
  //  PFCP_IE_C_TAG
  virtual bool get(pfcp::c_tag_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_C_TAG, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::c_tag_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_C_TAG, __FILE__, __LINE__);
  }
  //  PFCP_IE_S_TAG
  virtual bool get(pfcp::s_tag_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_S_TAG, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::s_tag_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_S_TAG, __FILE__, __LINE__);
  }
  //  PFCP_IE_ETHERTYPE
  virtual bool get(pfcp::ethertype_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERTYPE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::ethertype_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERTYPE, __FILE__, __LINE__);
  }
  //  PFCP_IE_PROXYING
  virtual bool get(pfcp::proxying_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PROXYING, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::proxying_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_PROXYING, __FILE__, __LINE__);
  }
  //  PFCP_IE_ETHERNET_FILTER_ID
  virtual bool get(pfcp::ethernet_filter_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_FILTER_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::ethernet_filter_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_FILTER_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_ETHERNET_FILTER_PROPERTIES
  virtual bool get(pfcp::ethernet_filter_properties_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_FILTER_PROPERTIES, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::ethernet_filter_properties_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_FILTER_PROPERTIES, __FILE__, __LINE__);
  }
  //  PFCP_IE_SUGGESTED_BUFFERING_PACKETS_COUNT
  virtual bool get(pfcp::suggested_buffering_packets_count_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SUGGESTED_BUFFERING_PACKETS_COUNT, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::suggested_buffering_packets_count_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_SUGGESTED_BUFFERING_PACKETS_COUNT, __FILE__, __LINE__);
  }
  //  PFCP_IE_USER_ID
  virtual bool get(pfcp::user_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_USER_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::user_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(0, PFCP_IE_USER_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_ETHERNET_PDU_SESSION_INFORMATION
  virtual bool get(pfcp::ethernet_pdu_session_information_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_PDU_SESSION_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::ethernet_pdu_session_information_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_PDU_SESSION_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_ETHERNET_TRAFFIC_INFORMATION
  virtual bool get(pfcp::ethernet_traffic_information& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_TRAFFIC_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::ethernet_traffic_information& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_TRAFFIC_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_MAC_ADDRESSES_DETECTED
  virtual bool get(pfcp::mac_addresses_detected_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MAC_ADDRESSES_DETECTED, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::mac_addresses_detected_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MAC_ADDRESSES_DETECTED, __FILE__, __LINE__);
  }
  //  PFCP_IE_MAC_ADDRESSES_REMOVED
  virtual bool get(pfcp::mac_addresses_removed_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MAC_ADDRESSES_REMOVED, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::mac_addresses_removed_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_MAC_ADDRESSES_REMOVED, __FILE__, __LINE__);
  }
  //  PFCP_IE_ETHERNET_INACTIVITY_TIMER
  virtual bool get(pfcp::ethernet_inactivity_timer_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_INACTIVITY_TIMER, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::ethernet_inactivity_timer_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ETHERNET_INACTIVITY_TIMER, __FILE__, __LINE__);
  }
  //  PFCP_IE_ADDITIONAL_MONITORING_TIME
  virtual bool get(pfcp::additional_monitoring_time& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ADDITIONAL_MONITORING_TIME, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::additional_monitoring_time& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_ADDITIONAL_MONITORING_TIME, __FILE__, __LINE__);
  }
  //  PFCP_IE_EVENT_INFORMATION
  virtual bool get(pfcp::event_information& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_EVENT_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::event_information& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_EVENT_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_EVENT_REPORTING
  virtual bool get(pfcp::event_reporting& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_EVENT_REPORTING, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::event_reporting& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_EVENT_REPORTING, __FILE__, __LINE__);
  }
  //  PFCP_IE_EVENT_ID
  virtual bool get(pfcp::event_id_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_EVENT_ID, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::event_id_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_EVENT_ID, __FILE__, __LINE__);
  }
  //  PFCP_IE_EVENT_THRESHOLD
  virtual bool get(pfcp::event_threshold_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_EVENT_THRESHOLD, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::event_threshold_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_EVENT_THRESHOLD, __FILE__, __LINE__);
  }
  //  PFCP_IE_TRACE_INFORMATION
  virtual bool get(pfcp::trace_information_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TRACE_INFORMATION, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::trace_information_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_TRACE_INFORMATION, __FILE__, __LINE__);
  }
  //  PFCP_IE_FRAMED_ROUTE
  virtual bool get(pfcp::framed_route_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FRAMED_ROUTE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::framed_route_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FRAMED_ROUTE, __FILE__, __LINE__);
  }
  //  PFCP_IE_FRAMED_ROUTING
  virtual bool get(pfcp::framed_routing_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FRAMED_ROUTING, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::framed_routing_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FRAMED_ROUTING, __FILE__, __LINE__);
  }
  //  PFCP_IE_FRAMED_IPV6_ROUTE
  virtual bool get(pfcp::framed_ipv6_route_t& v) const {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FRAMED_IPV6_ROUTE, __FILE__, __LINE__);
  }
  virtual void set(const pfcp::framed_ipv6_route_t& v) {
    throw pfcp_msg_illegal_ie_exception(
        0, PFCP_IE_FRAMED_IPV6_ROUTE, __FILE__, __LINE__);
  }

  virtual ~pfcp_ies_container(){};
};
}  // namespace pfcp

namespace pfcp {

//------------------------------------------------------------------------------
// Table 7.4.3.1-3: PFD
class pfd : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::pfd_contents_t> pfd_contents;

  pfd() : pfd_contents() {}

  pfd(const pfd& p) : pfd_contents(p.pfd_contents) {}

  // virtual ~pfd() {};
  void set(const pfcp::pfd_contents_t& v) {
    pfd_contents.first  = true;
    pfd_contents.second = v;
  }

  bool get(pfcp::pfd_contents_t& v) const {
    if (pfd_contents.first) {
      v = pfd_contents.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.4.3.1-2: Application ID's PFDs
class application_ids_pfds : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::application_id_t> application_id;
  std::pair<bool, std::vector<pfcp::pfd>> pfds;

  application_ids_pfds() : application_id(), pfds() {}

  application_ids_pfds(const application_ids_pfds& a)
      : application_id(a.application_id), pfds(a.pfds) {}

  // virtual ~application_ids_pfds() {};
  void set(const pfcp::application_id_t& v) {
    application_id.first  = true;
    application_id.second = v;
  }
  void set(const std::vector<pfcp::pfd>& v) {
    pfds.first  = true;
    pfds.second = v;
  }

  bool get(pfcp::application_id_t& v) const {
    if (application_id.first) {
      v = application_id.second;
      return true;
    }
    return false;
  }
  bool get(std::vector<pfcp::pfd>& v) const {
    if (pfds.first) {
      v = pfds.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.4.5.1.2-1: User Plane Path Failure IE within PFCP Node Report Request
class user_plane_path_failure_report : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::remote_gtp_u_peer_t> remote_gtp_u_peer;

  user_plane_path_failure_report() : remote_gtp_u_peer() {}

  user_plane_path_failure_report(const user_plane_path_failure_report& u)
      : remote_gtp_u_peer(u.remote_gtp_u_peer) {}

  // virtual ~user_plane_path_failure_report() {};
  void set(const pfcp::remote_gtp_u_peer_t& v) {
    remote_gtp_u_peer.first  = true;
    remote_gtp_u_peer.second = v;
  }

  bool get(pfcp::remote_gtp_u_peer_t& v) const {
    if (remote_gtp_u_peer.first) {
      v = remote_gtp_u_peer.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.2-3: Ethernet Packet Filter IE within PFCP Session Establishment
// Request
class ethernet_packet_filter : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::ethernet_filter_id_t> ethernet_filter_id;
  std::pair<bool, pfcp::ethernet_filter_properties_t>
      ethernet_filter_properties;
  std::pair<bool, pfcp::mac_address_t> mac_address;
  std::pair<bool, pfcp::ethertype_t> ethertype;
  std::pair<bool, pfcp::c_tag_t> c_tag;
  std::pair<bool, pfcp::s_tag_t> s_tag;
  std::pair<bool, pfcp::sdf_filter_t> sdf_filter;

  ethernet_packet_filter()
      : ethernet_filter_id(),
        ethernet_filter_properties(),
        mac_address(),
        ethertype(),
        c_tag(),
        s_tag(),
        sdf_filter() {}

  ethernet_packet_filter(const ethernet_packet_filter& e)
      : ethernet_filter_id(e.ethernet_filter_id),
        ethernet_filter_properties(e.ethernet_filter_properties),
        mac_address(e.mac_address),
        ethertype(e.ethertype),
        c_tag(e.c_tag),
        s_tag(e.s_tag),
        sdf_filter(e.sdf_filter) {}

  // virtual ~ethernet_packet_filter() {};
  void set(const pfcp::ethernet_filter_id_t& v) {
    ethernet_filter_id.first  = true;
    ethernet_filter_id.second = v;
  }
  void set(const pfcp::ethernet_filter_properties_t& v) {
    ethernet_filter_properties.first  = true;
    ethernet_filter_properties.second = v;
  }
  void set(const pfcp::mac_address_t& v) {
    mac_address.first  = true;
    mac_address.second = v;
  }
  void set(const pfcp::ethertype_t& v) {
    ethertype.first  = true;
    ethertype.second = v;
  }
  void set(const pfcp::c_tag_t& v) {
    c_tag.first  = true;
    c_tag.second = v;
  }
  void set(const pfcp::s_tag_t& v) {
    s_tag.first  = true;
    s_tag.second = v;
  }
  void set(const pfcp::sdf_filter_t& v) {
    sdf_filter.first  = true;
    sdf_filter.second = v;
  }

  bool get(pfcp::ethernet_filter_id_t& v) const {
    if (ethernet_filter_id.first) {
      v = ethernet_filter_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ethernet_filter_properties_t& v) const {
    if (ethernet_filter_properties.first) {
      v = ethernet_filter_properties.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::mac_address_t& v) const {
    if (mac_address.first) {
      v = mac_address.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ethertype_t& v) const {
    if (ethertype.first) {
      v = ethertype.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::c_tag_t& v) const {
    if (c_tag.first) {
      v = c_tag.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::s_tag_t& v) const {
    if (s_tag.first) {
      v = s_tag.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::sdf_filter_t& v) const {
    if (sdf_filter.first) {
      v = sdf_filter.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.2-2: PDI IE within PFCP Session Establishment Request
class pdi : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::source_interface_t> source_interface;
  std::pair<bool, pfcp::fteid_t> local_fteid;
  std::pair<bool, pfcp::network_instance_t> network_instance;
  std::pair<bool, pfcp::ue_ip_address_t> ue_ip_address;
  std::pair<bool, pfcp::traffic_endpoint_id_t> traffic_endpoint_id;
  std::pair<bool, pfcp::sdf_filter_t> sdf_filter;
  std::pair<bool, pfcp::application_id_t> application_id;
  std::pair<bool, pfcp::ethernet_packet_filter> ethernet_packet_filter;
  std::pair<bool, pfcp::qfi_t> qfi;
  std::pair<bool, pfcp::framed_route_t> framed_route;
  std::pair<bool, pfcp::framed_routing_t> framed_routing;
  std::pair<bool, pfcp::framed_ipv6_route_t> framed_ipv6_route;

  pdi()
      : source_interface(),
        local_fteid(),
        network_instance(),
        ue_ip_address(),
        traffic_endpoint_id(),
        sdf_filter(),
        application_id(),
        ethernet_packet_filter(),
        qfi(),
        framed_route(),
        framed_routing(),
        framed_ipv6_route() {}

  pdi(const pdi& p)
      : source_interface(p.source_interface),
        local_fteid(p.local_fteid),
        network_instance(p.network_instance),
        ue_ip_address(p.ue_ip_address),
        traffic_endpoint_id(p.traffic_endpoint_id),
        sdf_filter(p.sdf_filter),
        application_id(p.application_id),
        ethernet_packet_filter(p.ethernet_packet_filter),
        qfi(p.qfi),
        framed_route(p.framed_route),
        framed_routing(p.framed_routing),
        framed_ipv6_route(p.framed_ipv6_route) {}

  // virtual ~pdi() {};
  void set(const pfcp::source_interface_t& v) {
    source_interface.first  = true;
    source_interface.second = v;
  }
  void set(const pfcp::fteid_t& v) {
    local_fteid.first  = true;
    local_fteid.second = v;
  }
  void set(const pfcp::network_instance_t& v) {
    network_instance.first  = true;
    network_instance.second = v;
  }
  void set(const pfcp::ue_ip_address_t& v) {
    ue_ip_address.first  = true;
    ue_ip_address.second = v;
  }
  void set(const pfcp::traffic_endpoint_id_t& v) {
    traffic_endpoint_id.first  = true;
    traffic_endpoint_id.second = v;
  }
  void set(const pfcp::sdf_filter_t& v) {
    sdf_filter.first  = true;
    sdf_filter.second = v;
  }
  void set(const pfcp::application_id_t& v) {
    application_id.first  = true;
    application_id.second = v;
  }
  void set(const pfcp::ethernet_packet_filter& v) {
    ethernet_packet_filter.first  = true;
    ethernet_packet_filter.second = v;
  }
  void set(const pfcp::qfi_t& v) {
    qfi.first  = true;
    qfi.second = v;
  }
  void set(const pfcp::framed_route_t& v) {
    framed_route.first  = true;
    framed_route.second = v;
  }
  void set(const pfcp::framed_routing_t& v) {
    framed_routing.first  = true;
    framed_routing.second = v;
  }
  void set(const pfcp::framed_ipv6_route_t& v) {
    framed_ipv6_route.first  = true;
    framed_ipv6_route.second = v;
  }

  bool get(pfcp::source_interface_t& v) const {
    if (source_interface.first) {
      v = source_interface.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::fteid_t& v) const {
    if (local_fteid.first) {
      v = local_fteid.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::network_instance_t& v) const {
    if (network_instance.first) {
      v = network_instance.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ue_ip_address_t& v) const {
    if (ue_ip_address.first) {
      v = ue_ip_address.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::traffic_endpoint_id_t& v) const {
    if (traffic_endpoint_id.first) {
      v = traffic_endpoint_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::sdf_filter_t& v) const {
    if (sdf_filter.first) {
      v = sdf_filter.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::application_id_t& v) const {
    if (application_id.first) {
      v = application_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ethernet_packet_filter& v) const {
    if (ethernet_packet_filter.first) {
      v = ethernet_packet_filter.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::qfi_t& v) const {
    if (qfi.first) {
      v = qfi.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::framed_route_t& v) const {
    if (framed_route.first) {
      v = framed_route.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::framed_routing_t& v) const {
    if (framed_routing.first) {
      v = framed_routing.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::framed_ipv6_route_t& v) const {
    if (framed_ipv6_route.first) {
      v = framed_ipv6_route.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.2-1: Create PDR IE within PFCP Session Establishment Request
class create_pdr : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::pdr_id_t> pdr_id;
  std::pair<bool, pfcp::precedence_t> precedence;
  std::pair<bool, pfcp::pdi> pdi;
  std::pair<bool, pfcp::outer_header_removal_t> outer_header_removal;
  std::pair<bool, pfcp::far_id_t> far_id;
  std::pair<bool, pfcp::urr_id_t> urr_id;
  std::pair<bool, pfcp::qer_id_t> qer_id;
  std::pair<bool, pfcp::activate_predefined_rules_t> activate_predefined_rules;

  create_pdr()
      : pdr_id(),
        precedence(),
        pdi(),
        outer_header_removal(),
        far_id(),
        urr_id(),
        qer_id(),
        activate_predefined_rules() {}

  create_pdr(const create_pdr& c)
      : pdr_id(c.pdr_id),
        precedence(c.precedence),
        pdi(c.pdi),
        outer_header_removal(c.outer_header_removal),
        far_id(c.far_id),
        urr_id(c.urr_id),
        qer_id(c.qer_id),
        activate_predefined_rules(c.activate_predefined_rules) {}

  // virtual ~create_pdr() {};
  void set(const pfcp::pdr_id_t& v) {
    pdr_id.first  = true;
    pdr_id.second = v;
  }
  void set(const pfcp::precedence_t& v) {
    precedence.first  = true;
    precedence.second = v;
  }
  void set(const pfcp::pdi& v) {
    pdi.first  = true;
    pdi.second = v;
  }
  void set(const pfcp::outer_header_removal_t& v) {
    outer_header_removal.first  = true;
    outer_header_removal.second = v;
  }
  void set(const pfcp::far_id_t& v) {
    far_id.first  = true;
    far_id.second = v;
  }
  void set(const pfcp::urr_id_t& v) {
    urr_id.first  = true;
    urr_id.second = v;
  }
  void set(const pfcp::qer_id_t& v) {
    qer_id.first  = true;
    qer_id.second = v;
  }
  void set(const pfcp::activate_predefined_rules_t& v) {
    activate_predefined_rules.first  = true;
    activate_predefined_rules.second = v;
  }

  bool get(pfcp::pdr_id_t& v) const {
    if (pdr_id.first) {
      v = pdr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::precedence_t& v) const {
    if (precedence.first) {
      v = precedence.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::pdi& v) const {
    if (pdi.first) {
      v = pdi.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::outer_header_removal_t& v) const {
    if (outer_header_removal.first) {
      v = outer_header_removal.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::far_id_t& v) const {
    if (far_id.first) {
      v = far_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::urr_id_t& v) const {
    if (urr_id.first) {
      v = urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::qer_id_t& v) const {
    if (qer_id.first) {
      v = qer_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::activate_predefined_rules_t& v) const {
    if (activate_predefined_rules.first) {
      v = activate_predefined_rules.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.3-2: Update Forwarding Parameters IE in FAR
class update_forwarding_parameters : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::destination_interface_t> destination_interface;
  std::pair<bool, pfcp::network_instance_t> network_instance;
  std::pair<bool, pfcp::outer_header_creation_t> outer_header_creation;
  std::pair<bool, pfcp::transport_level_marking_t> transport_level_marking;
  std::pair<bool, pfcp::forwarding_policy_t> forwarding_policy;
  std::pair<bool, pfcp::header_enrichment_t> header_enrichment;
  std::pair<bool, pfcp::pfcpsmreq_flags_t> pfcpsmreq_flags;
  std::pair<bool, pfcp::traffic_endpoint_id_t> linked_traffic_endpoint_id;

  update_forwarding_parameters()
      : destination_interface(),
        network_instance(),
        outer_header_creation(),
        transport_level_marking(),
        forwarding_policy(),
        header_enrichment(),
        pfcpsmreq_flags(),
        linked_traffic_endpoint_id() {}

  update_forwarding_parameters(const update_forwarding_parameters& u)
      : destination_interface(u.destination_interface),
        network_instance(u.network_instance),
        outer_header_creation(u.outer_header_creation),
        transport_level_marking(u.transport_level_marking),
        forwarding_policy(u.forwarding_policy),
        header_enrichment(u.header_enrichment),
        pfcpsmreq_flags(u.pfcpsmreq_flags),
        linked_traffic_endpoint_id(u.linked_traffic_endpoint_id) {}

  // virtual ~update_forwarding_parameters() {};
  void set(const pfcp::destination_interface_t& v) {
    destination_interface.first  = true;
    destination_interface.second = v;
  }
  void set(const pfcp::network_instance_t& v) {
    network_instance.first  = true;
    network_instance.second = v;
  }
  void set(const pfcp::outer_header_creation_t& v) {
    outer_header_creation.first  = true;
    outer_header_creation.second = v;
  }
  void set(const pfcp::transport_level_marking_t& v) {
    transport_level_marking.first  = true;
    transport_level_marking.second = v;
  }
  void set(const pfcp::forwarding_policy_t& v) {
    forwarding_policy.first  = true;
    forwarding_policy.second = v;
  }
  void set(const pfcp::header_enrichment_t& v) {
    header_enrichment.first  = true;
    header_enrichment.second = v;
  }
  void set(const pfcp::pfcpsmreq_flags_t& v) {
    pfcpsmreq_flags.first  = true;
    pfcpsmreq_flags.second = v;
  }
  void set(const pfcp::traffic_endpoint_id_t& v) {
    linked_traffic_endpoint_id.first  = true;
    linked_traffic_endpoint_id.second = v;
  }

  bool get(pfcp::destination_interface_t& v) const {
    if (destination_interface.first) {
      v = destination_interface.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::network_instance_t& v) const {
    if (network_instance.first) {
      v = network_instance.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::outer_header_creation_t& v) const {
    if (outer_header_creation.first) {
      v = outer_header_creation.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::transport_level_marking_t& v) const {
    if (transport_level_marking.first) {
      v = transport_level_marking.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::forwarding_policy_t& v) const {
    if (forwarding_policy.first) {
      v = forwarding_policy.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::header_enrichment_t& v) const {
    if (header_enrichment.first) {
      v = header_enrichment.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::pfcpsmreq_flags_t& v) const {
    if (pfcpsmreq_flags.first) {
      v = pfcpsmreq_flags.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::traffic_endpoint_id_t& v) const {
    if (linked_traffic_endpoint_id.first) {
      v = linked_traffic_endpoint_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.3-2: Forwarding Parameters IE in FAR
class forwarding_parameters : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::destination_interface_t> destination_interface;
  std::pair<bool, pfcp::network_instance_t> network_instance;
  std::pair<bool, pfcp::redirect_information_t> redirect_information;
  std::pair<bool, pfcp::outer_header_creation_t> outer_header_creation;
  std::pair<bool, pfcp::transport_level_marking_t> transport_level_marking;
  std::pair<bool, pfcp::forwarding_policy_t> forwarding_policy;
  std::pair<bool, pfcp::header_enrichment_t> header_enrichment;
  std::pair<bool, pfcp::traffic_endpoint_id_t> linked_traffic_endpoint_id;
  std::pair<bool, pfcp::proxying_t> proxying;

  forwarding_parameters()
      : destination_interface(),
        network_instance(),
        redirect_information(),
        outer_header_creation(),
        transport_level_marking(),
        forwarding_policy(),
        header_enrichment(),
        linked_traffic_endpoint_id(),
        proxying() {}

  forwarding_parameters(const forwarding_parameters& f)
      : destination_interface(f.destination_interface),
        network_instance(f.network_instance),
        redirect_information(f.redirect_information),
        outer_header_creation(f.outer_header_creation),
        transport_level_marking(f.transport_level_marking),
        forwarding_policy(f.forwarding_policy),
        header_enrichment(f.header_enrichment),
        linked_traffic_endpoint_id(f.linked_traffic_endpoint_id),
        proxying(f.proxying) {}

  bool update(const update_forwarding_parameters& u) {
    if (u.destination_interface.first) set(u.destination_interface.second);
    if (u.network_instance.first) set(u.network_instance.second);
    if (u.outer_header_creation.first) set(u.outer_header_creation.second);
    if (u.transport_level_marking.first) set(u.transport_level_marking.second);
    if (u.forwarding_policy.first) set(u.forwarding_policy.second);
    if (u.header_enrichment.first) set(u.header_enrichment.second);
    if (u.linked_traffic_endpoint_id.first)
      set(u.linked_traffic_endpoint_id.second);
    return true;
  }

  // virtual ~forwarding_parameters() {};
  void set(const pfcp::destination_interface_t& v) {
    destination_interface.first  = true;
    destination_interface.second = v;
  }
  void set(const pfcp::network_instance_t& v) {
    network_instance.first  = true;
    network_instance.second = v;
  }
  void set(const pfcp::redirect_information_t& v) {
    redirect_information.first  = true;
    redirect_information.second = v;
  }
  void set(const pfcp::outer_header_creation_t& v) {
    outer_header_creation.first  = true;
    outer_header_creation.second = v;
  }
  void set(const pfcp::transport_level_marking_t& v) {
    transport_level_marking.first  = true;
    transport_level_marking.second = v;
  }
  void set(const pfcp::forwarding_policy_t& v) {
    forwarding_policy.first  = true;
    forwarding_policy.second = v;
  }
  void set(const pfcp::header_enrichment_t& v) {
    header_enrichment.first  = true;
    header_enrichment.second = v;
  }
  void set(const pfcp::traffic_endpoint_id_t& v) {
    linked_traffic_endpoint_id.first  = true;
    linked_traffic_endpoint_id.second = v;
  }
  void set(const pfcp::proxying_t& v) {
    proxying.first  = true;
    proxying.second = v;
  }

  bool get(pfcp::destination_interface_t& v) const {
    if (destination_interface.first) {
      v = destination_interface.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::network_instance_t& v) const {
    if (network_instance.first) {
      v = network_instance.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::redirect_information_t& v) const {
    if (redirect_information.first) {
      v = redirect_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::outer_header_creation_t& v) const {
    if (outer_header_creation.first) {
      v = outer_header_creation.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::transport_level_marking_t& v) const {
    if (transport_level_marking.first) {
      v = transport_level_marking.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::forwarding_policy_t& v) const {
    if (forwarding_policy.first) {
      v = forwarding_policy.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::header_enrichment_t& v) const {
    if (header_enrichment.first) {
      v = header_enrichment.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::traffic_endpoint_id_t& v) const {
    if (linked_traffic_endpoint_id.first) {
      v = linked_traffic_endpoint_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::proxying_t& v) const {
    if (proxying.first) {
      v = proxying.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.3-3: Update Duplicating Parameters IE in FAR
class update_duplicating_parameters : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::destination_interface_t> destination_interface;
  std::pair<bool, pfcp::outer_header_creation_t> outer_header_creation;
  std::pair<bool, pfcp::transport_level_marking_t> transport_level_marking;
  std::pair<bool, pfcp::forwarding_policy_t> forwarding_policy;

  update_duplicating_parameters()
      : destination_interface(),
        outer_header_creation(),
        transport_level_marking(),
        forwarding_policy() {}

  update_duplicating_parameters(const update_duplicating_parameters& u)
      : destination_interface(u.destination_interface),
        outer_header_creation(u.outer_header_creation),
        transport_level_marking(u.transport_level_marking),
        forwarding_policy(u.forwarding_policy) {}

  // virtual ~update_duplicating_parameters() {};
  void set(const pfcp::destination_interface_t& v) {
    destination_interface.first  = true;
    destination_interface.second = v;
  }
  void set(const pfcp::outer_header_creation_t& v) {
    outer_header_creation.first  = true;
    outer_header_creation.second = v;
  }
  void set(const pfcp::transport_level_marking_t& v) {
    transport_level_marking.first  = true;
    transport_level_marking.second = v;
  }
  void set(const pfcp::forwarding_policy_t& v) {
    forwarding_policy.first  = true;
    forwarding_policy.second = v;
  }

  bool get(pfcp::destination_interface_t& v) const {
    if (destination_interface.first) {
      v = destination_interface.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::outer_header_creation_t& v) const {
    if (outer_header_creation.first) {
      v = outer_header_creation.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::transport_level_marking_t& v) const {
    if (transport_level_marking.first) {
      v = transport_level_marking.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::forwarding_policy_t& v) const {
    if (forwarding_policy.first) {
      v = forwarding_policy.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.3-3: Duplicating Parameters IE in FAR
class duplicating_parameters : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::destination_interface_t> destination_interface;
  std::pair<bool, pfcp::outer_header_creation_t> outer_header_creation;
  std::pair<bool, pfcp::transport_level_marking_t> transport_level_marking;
  std::pair<bool, pfcp::forwarding_policy_t> forwarding_policy;

  duplicating_parameters()
      : destination_interface(),
        outer_header_creation(),
        transport_level_marking(),
        forwarding_policy() {}

  duplicating_parameters(const duplicating_parameters& d)
      : destination_interface(d.destination_interface),
        outer_header_creation(d.outer_header_creation),
        transport_level_marking(d.transport_level_marking),
        forwarding_policy(d.forwarding_policy) {}

  bool update(const update_duplicating_parameters& u) {
    if (u.destination_interface.first) set(u.destination_interface.second);
    if (u.outer_header_creation.first) set(u.outer_header_creation.second);
    if (u.transport_level_marking.first) set(u.transport_level_marking.second);
    if (u.forwarding_policy.first) set(u.forwarding_policy.second);
    return true;
  }
  // virtual ~duplicating_parameters() {};
  void set(const pfcp::destination_interface_t& v) {
    destination_interface.first  = true;
    destination_interface.second = v;
  }
  void set(const pfcp::outer_header_creation_t& v) {
    outer_header_creation.first  = true;
    outer_header_creation.second = v;
  }
  void set(const pfcp::transport_level_marking_t& v) {
    transport_level_marking.first  = true;
    transport_level_marking.second = v;
  }
  void set(const pfcp::forwarding_policy_t& v) {
    forwarding_policy.first  = true;
    forwarding_policy.second = v;
  }

  bool get(pfcp::destination_interface_t& v) const {
    if (destination_interface.first) {
      v = destination_interface.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::outer_header_creation_t& v) const {
    if (outer_header_creation.first) {
      v = outer_header_creation.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::transport_level_marking_t& v) const {
    if (transport_level_marking.first) {
      v = transport_level_marking.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::forwarding_policy_t& v) const {
    if (forwarding_policy.first) {
      v = forwarding_policy.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.3-1: Create FAR IE within PFCP Session Establishment Request
class create_far : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::far_id_t> far_id;
  std::pair<bool, pfcp::apply_action_t> apply_action;
  std::pair<bool, pfcp::forwarding_parameters> forwarding_parameters;
  std::pair<bool, pfcp::duplicating_parameters> duplicating_parameters;
  std::pair<bool, pfcp::bar_id_t> bar_id;

  create_far()
      : far_id(),
        apply_action(),
        forwarding_parameters(),
        duplicating_parameters(),
        bar_id() {}

  create_far(const create_far& c)
      : far_id(c.far_id),
        apply_action(c.apply_action),
        forwarding_parameters(c.forwarding_parameters),
        duplicating_parameters(c.duplicating_parameters),
        bar_id(c.bar_id) {}

  // virtual ~create_far() {};
  void set(const pfcp::far_id_t& v) {
    far_id.first  = true;
    far_id.second = v;
  }
  void set(const pfcp::apply_action_t& v) {
    apply_action.first  = true;
    apply_action.second = v;
  }
  void set(const pfcp::forwarding_parameters& v) {
    forwarding_parameters.first  = true;
    forwarding_parameters.second = v;
  }
  void set(const pfcp::duplicating_parameters& v) {
    duplicating_parameters.first  = true;
    duplicating_parameters.second = v;
  }
  void set(const pfcp::bar_id_t& v) {
    bar_id.first  = true;
    bar_id.second = v;
  }

  bool get(pfcp::far_id_t& v) const {
    if (far_id.first) {
      v = far_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::apply_action_t& v) const {
    if (apply_action.first) {
      v = apply_action.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::forwarding_parameters& v) const {
    if (forwarding_parameters.first) {
      v = forwarding_parameters.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::duplicating_parameters& v) const {
    if (duplicating_parameters.first) {
      v = duplicating_parameters.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::bar_id_t& v) const {
    if (bar_id.first) {
      v = bar_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.4-2: Aggregated URRs
class aggregated_urrs : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::aggregated_urr_id_t> aggregated_urr_id;
  std::pair<bool, pfcp::multiplier_t> multiplier;

  aggregated_urrs() : aggregated_urr_id(), multiplier() {}

  aggregated_urrs(const aggregated_urrs& a)
      : aggregated_urr_id(a.aggregated_urr_id), multiplier(a.multiplier) {}

  // virtual ~aggregated_urrs() {};
  void set(const pfcp::aggregated_urr_id_t& v) {
    aggregated_urr_id.first  = true;
    aggregated_urr_id.second = v;
  }
  void set(const pfcp::multiplier_t& v) {
    multiplier.first  = true;
    multiplier.second = v;
  }

  bool get(pfcp::aggregated_urr_id_t& v) const {
    if (aggregated_urr_id.first) {
      v = aggregated_urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::multiplier_t& v) const {
    if (multiplier.first) {
      v = multiplier.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.4-3: Additional Monitoring Time
class additional_monitoring_time : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::monitoring_time_t> monitoring_time;
  std::pair<bool, pfcp::subsequent_volume_threshold_t>
      subsequent_volume_threshold;
  std::pair<bool, pfcp::subsequent_time_threshold_t> subsequent_time_threshold;
  std::pair<bool, pfcp::subsequent_volume_quota_t> subsequent_volume_quota;
  std::pair<bool, pfcp::subsequent_time_quota_t> subsequent_time_quota;

  additional_monitoring_time()
      : monitoring_time(),
        subsequent_volume_threshold(),
        subsequent_time_threshold(),
        subsequent_volume_quota(),
        subsequent_time_quota() {}

  additional_monitoring_time(const additional_monitoring_time& a)
      : monitoring_time(a.monitoring_time),
        subsequent_volume_threshold(a.subsequent_volume_threshold),
        subsequent_time_threshold(a.subsequent_time_threshold),
        subsequent_volume_quota(a.subsequent_volume_quota),
        subsequent_time_quota(a.subsequent_time_quota) {}

  // virtual ~additional_monitoring_time() {};
  void set(const pfcp::monitoring_time_t& v) {
    monitoring_time.first  = true;
    monitoring_time.second = v;
  }
  void set(const pfcp::subsequent_volume_threshold_t& v) {
    subsequent_volume_threshold.first  = true;
    subsequent_volume_threshold.second = v;
  }
  void set(const pfcp::subsequent_time_threshold_t& v) {
    subsequent_time_threshold.first  = true;
    subsequent_time_threshold.second = v;
  }
  void set(const pfcp::subsequent_volume_quota_t& v) {
    subsequent_volume_quota.first  = true;
    subsequent_volume_quota.second = v;
  }
  void set(const pfcp::subsequent_time_quota_t& v) {
    subsequent_time_quota.first  = true;
    subsequent_time_quota.second = v;
  }

  bool get(pfcp::monitoring_time_t& v) const {
    if (monitoring_time.first) {
      v = monitoring_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_volume_threshold_t& v) const {
    if (subsequent_volume_threshold.first) {
      v = subsequent_volume_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_time_threshold_t& v) const {
    if (subsequent_time_threshold.first) {
      v = subsequent_time_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_volume_quota_t& v) const {
    if (subsequent_volume_quota.first) {
      v = subsequent_volume_quota.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_time_quota_t& v) const {
    if (subsequent_time_quota.first) {
      v = subsequent_time_quota.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.4-4: Event Information
class event_information : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::event_id_t> event_id;
  std::pair<bool, pfcp::event_threshold_t> event_threshold;

  event_information() : event_id(), event_threshold() {}

  event_information(const event_information& e)
      : event_id(e.event_id), event_threshold(e.event_threshold) {}

  // virtual ~event_information() {};
  void set(const pfcp::event_id_t& v) {
    event_id.first  = true;
    event_id.second = v;
  }
  void set(const pfcp::event_threshold_t& v) {
    event_threshold.first  = true;
    event_threshold.second = v;
  }

  bool get(pfcp::event_id_t& v) const {
    if (event_id.first) {
      v = event_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::event_threshold_t& v) const {
    if (event_threshold.first) {
      v = event_threshold.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.4-1: Create URR IE within PFCP Session Establishment Request
class create_urr : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::urr_id_t> urr_id;
  std::pair<bool, pfcp::measurement_method_t> measurement_method;
  std::pair<bool, pfcp::reporting_triggers_t> reporting_triggers;
  std::pair<bool, pfcp::measurement_period_t> measurement_period;
  std::pair<bool, pfcp::volume_threshold_t> volume_threshold;
  std::pair<bool, pfcp::volume_quota_t> volume_quota;
  std::pair<bool, pfcp::time_threshold_t> time_threshold;
  std::pair<bool, pfcp::time_quota_t> time_quota;
  std::pair<bool, pfcp::quota_holding_time_t> quota_holding_time;
  std::pair<bool, pfcp::dropped_dl_traffic_threshold_t>
      dropped_dl_traffic_threshold;
  std::pair<bool, pfcp::monitoring_time_t> monitoring_time;
  std::pair<bool, pfcp::event_information> event_information;
  std::pair<bool, pfcp::subsequent_volume_threshold_t>
      subsequent_volume_threshold;
  std::pair<bool, pfcp::subsequent_time_threshold_t> subsequent_time_threshold;
  std::pair<bool, pfcp::subsequent_volume_quota_t> subsequent_volume_quota;
  std::pair<bool, pfcp::subsequent_time_quota_t> subsequent_time_quota;
  std::pair<bool, pfcp::inactivity_detection_time_t> inactivity_detection_time;
  std::pair<bool, pfcp::linked_urr_id_t> linked_urr_id;
  std::pair<bool, pfcp::measurement_information_t> measurement_information;
  std::pair<bool, pfcp::time_quota_mechanism_t> time_quota_mechanism;
  std::pair<bool, pfcp::aggregated_urrs> aggregated_urrs;
  std::pair<bool, pfcp::far_id_t> far_id_for_quota_action;
  std::pair<bool, pfcp::ethernet_inactivity_timer_t> ethernet_inactivity_timer;
  std::pair<bool, pfcp::additional_monitoring_time> additional_monitoring_time;

  create_urr()
      : urr_id(),
        measurement_method(),
        reporting_triggers(),
        measurement_period(),
        volume_threshold(),
        volume_quota(),
        time_threshold(),
        time_quota(),
        quota_holding_time(),
        dropped_dl_traffic_threshold(),
        monitoring_time(),
        event_information(),
        subsequent_volume_threshold(),
        subsequent_time_threshold(),
        subsequent_volume_quota(),
        subsequent_time_quota(),
        inactivity_detection_time(),
        linked_urr_id(),
        measurement_information(),
        time_quota_mechanism(),
        aggregated_urrs(),
        far_id_for_quota_action(),
        ethernet_inactivity_timer(),
        additional_monitoring_time() {}

  create_urr(const create_urr& c)
      : urr_id(c.urr_id),
        measurement_method(c.measurement_method),
        reporting_triggers(c.reporting_triggers),
        measurement_period(c.measurement_period),
        volume_threshold(c.volume_threshold),
        volume_quota(c.volume_quota),
        time_threshold(c.time_threshold),
        time_quota(c.time_quota),
        quota_holding_time(c.quota_holding_time),
        dropped_dl_traffic_threshold(c.dropped_dl_traffic_threshold),
        monitoring_time(c.monitoring_time),
        event_information(c.event_information),
        subsequent_volume_threshold(c.subsequent_volume_threshold),
        subsequent_time_threshold(c.subsequent_time_threshold),
        subsequent_volume_quota(c.subsequent_volume_quota),
        subsequent_time_quota(c.subsequent_time_quota),
        inactivity_detection_time(c.inactivity_detection_time),
        linked_urr_id(c.linked_urr_id),
        measurement_information(c.measurement_information),
        time_quota_mechanism(c.time_quota_mechanism),
        aggregated_urrs(c.aggregated_urrs),
        far_id_for_quota_action(c.far_id_for_quota_action),
        ethernet_inactivity_timer(c.ethernet_inactivity_timer),
        additional_monitoring_time(c.additional_monitoring_time) {}

  // virtual ~create_urr() {};
  void set(const pfcp::urr_id_t& v) {
    urr_id.first  = true;
    urr_id.second = v;
  }
  void set(const pfcp::measurement_method_t& v) {
    measurement_method.first  = true;
    measurement_method.second = v;
  }
  void set(const pfcp::reporting_triggers_t& v) {
    reporting_triggers.first  = true;
    reporting_triggers.second = v;
  }
  void set(const pfcp::measurement_period_t& v) {
    measurement_period.first  = true;
    measurement_period.second = v;
  }
  void set(const pfcp::volume_threshold_t& v) {
    volume_threshold.first  = true;
    volume_threshold.second = v;
  }
  void set(const pfcp::volume_quota_t& v) {
    volume_quota.first  = true;
    volume_quota.second = v;
  }
  void set(const pfcp::time_threshold_t& v) {
    time_threshold.first  = true;
    time_threshold.second = v;
  }
  void set(const pfcp::time_quota_t& v) {
    time_quota.first  = true;
    time_quota.second = v;
  }
  void set(const pfcp::quota_holding_time_t& v) {
    quota_holding_time.first  = true;
    quota_holding_time.second = v;
  }
  void set(const pfcp::dropped_dl_traffic_threshold_t& v) {
    dropped_dl_traffic_threshold.first  = true;
    dropped_dl_traffic_threshold.second = v;
  }
  void set(const pfcp::monitoring_time_t& v) {
    monitoring_time.first  = true;
    monitoring_time.second = v;
  }
  void set(const pfcp::event_information& v) {
    event_information.first  = true;
    event_information.second = v;
  }
  void set(const pfcp::subsequent_volume_threshold_t& v) {
    subsequent_volume_threshold.first  = true;
    subsequent_volume_threshold.second = v;
  }
  void set(const pfcp::subsequent_time_threshold_t& v) {
    subsequent_time_threshold.first  = true;
    subsequent_time_threshold.second = v;
  }
  void set(const pfcp::subsequent_volume_quota_t& v) {
    subsequent_volume_quota.first  = true;
    subsequent_volume_quota.second = v;
  }
  void set(const pfcp::subsequent_time_quota_t& v) {
    subsequent_time_quota.first  = true;
    subsequent_time_quota.second = v;
  }
  void set(const pfcp::inactivity_detection_time_t& v) {
    inactivity_detection_time.first  = true;
    inactivity_detection_time.second = v;
  }
  void set(const pfcp::linked_urr_id_t& v) {
    linked_urr_id.first  = true;
    linked_urr_id.second = v;
  }
  void set(const pfcp::measurement_information_t& v) {
    measurement_information.first  = true;
    measurement_information.second = v;
  }
  void set(const pfcp::time_quota_mechanism_t& v) {
    time_quota_mechanism.first  = true;
    time_quota_mechanism.second = v;
  }
  void set(const pfcp::aggregated_urrs& v) {
    aggregated_urrs.first  = true;
    aggregated_urrs.second = v;
  }
  void set(const pfcp::far_id_t& v) {
    far_id_for_quota_action.first  = true;
    far_id_for_quota_action.second = v;
  }
  void set(const pfcp::ethernet_inactivity_timer_t& v) {
    ethernet_inactivity_timer.first  = true;
    ethernet_inactivity_timer.second = v;
  }
  void set(const pfcp::additional_monitoring_time& v) {
    additional_monitoring_time.first  = true;
    additional_monitoring_time.second = v;
  }

  bool get(pfcp::urr_id_t& v) const {
    if (urr_id.first) {
      v = urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::measurement_method_t& v) const {
    if (measurement_method.first) {
      v = measurement_method.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::reporting_triggers_t& v) const {
    if (reporting_triggers.first) {
      v = reporting_triggers.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::measurement_period_t& v) const {
    if (measurement_period.first) {
      v = measurement_period.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::volume_threshold_t& v) const {
    if (volume_threshold.first) {
      v = volume_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::volume_quota_t& v) const {
    if (volume_quota.first) {
      v = volume_quota.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_threshold_t& v) const {
    if (time_threshold.first) {
      v = time_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_quota_t& v) const {
    if (time_quota.first) {
      v = time_quota.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::quota_holding_time_t& v) const {
    if (quota_holding_time.first) {
      v = quota_holding_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::dropped_dl_traffic_threshold_t& v) const {
    if (dropped_dl_traffic_threshold.first) {
      v = dropped_dl_traffic_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::monitoring_time_t& v) const {
    if (monitoring_time.first) {
      v = monitoring_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::event_information& v) const {
    if (event_information.first) {
      v = event_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_volume_threshold_t& v) const {
    if (subsequent_volume_threshold.first) {
      v = subsequent_volume_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_time_threshold_t& v) const {
    if (subsequent_time_threshold.first) {
      v = subsequent_time_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_volume_quota_t& v) const {
    if (subsequent_volume_quota.first) {
      v = subsequent_volume_quota.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_time_quota_t& v) const {
    if (subsequent_time_quota.first) {
      v = subsequent_time_quota.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::inactivity_detection_time_t& v) const {
    if (inactivity_detection_time.first) {
      v = inactivity_detection_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::linked_urr_id_t& v) const {
    if (linked_urr_id.first) {
      v = linked_urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::measurement_information_t& v) const {
    if (measurement_information.first) {
      v = measurement_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_quota_mechanism_t& v) const {
    if (time_quota_mechanism.first) {
      v = time_quota_mechanism.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::aggregated_urrs& v) const {
    if (aggregated_urrs.first) {
      v = aggregated_urrs.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::far_id_t& v) const {
    if (far_id_for_quota_action.first) {
      v = far_id_for_quota_action.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ethernet_inactivity_timer_t& v) const {
    if (ethernet_inactivity_timer.first) {
      v = ethernet_inactivity_timer.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::additional_monitoring_time& v) const {
    if (additional_monitoring_time.first) {
      v = additional_monitoring_time.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.5-1: Create QER IE within PFCP Session Establishment Request
class create_qer : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::qer_id_t> qer_id;
  std::pair<bool, pfcp::qer_correlation_id_t> qer_correlation_id;
  std::pair<bool, pfcp::gate_status_t> gate_status;
  std::pair<bool, pfcp::mbr_t> maximum_bitrate;
  std::pair<bool, pfcp::gbr_t> guaranteed_bitrate;
  std::pair<bool, pfcp::packet_rate_t> packet_rate;
  std::pair<bool, pfcp::dl_flow_level_marking_t> dl_flow_level_marking;
  std::pair<bool, pfcp::qfi_t> qos_flow_identifier;
  std::pair<bool, pfcp::rqi_t> reflective_qos;

  create_qer()
      : qer_id(),
        qer_correlation_id(),
        gate_status(),
        maximum_bitrate(),
        guaranteed_bitrate(),
        packet_rate(),
        dl_flow_level_marking(),
        qos_flow_identifier(),
        reflective_qos() {}

  create_qer(const create_qer& c)
      : qer_id(c.qer_id),
        qer_correlation_id(c.qer_correlation_id),
        gate_status(c.gate_status),
        maximum_bitrate(c.maximum_bitrate),
        guaranteed_bitrate(c.guaranteed_bitrate),
        packet_rate(c.packet_rate),
        dl_flow_level_marking(c.dl_flow_level_marking),
        qos_flow_identifier(c.qos_flow_identifier),
        reflective_qos(c.reflective_qos) {}

  // virtual ~create_qer() {};
  void set(const pfcp::qer_id_t& v) {
    qer_id.first  = true;
    qer_id.second = v;
  }
  void set(const pfcp::qer_correlation_id_t& v) {
    qer_correlation_id.first  = true;
    qer_correlation_id.second = v;
  }
  void set(const pfcp::gate_status_t& v) {
    gate_status.first  = true;
    gate_status.second = v;
  }
  void set(const pfcp::mbr_t& v) {
    maximum_bitrate.first  = true;
    maximum_bitrate.second = v;
  }
  void set(const pfcp::gbr_t& v) {
    guaranteed_bitrate.first  = true;
    guaranteed_bitrate.second = v;
  }
  void set(const pfcp::packet_rate_t& v) {
    packet_rate.first  = true;
    packet_rate.second = v;
  }
  void set(const pfcp::dl_flow_level_marking_t& v) {
    dl_flow_level_marking.first  = true;
    dl_flow_level_marking.second = v;
  }
  void set(const pfcp::qfi_t& v) {
    qos_flow_identifier.first  = true;
    qos_flow_identifier.second = v;
  }
  void set(const pfcp::rqi_t& v) {
    reflective_qos.first  = true;
    reflective_qos.second = v;
  }

  bool get(pfcp::qer_id_t& v) const {
    if (qer_id.first) {
      v = qer_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::qer_correlation_id_t& v) const {
    if (qer_correlation_id.first) {
      v = qer_correlation_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::gate_status_t& v) const {
    if (gate_status.first) {
      v = gate_status.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::mbr_t& v) const {
    if (maximum_bitrate.first) {
      v = maximum_bitrate.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::gbr_t& v) const {
    if (guaranteed_bitrate.first) {
      v = guaranteed_bitrate.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::packet_rate_t& v) const {
    if (packet_rate.first) {
      v = packet_rate.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::dl_flow_level_marking_t& v) const {
    if (dl_flow_level_marking.first) {
      v = dl_flow_level_marking.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::qfi_t& v) const {
    if (qos_flow_identifier.first) {
      v = qos_flow_identifier.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::rqi_t& v) const {
    if (reflective_qos.first) {
      v = reflective_qos.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.6-1: Create BAR IE within PFCP Session Establishment Request
class create_bar : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::bar_id_t> bar_id;
  std::pair<bool, pfcp::downlink_data_notification_delay_t>
      downlink_data_notification_delay;
  std::pair<bool, pfcp::suggested_buffering_packets_count_t>
      suggested_buffering_packets_count;

  create_bar()
      : bar_id(),
        downlink_data_notification_delay(),
        suggested_buffering_packets_count() {}

  create_bar(const create_bar& c)
      : bar_id(c.bar_id),
        downlink_data_notification_delay(c.downlink_data_notification_delay),
        suggested_buffering_packets_count(c.suggested_buffering_packets_count) {
  }

  // virtual ~create_bar() {};
  void set(const pfcp::bar_id_t& v) {
    bar_id.first  = true;
    bar_id.second = v;
  }
  void set(const pfcp::downlink_data_notification_delay_t& v) {
    downlink_data_notification_delay.first  = true;
    downlink_data_notification_delay.second = v;
  }
  void set(const pfcp::suggested_buffering_packets_count_t& v) {
    suggested_buffering_packets_count.first  = true;
    suggested_buffering_packets_count.second = v;
  }

  bool get(pfcp::bar_id_t& v) const {
    if (bar_id.first) {
      v = bar_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::downlink_data_notification_delay_t& v) const {
    if (downlink_data_notification_delay.first) {
      v = downlink_data_notification_delay.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::suggested_buffering_packets_count_t& v) const {
    if (suggested_buffering_packets_count.first) {
      v = suggested_buffering_packets_count.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.2.7-1: Create Traffic Endpoint IE within PFCP Session Establishment
// Request
class create_traffic_endpoint : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::traffic_endpoint_id_t> traffic_endpoint_id;
  std::pair<bool, pfcp::fteid_t> local_fteid;
  std::pair<bool, pfcp::network_instance_t> network_instance;
  std::pair<bool, pfcp::ue_ip_address_t> ue_ip_address;
  std::pair<bool, pfcp::ethernet_pdu_session_information_t>
      ethernet_pdu_session_information;
  std::pair<bool, pfcp::framed_route_t> framed_route;
  std::pair<bool, pfcp::framed_routing_t> framed_routing;
  std::pair<bool, pfcp::framed_ipv6_route_t> framed_ipv6_route;

  create_traffic_endpoint()
      : traffic_endpoint_id(),
        local_fteid(),
        network_instance(),
        ue_ip_address(),
        ethernet_pdu_session_information(),
        framed_route(),
        framed_routing(),
        framed_ipv6_route() {}

  create_traffic_endpoint(const create_traffic_endpoint& c)
      : traffic_endpoint_id(c.traffic_endpoint_id),
        local_fteid(c.local_fteid),
        network_instance(c.network_instance),
        ue_ip_address(c.ue_ip_address),
        ethernet_pdu_session_information(c.ethernet_pdu_session_information),
        framed_route(c.framed_route),
        framed_routing(c.framed_routing),
        framed_ipv6_route(c.framed_ipv6_route) {}

  // virtual ~create_traffic_endpoint() {};
  void set(const pfcp::traffic_endpoint_id_t& v) {
    traffic_endpoint_id.first  = true;
    traffic_endpoint_id.second = v;
  }
  void set(const pfcp::fteid_t& v) {
    local_fteid.first  = true;
    local_fteid.second = v;
  }
  void set(const pfcp::network_instance_t& v) {
    network_instance.first  = true;
    network_instance.second = v;
  }
  void set(const pfcp::ue_ip_address_t& v) {
    ue_ip_address.first  = true;
    ue_ip_address.second = v;
  }
  void set(const pfcp::ethernet_pdu_session_information_t& v) {
    ethernet_pdu_session_information.first  = true;
    ethernet_pdu_session_information.second = v;
  }
  void set(const pfcp::framed_route_t& v) {
    framed_route.first  = true;
    framed_route.second = v;
  }
  void set(const pfcp::framed_routing_t& v) {
    framed_routing.first  = true;
    framed_routing.second = v;
  }
  void set(const pfcp::framed_ipv6_route_t& v) {
    framed_ipv6_route.first  = true;
    framed_ipv6_route.second = v;
  }

  bool get(pfcp::traffic_endpoint_id_t& v) const {
    if (traffic_endpoint_id.first) {
      v = traffic_endpoint_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::fteid_t& v) const {
    if (local_fteid.first) {
      v = local_fteid.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::network_instance_t& v) const {
    if (network_instance.first) {
      v = network_instance.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ue_ip_address_t& v) const {
    if (ue_ip_address.first) {
      v = ue_ip_address.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ethernet_pdu_session_information_t& v) const {
    if (ethernet_pdu_session_information.first) {
      v = ethernet_pdu_session_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::framed_route_t& v) const {
    if (framed_route.first) {
      v = framed_route.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::framed_routing_t& v) const {
    if (framed_routing.first) {
      v = framed_routing.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::framed_ipv6_route_t& v) const {
    if (framed_ipv6_route.first) {
      v = framed_ipv6_route.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.3.2-1: Created PDR IE within PFCP Session Establishment Response
class created_pdr : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::pdr_id_t> pdr_id;
  std::pair<bool, pfcp::fteid_t> local_fteid;

  created_pdr() : pdr_id(), local_fteid() {}

  created_pdr(const created_pdr& c) {
    pdr_id      = c.pdr_id;
    local_fteid = c.local_fteid;
  }

  // virtual ~created_pdr() {};
  void set(const pfcp::pdr_id_t& v) {
    pdr_id.first  = true;
    pdr_id.second = v;
  }
  void set(const pfcp::fteid_t& v) {
    local_fteid.first  = true;
    local_fteid.second = v;
  }

  bool get(pfcp::pdr_id_t& v) const {
    if (pdr_id.first) {
      v = pdr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::fteid_t& v) const {
    if (local_fteid.first) {
      v = local_fteid.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.3.3-1: Load Control Information IE within PFCP Session
// Establishment Response
class load_control_information : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::sequence_number_t> load_control_sequence_number;
  std::pair<bool, pfcp::metric_t> load_metric;

  load_control_information() : load_control_sequence_number(), load_metric() {}

  load_control_information(const load_control_information& l) {
    load_control_sequence_number = l.load_control_sequence_number;
    load_metric                  = l.load_metric;
  }

  // virtual ~load_control_information() {};
  void set(const pfcp::sequence_number_t& v) {
    load_control_sequence_number.first  = true;
    load_control_sequence_number.second = v;
  }
  void set(const pfcp::metric_t& v) {
    load_metric.first  = true;
    load_metric.second = v;
  }

  bool get(pfcp::sequence_number_t& v) const {
    if (load_control_sequence_number.first) {
      v = load_control_sequence_number.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::metric_t& v) const {
    if (load_metric.first) {
      v = load_metric.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.3.4-1: Overload Control Information IE within PFCP Session
// Establishment Response
class overload_control_information : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::sequence_number_t> overload_control_sequence_number;
  std::pair<bool, pfcp::metric_t> overload_reduction_metric;
  std::pair<bool, pfcp::timer_t> period_of_validity;
  std::pair<bool, pfcp::oci_flags_t> overload_control_information_flags;

  overload_control_information()
      : overload_control_sequence_number(),
        overload_reduction_metric(),
        period_of_validity(),
        overload_control_information_flags() {}

  overload_control_information(const overload_control_information& o) {
    overload_control_sequence_number   = o.overload_control_sequence_number;
    overload_reduction_metric          = o.overload_reduction_metric;
    period_of_validity                 = o.period_of_validity;
    overload_control_information_flags = o.overload_control_information_flags;
  }

  // virtual ~overload_control_information() {};
  void set(const pfcp::sequence_number_t& v) {
    overload_control_sequence_number.first  = true;
    overload_control_sequence_number.second = v;
  }
  void set(const pfcp::metric_t& v) {
    overload_reduction_metric.first  = true;
    overload_reduction_metric.second = v;
  }
  void set(const pfcp::timer_t& v) {
    period_of_validity.first  = true;
    period_of_validity.second = v;
  }
  void set(const pfcp::oci_flags_t& v) {
    overload_control_information_flags.first  = true;
    overload_control_information_flags.second = v;
  }

  bool get(pfcp::sequence_number_t& v) const {
    if (overload_control_sequence_number.first) {
      v = overload_control_sequence_number.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::metric_t& v) const {
    if (overload_reduction_metric.first) {
      v = overload_reduction_metric.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::timer_t& v) const {
    if (period_of_validity.first) {
      v = period_of_validity.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::oci_flags_t& v) const {
    if (overload_control_information_flags.first) {
      v = overload_control_information_flags.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.2-1: Update PDR IE within PFCP Session Modification Request
class update_pdr : public pfcp::pfcp_ies_container {
 public:
  pfcp::pdr_id_t pdr_id;
  std::pair<bool, pfcp::outer_header_removal_t> outer_header_removal;
  std::pair<bool, pfcp::precedence_t> precedence;
  std::pair<bool, pfcp::pdi> pdi;
  std::pair<bool, pfcp::far_id_t> far_id;
  std::pair<bool, pfcp::urr_id_t> urr_id;
  std::pair<bool, pfcp::qer_id_t> qer_id;
  std::vector<pfcp::activate_predefined_rules_t> activate_predefined_rules;
  std::vector<pfcp::deactivate_predefined_rules_t> deactivate_predefined_rules;

  update_pdr()
      : pdr_id(),
        outer_header_removal(),
        precedence(),
        pdi(),
        far_id(),
        urr_id(),
        qer_id(),
        activate_predefined_rules(),
        deactivate_predefined_rules() {}

  update_pdr(const update_pdr& u)
      : pdr_id(u.pdr_id),
        outer_header_removal(u.outer_header_removal),
        precedence(u.precedence),
        pdi(u.pdi),
        far_id(u.far_id),
        urr_id(u.urr_id),
        qer_id(u.qer_id),
        activate_predefined_rules(u.activate_predefined_rules),
        deactivate_predefined_rules(u.deactivate_predefined_rules) {}

  // virtual ~update_pdr() {};
  void set(const pfcp::pdr_id_t& v) { pdr_id = v; }
  void set(const pfcp::outer_header_removal_t& v) {
    outer_header_removal.first  = true;
    outer_header_removal.second = v;
  }
  void set(const pfcp::precedence_t& v) {
    precedence.first  = true;
    precedence.second = v;
  }
  void set(const pfcp::pdi& v) {
    pdi.first  = true;
    pdi.second = v;
  }
  void set(const pfcp::far_id_t& v) {
    far_id.first  = true;
    far_id.second = v;
  }
  void set(const pfcp::urr_id_t& v) {
    urr_id.first  = true;
    urr_id.second = v;
  }
  void set(const pfcp::qer_id_t& v) {
    qer_id.first  = true;
    qer_id.second = v;
  }
  void set(const pfcp::activate_predefined_rules_t& v) {
    activate_predefined_rules.push_back(v);
  }
  void set(const pfcp::deactivate_predefined_rules_t& v) {
    deactivate_predefined_rules.push_back(v);
  }

  bool get(pfcp::pdr_id_t& v) const {
    v = pdr_id;
    return true;
  }
  bool get(pfcp::outer_header_removal_t& v) const {
    if (outer_header_removal.first) {
      v = outer_header_removal.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::precedence_t& v) const {
    if (precedence.first) {
      v = precedence.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::pdi& v) const {
    if (pdi.first) {
      v = pdi.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::far_id_t& v) const {
    if (far_id.first) {
      v = far_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::urr_id_t& v) const {
    if (urr_id.first) {
      v = urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::qer_id_t& v) const {
    if (qer_id.first) {
      v = qer_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.3-1: Update FAR IE within PFCP Session Modification Request
class update_far : public pfcp::pfcp_ies_container {
 public:
  pfcp::far_id_t far_id;
  std::pair<bool, pfcp::apply_action_t> apply_action;
  std::pair<bool, pfcp::update_forwarding_parameters>
      update_forwarding_parameters;
  std::pair<bool, pfcp::update_duplicating_parameters>
      update_duplicating_parameters;
  std::pair<bool, pfcp::bar_id_t> bar_id;

  update_far()
      : far_id(),
        apply_action(),
        update_forwarding_parameters(),
        update_duplicating_parameters(),
        bar_id() {}

  update_far(const update_far& u)
      : far_id(u.far_id),
        apply_action(u.apply_action),
        update_forwarding_parameters(u.update_forwarding_parameters),
        update_duplicating_parameters(u.update_duplicating_parameters),
        bar_id(u.bar_id) {}

  // virtual ~update_far() {};
  void set(const pfcp::far_id_t& v) { far_id = v; }
  void set(const pfcp::apply_action_t& v) {
    apply_action.first  = true;
    apply_action.second = v;
  }
  void set(const pfcp::update_forwarding_parameters& v) {
    update_forwarding_parameters.first  = true;
    update_forwarding_parameters.second = v;
  }
  void set(const pfcp::update_duplicating_parameters& v) {
    update_duplicating_parameters.first  = true;
    update_duplicating_parameters.second = v;
  }
  void set(const pfcp::bar_id_t& v) {
    bar_id.first  = true;
    bar_id.second = v;
  }

  bool get(pfcp::far_id_t& v) const {
    v = far_id;
    return true;
  }
  bool get(pfcp::apply_action_t& v) const {
    if (apply_action.first) {
      v = apply_action.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::update_forwarding_parameters& v) const {
    if (update_forwarding_parameters.first) {
      v = update_forwarding_parameters.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::update_duplicating_parameters& v) const {
    if (update_duplicating_parameters.first) {
      v = update_duplicating_parameters.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::bar_id_t& v) const {
    if (bar_id.first) {
      v = bar_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.4-1: Update URR IE within PFCP Session Modification Request
class update_urr : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::urr_id_t> urr_id;
  std::pair<bool, pfcp::measurement_method_t> measurement_method;
  std::pair<bool, pfcp::reporting_triggers_t> reporting_triggers;
  std::pair<bool, pfcp::measurement_period_t> measurement_period;
  std::pair<bool, pfcp::volume_threshold_t> volume_threshold;
  std::pair<bool, pfcp::volume_quota_t> volume_quota;
  std::pair<bool, pfcp::time_threshold_t> time_threshold;
  std::pair<bool, pfcp::time_quota_t> time_quota;
  std::pair<bool, pfcp::quota_holding_time_t> quota_holding_time;
  std::pair<bool, pfcp::dropped_dl_traffic_threshold_t>
      dropped_dl_traffic_threshold;
  std::pair<bool, pfcp::monitoring_time_t> monitoring_time;
  std::pair<bool, pfcp::event_information> event_information;
  std::pair<bool, pfcp::subsequent_volume_threshold_t>
      subsequent_volume_threshold;
  std::pair<bool, pfcp::subsequent_time_threshold_t> subsequent_time_threshold;
  std::pair<bool, pfcp::subsequent_volume_quota_t> subsequent_volume_quota;
  std::pair<bool, pfcp::subsequent_time_quota_t> subsequent_time_quota;
  std::pair<bool, pfcp::inactivity_detection_time_t> inactivity_detection_time;
  std::pair<bool, pfcp::linked_urr_id_t> linked_urr_id;
  std::pair<bool, pfcp::measurement_information_t> measurement_information;
  std::pair<bool, pfcp::time_quota_mechanism_t> time_quota_mechanism;
  std::pair<bool, pfcp::aggregated_urrs> aggregated_urrs;
  std::pair<bool, pfcp::far_id_t> far_id_for_quota_action;
  std::pair<bool, pfcp::ethernet_inactivity_timer_t> ethernet_inactivity_timer;
  std::pair<bool, pfcp::additional_monitoring_time> additional_monitoring_time;

  update_urr()
      : urr_id(),
        measurement_method(),
        reporting_triggers(),
        measurement_period(),
        volume_threshold(),
        volume_quota(),
        time_threshold(),
        time_quota(),
        quota_holding_time(),
        dropped_dl_traffic_threshold(),
        monitoring_time(),
        event_information(),
        subsequent_volume_threshold(),
        subsequent_time_threshold(),
        subsequent_volume_quota(),
        subsequent_time_quota(),
        inactivity_detection_time(),
        linked_urr_id(),
        measurement_information(),
        time_quota_mechanism(),
        aggregated_urrs(),
        far_id_for_quota_action(),
        ethernet_inactivity_timer(),
        additional_monitoring_time() {}

  update_urr(const update_urr& u)
      : urr_id(u.urr_id),
        measurement_method(u.measurement_method),
        reporting_triggers(u.reporting_triggers),
        measurement_period(u.measurement_period),
        volume_threshold(u.volume_threshold),
        volume_quota(u.volume_quota),
        time_threshold(u.time_threshold),
        time_quota(u.time_quota),
        quota_holding_time(u.quota_holding_time),
        dropped_dl_traffic_threshold(u.dropped_dl_traffic_threshold),
        monitoring_time(u.monitoring_time),
        event_information(u.event_information),
        subsequent_volume_threshold(u.subsequent_volume_threshold),
        subsequent_time_threshold(u.subsequent_time_threshold),
        subsequent_volume_quota(u.subsequent_volume_quota),
        subsequent_time_quota(u.subsequent_time_quota),
        inactivity_detection_time(u.inactivity_detection_time),
        linked_urr_id(u.linked_urr_id),
        measurement_information(u.measurement_information),
        time_quota_mechanism(u.time_quota_mechanism),
        aggregated_urrs(u.aggregated_urrs),
        far_id_for_quota_action(u.far_id_for_quota_action),
        ethernet_inactivity_timer(u.ethernet_inactivity_timer),
        additional_monitoring_time(u.additional_monitoring_time) {}

  // virtual ~update_urr() {};
  void set(const pfcp::urr_id_t& v) {
    urr_id.first  = true;
    urr_id.second = v;
  }
  void set(const pfcp::measurement_method_t& v) {
    measurement_method.first  = true;
    measurement_method.second = v;
  }
  void set(const pfcp::reporting_triggers_t& v) {
    reporting_triggers.first  = true;
    reporting_triggers.second = v;
  }
  void set(const pfcp::measurement_period_t& v) {
    measurement_period.first  = true;
    measurement_period.second = v;
  }
  void set(const pfcp::volume_threshold_t& v) {
    volume_threshold.first  = true;
    volume_threshold.second = v;
  }
  void set(const pfcp::volume_quota_t& v) {
    volume_quota.first  = true;
    volume_quota.second = v;
  }
  void set(const pfcp::time_threshold_t& v) {
    time_threshold.first  = true;
    time_threshold.second = v;
  }
  void set(const pfcp::time_quota_t& v) {
    time_quota.first  = true;
    time_quota.second = v;
  }
  void set(const pfcp::quota_holding_time_t& v) {
    quota_holding_time.first  = true;
    quota_holding_time.second = v;
  }
  void set(const pfcp::dropped_dl_traffic_threshold_t& v) {
    dropped_dl_traffic_threshold.first  = true;
    dropped_dl_traffic_threshold.second = v;
  }
  void set(const pfcp::monitoring_time_t& v) {
    monitoring_time.first  = true;
    monitoring_time.second = v;
  }
  void set(const pfcp::event_information& v) {
    event_information.first  = true;
    event_information.second = v;
  }
  void set(const pfcp::subsequent_volume_threshold_t& v) {
    subsequent_volume_threshold.first  = true;
    subsequent_volume_threshold.second = v;
  }
  void set(const pfcp::subsequent_time_threshold_t& v) {
    subsequent_time_threshold.first  = true;
    subsequent_time_threshold.second = v;
  }
  void set(const pfcp::subsequent_volume_quota_t& v) {
    subsequent_volume_quota.first  = true;
    subsequent_volume_quota.second = v;
  }
  void set(const pfcp::subsequent_time_quota_t& v) {
    subsequent_time_quota.first  = true;
    subsequent_time_quota.second = v;
  }
  void set(const pfcp::inactivity_detection_time_t& v) {
    inactivity_detection_time.first  = true;
    inactivity_detection_time.second = v;
  }
  void set(const pfcp::linked_urr_id_t& v) {
    linked_urr_id.first  = true;
    linked_urr_id.second = v;
  }
  void set(const pfcp::measurement_information_t& v) {
    measurement_information.first  = true;
    measurement_information.second = v;
  }
  void set(const pfcp::time_quota_mechanism_t& v) {
    time_quota_mechanism.first  = true;
    time_quota_mechanism.second = v;
  }
  void set(const pfcp::aggregated_urrs& v) {
    aggregated_urrs.first  = true;
    aggregated_urrs.second = v;
  }
  void set(const pfcp::far_id_t& v) {
    far_id_for_quota_action.first  = true;
    far_id_for_quota_action.second = v;
  }
  void set(const pfcp::ethernet_inactivity_timer_t& v) {
    ethernet_inactivity_timer.first  = true;
    ethernet_inactivity_timer.second = v;
  }
  void set(const pfcp::additional_monitoring_time& v) {
    additional_monitoring_time.first  = true;
    additional_monitoring_time.second = v;
  }

  bool get(pfcp::urr_id_t& v) const {
    if (urr_id.first) {
      v = urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::measurement_method_t& v) const {
    if (measurement_method.first) {
      v = measurement_method.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::reporting_triggers_t& v) const {
    if (reporting_triggers.first) {
      v = reporting_triggers.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::measurement_period_t& v) const {
    if (measurement_period.first) {
      v = measurement_period.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::volume_threshold_t& v) const {
    if (volume_threshold.first) {
      v = volume_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::volume_quota_t& v) const {
    if (volume_quota.first) {
      v = volume_quota.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_threshold_t& v) const {
    if (time_threshold.first) {
      v = time_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_quota_t& v) const {
    if (time_quota.first) {
      v = time_quota.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::quota_holding_time_t& v) const {
    if (quota_holding_time.first) {
      v = quota_holding_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::dropped_dl_traffic_threshold_t& v) const {
    if (dropped_dl_traffic_threshold.first) {
      v = dropped_dl_traffic_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::monitoring_time_t& v) const {
    if (monitoring_time.first) {
      v = monitoring_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::event_information& v) const {
    if (event_information.first) {
      v = event_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_volume_threshold_t& v) const {
    if (subsequent_volume_threshold.first) {
      v = subsequent_volume_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_time_threshold_t& v) const {
    if (subsequent_time_threshold.first) {
      v = subsequent_time_threshold.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_volume_quota_t& v) const {
    if (subsequent_volume_quota.first) {
      v = subsequent_volume_quota.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::subsequent_time_quota_t& v) const {
    if (subsequent_time_quota.first) {
      v = subsequent_time_quota.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::inactivity_detection_time_t& v) const {
    if (inactivity_detection_time.first) {
      v = inactivity_detection_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::linked_urr_id_t& v) const {
    if (linked_urr_id.first) {
      v = linked_urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::measurement_information_t& v) const {
    if (measurement_information.first) {
      v = measurement_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_quota_mechanism_t& v) const {
    if (time_quota_mechanism.first) {
      v = time_quota_mechanism.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::aggregated_urrs& v) const {
    if (aggregated_urrs.first) {
      v = aggregated_urrs.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::far_id_t& v) const {
    if (far_id_for_quota_action.first) {
      v = far_id_for_quota_action.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ethernet_inactivity_timer_t& v) const {
    if (ethernet_inactivity_timer.first) {
      v = ethernet_inactivity_timer.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::additional_monitoring_time& v) const {
    if (additional_monitoring_time.first) {
      v = additional_monitoring_time.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.5-1: Update QER IE within PFCP Session Modification Request
class update_qer : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::qer_id_t> qer_id;
  std::pair<bool, pfcp::qer_correlation_id_t> qer_correlation_id;
  std::pair<bool, pfcp::gate_status_t> gate_status;
  std::pair<bool, pfcp::mbr_t> maximum_bitrate;
  std::pair<bool, pfcp::gbr_t> guaranteed_bitrate;
  std::pair<bool, pfcp::packet_rate_t> packet_rate;
  std::pair<bool, pfcp::dl_flow_level_marking_t> dl_flow_level_marking;
  std::pair<bool, pfcp::qfi_t> qos_flow_identifier;
  std::pair<bool, pfcp::rqi_t> reflective_qos;

  update_qer()
      : qer_id(),
        qer_correlation_id(),
        gate_status(),
        maximum_bitrate(),
        guaranteed_bitrate(),
        packet_rate(),
        dl_flow_level_marking(),
        qos_flow_identifier(),
        reflective_qos() {}

  update_qer(const update_qer& u) {
    qer_id                = u.qer_id;
    qer_correlation_id    = u.qer_correlation_id;
    gate_status           = u.gate_status;
    maximum_bitrate       = u.maximum_bitrate;
    guaranteed_bitrate    = u.guaranteed_bitrate;
    packet_rate           = u.packet_rate;
    dl_flow_level_marking = u.dl_flow_level_marking;
    qos_flow_identifier   = u.qos_flow_identifier;
    reflective_qos        = u.reflective_qos;
  }

  // virtual ~update_qer() {};
  void set(const pfcp::qer_id_t& v) {
    qer_id.first  = true;
    qer_id.second = v;
  }
  void set(const pfcp::qer_correlation_id_t& v) {
    qer_correlation_id.first  = true;
    qer_correlation_id.second = v;
  }
  void set(const pfcp::gate_status_t& v) {
    gate_status.first  = true;
    gate_status.second = v;
  }
  void set(const pfcp::mbr_t& v) {
    maximum_bitrate.first  = true;
    maximum_bitrate.second = v;
  }
  void set(const pfcp::gbr_t& v) {
    guaranteed_bitrate.first  = true;
    guaranteed_bitrate.second = v;
  }
  void set(const pfcp::packet_rate_t& v) {
    packet_rate.first  = true;
    packet_rate.second = v;
  }
  void set(const pfcp::dl_flow_level_marking_t& v) {
    dl_flow_level_marking.first  = true;
    dl_flow_level_marking.second = v;
  }
  void set(const pfcp::qfi_t& v) {
    qos_flow_identifier.first  = true;
    qos_flow_identifier.second = v;
  }
  void set(const pfcp::rqi_t& v) {
    reflective_qos.first  = true;
    reflective_qos.second = v;
  }

  bool get(pfcp::qer_id_t& v) const {
    if (qer_id.first) {
      v = qer_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::qer_correlation_id_t& v) const {
    if (qer_correlation_id.first) {
      v = qer_correlation_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::gate_status_t& v) const {
    if (gate_status.first) {
      v = gate_status.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::mbr_t& v) const {
    if (maximum_bitrate.first) {
      v = maximum_bitrate.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::gbr_t& v) const {
    if (guaranteed_bitrate.first) {
      v = guaranteed_bitrate.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::packet_rate_t& v) const {
    if (packet_rate.first) {
      v = packet_rate.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::dl_flow_level_marking_t& v) const {
    if (dl_flow_level_marking.first) {
      v = dl_flow_level_marking.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::qfi_t& v) const {
    if (qos_flow_identifier.first) {
      v = qos_flow_identifier.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::rqi_t& v) const {
    if (reflective_qos.first) {
      v = reflective_qos.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.6-1: Remove PDR IE within PFCP Session Modification Request
class remove_pdr : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::pdr_id_t> pdr_id;

  remove_pdr() : pdr_id() {}

  remove_pdr(const remove_pdr& r) { pdr_id = r.pdr_id; }

  // virtual ~remove_pdr() {};
  void set(const pfcp::pdr_id_t& v) {
    pdr_id.first  = true;
    pdr_id.second = v;
  }

  bool get(pfcp::pdr_id_t& v) const {
    if (pdr_id.first) {
      v = pdr_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.7-1: Remove FAR IE within PFCP Session Modification Request
class remove_far : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::far_id_t> far_id;

  remove_far() : far_id() {}

  remove_far(const remove_far& r) { far_id = r.far_id; }

  // virtual ~remove_far() {};
  void set(const pfcp::far_id_t& v) {
    far_id.first  = true;
    far_id.second = v;
  }

  bool get(pfcp::far_id_t& v) const {
    if (far_id.first) {
      v = far_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.8-1: Remove URR IE within PFCP Session Modification Request
class remove_urr : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::urr_id_t> urr_id;

  remove_urr() : urr_id() {}

  remove_urr(const remove_urr& r) { urr_id = r.urr_id; }

  // virtual ~remove_urr() {};
  void set(const pfcp::urr_id_t& v) {
    urr_id.first  = true;
    urr_id.second = v;
  }

  bool get(pfcp::urr_id_t& v) const {
    if (urr_id.first) {
      v = urr_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.9-1: Remove QER IE PFCP Session Modification Request
class remove_qer : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::qer_id_t> qer_id;

  remove_qer() : qer_id() {}

  remove_qer(const remove_qer& r) { qer_id = r.qer_id; }

  // virtual ~remove_qer() {};
  void set(const pfcp::qer_id_t& v) {
    qer_id.first  = true;
    qer_id.second = v;
  }

  bool get(pfcp::qer_id_t& v) const {
    if (qer_id.first) {
      v = qer_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.10-1: Query URR IE within PFCP Session Modification Request
class query_urr : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::urr_id_t> urr_id;

  query_urr() : urr_id() {}

  query_urr(const query_urr& q) { urr_id = q.urr_id; }

  // virtual ~query_urr() {};
  void set(const pfcp::urr_id_t& v) {
    urr_id.first  = true;
    urr_id.second = v;
  }

  bool get(pfcp::urr_id_t& v) const {
    if (urr_id.first) {
      v = urr_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.11-1: Update BAR IE within PFCP Session Modification Request
class update_bar_within_pfcp_session_modification_request
    : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::bar_id_t> bar_id;
  std::pair<bool, pfcp::downlink_data_notification_delay_t>
      downlink_data_notification_delay;
  std::pair<bool, pfcp::suggested_buffering_packets_count_t>
      suggested_buffering_packets_count;

  update_bar_within_pfcp_session_modification_request()
      : bar_id(),
        downlink_data_notification_delay(),
        suggested_buffering_packets_count() {}

  update_bar_within_pfcp_session_modification_request(
      const update_bar_within_pfcp_session_modification_request& u) {
    bar_id                            = u.bar_id;
    downlink_data_notification_delay  = u.downlink_data_notification_delay;
    suggested_buffering_packets_count = u.suggested_buffering_packets_count;
  }

  // virtual ~update_bar_within_pfcp() {};
  void set(const pfcp::bar_id_t& v) {
    bar_id.first  = true;
    bar_id.second = v;
  }
  void set(const pfcp::downlink_data_notification_delay_t& v) {
    downlink_data_notification_delay.first  = true;
    downlink_data_notification_delay.second = v;
  }
  void set(const pfcp::suggested_buffering_packets_count_t& v) {
    suggested_buffering_packets_count.first  = true;
    suggested_buffering_packets_count.second = v;
  }

  bool get(pfcp::bar_id_t& v) const {
    if (bar_id.first) {
      v = bar_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::downlink_data_notification_delay_t& v) const {
    if (downlink_data_notification_delay.first) {
      v = downlink_data_notification_delay.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::suggested_buffering_packets_count_t& v) const {
    if (suggested_buffering_packets_count.first) {
      v = suggested_buffering_packets_count.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.12-1: Remove BAR IE within PFCP Session Modification Request
class remove_bar : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::bar_id_t> bar_id;

  remove_bar() : bar_id() {}

  remove_bar(const remove_bar& r) : bar_id(r.bar_id) {}

  // virtual ~remove_bar() {};
  void set(const pfcp::bar_id_t& v) {
    bar_id.first  = true;
    bar_id.second = v;
  }

  bool get(pfcp::bar_id_t& v) const {
    if (bar_id.first) {
      v = bar_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.3.5-1: Created Traffic Endpoint IE within Sx Session Establishment
// Response
class created_traffic_endpoint : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::traffic_endpoint_id_t> traffic_endpoint_id;
  std::pair<bool, pfcp::fteid_t> local_fteid;

  created_traffic_endpoint() : traffic_endpoint_id(), local_fteid() {}

  created_traffic_endpoint(const created_traffic_endpoint& c)
      : traffic_endpoint_id(c.traffic_endpoint_id),
        local_fteid(c.local_fteid) {}

  // virtual ~created_traffic_endpoint() {};
  void set(const pfcp::traffic_endpoint_id_t& v) {
    traffic_endpoint_id.first  = true;
    traffic_endpoint_id.second = v;
  }
  void set(const pfcp::fteid_t& v) {
    local_fteid.first  = true;
    local_fteid.second = v;
  }

  bool get(pfcp::traffic_endpoint_id_t& v) const {
    if (traffic_endpoint_id.first) {
      v = traffic_endpoint_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::fteid_t& v) const {
    if (local_fteid.first) {
      v = local_fteid.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.13-1: Update Traffic Endpoint IE within Sx Session Modification
// Request
class update_traffic_endpoint : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::traffic_endpoint_id_t> traffic_endpoint_id;
  std::pair<bool, pfcp::fteid_t> local_fteid;
  std::pair<bool, pfcp::network_instance_t> network_instance;
  std::pair<bool, pfcp::ue_ip_address_t> ue_ip_address;
  std::pair<bool, pfcp::framed_route_t> framed_route;
  std::pair<bool, pfcp::framed_routing_t> framed_routing;
  std::pair<bool, pfcp::framed_ipv6_route_t> framed_ipv6_route;

  update_traffic_endpoint()
      : traffic_endpoint_id(),
        local_fteid(),
        network_instance(),
        ue_ip_address(),
        framed_route(),
        framed_routing(),
        framed_ipv6_route() {}

  update_traffic_endpoint(const update_traffic_endpoint& u)
      : traffic_endpoint_id(u.traffic_endpoint_id),
        local_fteid(u.local_fteid),
        network_instance(u.network_instance),
        ue_ip_address(u.ue_ip_address),
        framed_route(u.framed_route),
        framed_routing(u.framed_routing),
        framed_ipv6_route(u.framed_ipv6_route) {}

  // virtual ~update_traffic_endpoint() {};
  void set(const pfcp::traffic_endpoint_id_t& v) {
    traffic_endpoint_id.first  = true;
    traffic_endpoint_id.second = v;
  }
  void set(const pfcp::fteid_t& v) {
    local_fteid.first  = true;
    local_fteid.second = v;
  }
  void set(const pfcp::network_instance_t& v) {
    network_instance.first  = true;
    network_instance.second = v;
  }
  void set(const pfcp::ue_ip_address_t& v) {
    ue_ip_address.first  = true;
    ue_ip_address.second = v;
  }
  void set(const pfcp::framed_route_t& v) {
    framed_route.first  = true;
    framed_route.second = v;
  }
  void set(const pfcp::framed_routing_t& v) {
    framed_routing.first  = true;
    framed_routing.second = v;
  }
  void set(const pfcp::framed_ipv6_route_t& v) {
    framed_ipv6_route.first  = true;
    framed_ipv6_route.second = v;
  }

  bool get(pfcp::traffic_endpoint_id_t& v) const {
    if (traffic_endpoint_id.first) {
      v = traffic_endpoint_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::fteid_t& v) const {
    if (local_fteid.first) {
      v = local_fteid.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::network_instance_t& v) const {
    if (network_instance.first) {
      v = network_instance.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ue_ip_address_t& v) const {
    if (ue_ip_address.first) {
      v = ue_ip_address.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::framed_route_t& v) const {
    if (framed_route.first) {
      v = framed_route.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::framed_routing_t& v) const {
    if (framed_routing.first) {
      v = framed_routing.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::framed_ipv6_route_t& v) const {
    if (framed_ipv6_route.first) {
      v = framed_ipv6_route.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.4.14-1: Remove Traffic Endpoint IE within Sx Session Modification
// Request
class remove_traffic_endpoint : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::traffic_endpoint_id_t> traffic_endpoint_id;

  remove_traffic_endpoint() : traffic_endpoint_id() {}

  remove_traffic_endpoint(const remove_traffic_endpoint& r)
      : traffic_endpoint_id(r.traffic_endpoint_id) {}

  // virtual ~remove_traffic_endpoint() {};
  void set(const pfcp::traffic_endpoint_id_t& v) {
    traffic_endpoint_id.first  = true;
    traffic_endpoint_id.second = v;
  }

  bool get(pfcp::traffic_endpoint_id_t& v) const {
    if (traffic_endpoint_id.first) {
      v = traffic_endpoint_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.8.3-3: Ethernet Traffic Information IE within Usage Report IE
class ethernet_traffic_information : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::mac_addresses_detected_t> mac_addresses_detected;
  std::pair<bool, pfcp::mac_addresses_removed_t> mac_addresses_removed;

  ethernet_traffic_information()
      : mac_addresses_detected(), mac_addresses_removed() {}

  ethernet_traffic_information(const ethernet_traffic_information& e)
      : mac_addresses_detected(e.mac_addresses_detected),
        mac_addresses_removed(e.mac_addresses_removed) {}

  ethernet_traffic_information& operator=(ethernet_traffic_information i) {
    std::swap(mac_addresses_detected, i.mac_addresses_detected);
    std::swap(mac_addresses_removed, i.mac_addresses_removed);
    return *this;
  }

  // virtual ~ethernet_traffic_information() {};
  void set(const pfcp::mac_addresses_detected_t& v) {
    mac_addresses_detected.first  = true;
    mac_addresses_detected.second = v;
  }
  void set(const pfcp::mac_addresses_removed_t& v) {
    mac_addresses_removed.first  = true;
    mac_addresses_removed.second = v;
  }

  bool get(pfcp::mac_addresses_detected_t& v) const {
    if (mac_addresses_detected.first) {
      v = mac_addresses_detected.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::mac_addresses_removed_t& v) const {
    if (mac_addresses_removed.first) {
      v = mac_addresses_removed.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.5.2-1: Usage Report IE within PFCP Session Modification Response
class usage_report_within_pfcp_session_modification_response
    : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::urr_id_t> urr_id;
  std::pair<bool, pfcp::ur_seqn_t> ur_seqn;
  std::pair<bool, pfcp::usage_report_trigger_t> usage_report_trigger;
  std::pair<bool, pfcp::start_time_t> start_time;
  std::pair<bool, pfcp::end_time_t> end_time;
  std::pair<bool, pfcp::volume_measurement_t> volume_measurement;
  std::pair<bool, pfcp::duration_measurement_t> duration_measurement;
  std::pair<bool, pfcp::time_of_first_packet_t> time_of_first_packet;
  std::pair<bool, pfcp::time_of_last_packet_t> time_of_last_packet;
  std::pair<bool, pfcp::usage_information_t> usage_information;
  std::pair<bool, pfcp::query_urr_reference_t> query_urr_reference;
  std::pair<bool, pfcp::ethernet_traffic_information>
      ethernet_traffic_information;

  usage_report_within_pfcp_session_modification_response()
      : urr_id(),
        ur_seqn(),
        usage_report_trigger(),
        start_time(),
        end_time(),
        volume_measurement(),
        duration_measurement(),
        time_of_first_packet(),
        time_of_last_packet(),
        usage_information(),
        query_urr_reference(),
        ethernet_traffic_information() {}

  usage_report_within_pfcp_session_modification_response(
      const usage_report_within_pfcp_session_modification_response& u)
      : urr_id(u.urr_id),
        ur_seqn(u.ur_seqn),
        usage_report_trigger(u.usage_report_trigger),
        start_time(u.start_time),
        end_time(u.end_time),
        volume_measurement(u.volume_measurement),
        duration_measurement(u.duration_measurement),
        time_of_first_packet(u.time_of_first_packet),
        time_of_last_packet(u.time_of_last_packet),
        usage_information(u.usage_information),
        query_urr_reference(u.query_urr_reference),
        ethernet_traffic_information(u.ethernet_traffic_information) {}

  // virtual ~usage_report_within_pfcp_session_modification_response() {};
  void set(const pfcp::urr_id_t& v) {
    urr_id.first  = true;
    urr_id.second = v;
  }
  void set(const pfcp::ur_seqn_t& v) {
    ur_seqn.first  = true;
    ur_seqn.second = v;
  }
  void set(const pfcp::usage_report_trigger_t& v) {
    usage_report_trigger.first  = true;
    usage_report_trigger.second = v;
  }
  void set(const pfcp::start_time_t& v) {
    start_time.first  = true;
    start_time.second = v;
  }
  void set(const pfcp::end_time_t& v) {
    end_time.first  = true;
    end_time.second = v;
  }
  void set(const pfcp::volume_measurement_t& v) {
    volume_measurement.first  = true;
    volume_measurement.second = v;
  }
  void set(const pfcp::duration_measurement_t& v) {
    duration_measurement.first  = true;
    duration_measurement.second = v;
  }
  void set(const pfcp::time_of_first_packet_t& v) {
    time_of_first_packet.first  = true;
    time_of_first_packet.second = v;
  }
  void set(const pfcp::time_of_last_packet_t& v) {
    time_of_last_packet.first  = true;
    time_of_last_packet.second = v;
  }
  void set(const pfcp::usage_information_t& v) {
    usage_information.first  = true;
    usage_information.second = v;
  }
  void set(const pfcp::query_urr_reference_t& v) {
    query_urr_reference.first  = true;
    query_urr_reference.second = v;
  }
  void set(const pfcp::ethernet_traffic_information& v) {
    ethernet_traffic_information.first  = true;
    ethernet_traffic_information.second = v;
  }

  bool get(pfcp::urr_id_t& v) const {
    if (urr_id.first) {
      v = urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ur_seqn_t& v) const {
    if (ur_seqn.first) {
      v = ur_seqn.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::usage_report_trigger_t& v) const {
    if (usage_report_trigger.first) {
      v = usage_report_trigger.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::start_time_t& v) const {
    if (start_time.first) {
      v = start_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::end_time_t& v) const {
    if (end_time.first) {
      v = end_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::volume_measurement_t& v) const {
    if (volume_measurement.first) {
      v = volume_measurement.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::duration_measurement_t& v) const {
    if (duration_measurement.first) {
      v = duration_measurement.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_of_first_packet_t& v) const {
    if (time_of_first_packet.first) {
      v = time_of_first_packet.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_of_last_packet_t& v) const {
    if (time_of_last_packet.first) {
      v = time_of_last_packet.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::usage_information_t& v) const {
    if (usage_information.first) {
      v = usage_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::query_urr_reference_t& v) const {
    if (query_urr_reference.first) {
      v = query_urr_reference.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ethernet_traffic_information& v) const {
    if (ethernet_traffic_information.first) {
      v = ethernet_traffic_information.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.7.2-1: Usage Report IE within PFCP Session Deletion Response
class usage_report_within_pfcp_session_deletion_response
    : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::urr_id_t> urr_id;
  std::pair<bool, pfcp::ur_seqn_t> ur_seqn;
  std::pair<bool, pfcp::usage_report_trigger_t> usage_report_trigger;
  std::pair<bool, pfcp::start_time_t> start_time;
  std::pair<bool, pfcp::end_time_t> end_time;
  std::pair<bool, pfcp::volume_measurement_t> volume_measurement;
  std::pair<bool, pfcp::duration_measurement_t> duration_measurement;
  std::pair<bool, pfcp::time_of_first_packet_t> time_of_first_packet;
  std::pair<bool, pfcp::time_of_last_packet_t> time_of_last_packet;
  std::pair<bool, pfcp::usage_information_t> usage_information;
  std::pair<bool, pfcp::ethernet_traffic_information>
      ethernet_traffic_information;

  usage_report_within_pfcp_session_deletion_response()
      : urr_id(),
        ur_seqn(),
        usage_report_trigger(),
        start_time(),
        end_time(),
        volume_measurement(),
        duration_measurement(),
        time_of_first_packet(),
        time_of_last_packet(),
        usage_information(),
        ethernet_traffic_information() {}

  usage_report_within_pfcp_session_deletion_response(
      const usage_report_within_pfcp_session_deletion_response& u)
      : urr_id(u.urr_id),
        ur_seqn(u.ur_seqn),
        usage_report_trigger(u.usage_report_trigger),
        start_time(u.start_time),
        end_time(u.end_time),
        volume_measurement(u.volume_measurement),
        duration_measurement(u.duration_measurement),
        time_of_first_packet(u.time_of_first_packet),
        time_of_last_packet(u.time_of_last_packet),
        usage_information(u.usage_information),
        ethernet_traffic_information(u.ethernet_traffic_information) {}

  // virtual ~usage_report_within_pfcp_session_deletion_response() {};
  void set(const pfcp::urr_id_t& v) {
    urr_id.first  = true;
    urr_id.second = v;
  }
  void set(const pfcp::ur_seqn_t& v) {
    ur_seqn.first  = true;
    ur_seqn.second = v;
  }
  void set(const pfcp::usage_report_trigger_t& v) {
    usage_report_trigger.first  = true;
    usage_report_trigger.second = v;
  }
  void set(const pfcp::start_time_t& v) {
    start_time.first  = true;
    start_time.second = v;
  }
  void set(const pfcp::end_time_t& v) {
    end_time.first  = true;
    end_time.second = v;
  }
  void set(const pfcp::volume_measurement_t& v) {
    volume_measurement.first  = true;
    volume_measurement.second = v;
  }
  void set(const pfcp::duration_measurement_t& v) {
    duration_measurement.first  = true;
    duration_measurement.second = v;
  }
  void set(const pfcp::time_of_first_packet_t& v) {
    time_of_first_packet.first  = true;
    time_of_first_packet.second = v;
  }
  void set(const pfcp::time_of_last_packet_t& v) {
    time_of_last_packet.first  = true;
    time_of_last_packet.second = v;
  }
  void set(const pfcp::usage_information_t& v) {
    usage_information.first  = true;
    usage_information.second = v;
  }
  void set(const pfcp::ethernet_traffic_information& v) {
    ethernet_traffic_information.first  = true;
    ethernet_traffic_information.second = v;
  }

  bool get(pfcp::urr_id_t& v) const {
    if (urr_id.first) {
      v = urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ur_seqn_t& v) const {
    if (ur_seqn.first) {
      v = ur_seqn.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::usage_report_trigger_t& v) const {
    if (usage_report_trigger.first) {
      v = usage_report_trigger.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::start_time_t& v) const {
    if (start_time.first) {
      v = start_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::end_time_t& v) const {
    if (end_time.first) {
      v = end_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::volume_measurement_t& v) const {
    if (volume_measurement.first) {
      v = volume_measurement.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::duration_measurement_t& v) const {
    if (duration_measurement.first) {
      v = duration_measurement.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_of_first_packet_t& v) const {
    if (time_of_first_packet.first) {
      v = time_of_first_packet.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_of_last_packet_t& v) const {
    if (time_of_last_packet.first) {
      v = time_of_last_packet.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::usage_information_t& v) const {
    if (usage_information.first) {
      v = usage_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ethernet_traffic_information& v) const {
    if (ethernet_traffic_information.first) {
      v = ethernet_traffic_information.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.8.2-1: Downlink Data Report IE within PFCP Session Report Request
class downlink_data_report : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::pdr_id_t> pdr_id;
  std::pair<bool, pfcp::downlink_data_service_information_t>
      downlink_data_service_information;

  downlink_data_report() : pdr_id(), downlink_data_service_information() {}

  downlink_data_report(const downlink_data_report& d)
      : pdr_id(d.pdr_id),
        downlink_data_service_information(d.downlink_data_service_information) {
  }

  // virtual ~downlink_data_report() {};
  void set(const pfcp::pdr_id_t& v) {
    pdr_id.first  = true;
    pdr_id.second = v;
  }
  void set(const pfcp::downlink_data_service_information_t& v) {
    downlink_data_service_information.first  = true;
    downlink_data_service_information.second = v;
  }

  bool get(pfcp::pdr_id_t& v) const {
    if (pdr_id.first) {
      v = pdr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::downlink_data_service_information_t& v) const {
    if (downlink_data_service_information.first) {
      v = downlink_data_service_information.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.8.3-2: Application Detection Information IE within Usage Report IE
class application_detection_information : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::application_id_t> application_id;
  std::pair<bool, pfcp::application_instance_id_t> application_instance_id;
  std::pair<bool, pfcp::flow_information_t> flow_information;

  application_detection_information()
      : application_id(), application_instance_id(), flow_information() {}

  application_detection_information(const application_detection_information& a)
      : application_id(a.application_id),
        application_instance_id(a.application_instance_id),
        flow_information(a.flow_information) {}

  // virtual ~application_detection_information() {};
  void set(const pfcp::application_id_t& v) {
    application_id.first  = true;
    application_id.second = v;
  }
  void set(const pfcp::application_instance_id_t& v) {
    application_instance_id.first  = true;
    application_instance_id.second = v;
  }
  void set(const pfcp::flow_information_t& v) {
    flow_information.first  = true;
    flow_information.second = v;
  }

  bool get(pfcp::application_id_t& v) const {
    if (application_id.first) {
      v = application_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::application_instance_id_t& v) const {
    if (application_instance_id.first) {
      v = application_instance_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::flow_information_t& v) const {
    if (flow_information.first) {
      v = flow_information.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.8.3-2A: Event Reporting IE within Usage Report IE
class event_reporting : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::event_id_t> event_id;

  event_reporting() : event_id() {}

  event_reporting(const event_reporting& e) : event_id(e.event_id) {}

  // virtual ~event_reporting() {};
  void set(const pfcp::event_id_t& v) {
    event_id.first  = true;
    event_id.second = v;
  }

  bool get(pfcp::event_id_t& v) const {
    if (event_id.first) {
      v = event_id.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.8.3-1: Usage Report IE within PFCP Session Report Request
class usage_report_within_pfcp_session_report_request
    : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::urr_id_t> urr_id;
  std::pair<bool, pfcp::ur_seqn_t> ur_seqn;
  std::pair<bool, pfcp::usage_report_trigger_t> usage_report_trigger;
  std::pair<bool, pfcp::start_time_t> start_time;
  std::pair<bool, pfcp::end_time_t> end_time;
  std::pair<bool, pfcp::volume_measurement_t> volume_measurement;
  std::pair<bool, pfcp::duration_measurement_t> duration_measurement;
  std::pair<bool, pfcp::application_detection_information>
      application_detection_information;
  std::pair<bool, pfcp::ue_ip_address_t> ue_ip_address;
  std::pair<bool, pfcp::network_instance_t> network_instance;
  std::pair<bool, pfcp::time_of_first_packet_t> time_of_first_packet;
  std::pair<bool, pfcp::time_of_last_packet_t> time_of_last_packet;
  std::pair<bool, pfcp::usage_information_t> usage_information;
  std::pair<bool, pfcp::query_urr_reference_t> query_urr_reference;
  std::pair<bool, pfcp::event_reporting> event_reporting;
  std::pair<bool, pfcp::ethernet_traffic_information>
      ethernet_traffic_information;

  usage_report_within_pfcp_session_report_request()
      : urr_id(),
        ur_seqn(),
        usage_report_trigger(),
        start_time(),
        end_time(),
        volume_measurement(),
        duration_measurement(),
        application_detection_information(),
        ue_ip_address(),
        network_instance(),
        time_of_first_packet(),
        time_of_last_packet(),
        usage_information(),
        query_urr_reference(),
        event_reporting(),
        ethernet_traffic_information() {}

  usage_report_within_pfcp_session_report_request(
      const usage_report_within_pfcp_session_report_request& u)
      : urr_id(u.urr_id),
        ur_seqn(u.ur_seqn),
        usage_report_trigger(u.usage_report_trigger),
        start_time(u.start_time),
        end_time(u.end_time),
        volume_measurement(u.volume_measurement),
        duration_measurement(u.duration_measurement),
        application_detection_information(u.application_detection_information),
        ue_ip_address(u.ue_ip_address),
        network_instance(u.network_instance),
        time_of_first_packet(u.time_of_first_packet),
        time_of_last_packet(u.time_of_last_packet),
        usage_information(u.usage_information),
        query_urr_reference(u.query_urr_reference),
        event_reporting(u.event_reporting),
        ethernet_traffic_information(u.ethernet_traffic_information) {}

  // virtual ~usage_report_within_pfcp_session_report_request() {};
  void set(const pfcp::urr_id_t& v) {
    urr_id.first  = true;
    urr_id.second = v;
  }
  void set(const pfcp::ur_seqn_t& v) {
    ur_seqn.first  = true;
    ur_seqn.second = v;
  }
  void set(const pfcp::usage_report_trigger_t& v) {
    usage_report_trigger.first  = true;
    usage_report_trigger.second = v;
  }
  void set(const pfcp::start_time_t& v) {
    start_time.first  = true;
    start_time.second = v;
  }
  void set(const pfcp::end_time_t& v) {
    end_time.first  = true;
    end_time.second = v;
  }
  void set(const pfcp::volume_measurement_t& v) {
    volume_measurement.first  = true;
    volume_measurement.second = v;
  }
  void set(const pfcp::duration_measurement_t& v) {
    duration_measurement.first  = true;
    duration_measurement.second = v;
  }
  void set(const pfcp::application_detection_information& v) {
    application_detection_information.first  = true;
    application_detection_information.second = v;
  }
  void set(const pfcp::ue_ip_address_t& v) {
    ue_ip_address.first  = true;
    ue_ip_address.second = v;
  }
  void set(const pfcp::network_instance_t& v) {
    network_instance.first  = true;
    network_instance.second = v;
  }
  void set(const pfcp::time_of_first_packet_t& v) {
    time_of_first_packet.first  = true;
    time_of_first_packet.second = v;
  }
  void set(const pfcp::time_of_last_packet_t& v) {
    time_of_last_packet.first  = true;
    time_of_last_packet.second = v;
  }
  void set(const pfcp::usage_information_t& v) {
    usage_information.first  = true;
    usage_information.second = v;
  }
  void set(const pfcp::query_urr_reference_t& v) {
    query_urr_reference.first  = true;
    query_urr_reference.second = v;
  }
  void set(const pfcp::event_reporting& v) {
    event_reporting.first  = true;
    event_reporting.second = v;
  }
  void set(const pfcp::ethernet_traffic_information& v) {
    ethernet_traffic_information.first  = true;
    ethernet_traffic_information.second = v;
  }

  bool get(pfcp::urr_id_t& v) const {
    if (urr_id.first) {
      v = urr_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ur_seqn_t& v) const {
    if (ur_seqn.first) {
      v = ur_seqn.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::usage_report_trigger_t& v) const {
    if (usage_report_trigger.first) {
      v = usage_report_trigger.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::start_time_t& v) const {
    if (start_time.first) {
      v = start_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::end_time_t& v) const {
    if (end_time.first) {
      v = end_time.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::volume_measurement_t& v) const {
    if (volume_measurement.first) {
      v = volume_measurement.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::duration_measurement_t& v) const {
    if (duration_measurement.first) {
      v = duration_measurement.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::application_detection_information& v) const {
    if (application_detection_information.first) {
      v = application_detection_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ue_ip_address_t& v) const {
    if (ue_ip_address.first) {
      v = ue_ip_address.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::network_instance_t& v) const {
    if (network_instance.first) {
      v = network_instance.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_of_first_packet_t& v) const {
    if (time_of_first_packet.first) {
      v = time_of_first_packet.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::time_of_last_packet_t& v) const {
    if (time_of_last_packet.first) {
      v = time_of_last_packet.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::usage_information_t& v) const {
    if (usage_information.first) {
      v = usage_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::query_urr_reference_t& v) const {
    if (query_urr_reference.first) {
      v = query_urr_reference.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::event_reporting& v) const {
    if (event_reporting.first) {
      v = event_reporting.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::ethernet_traffic_information& v) const {
    if (ethernet_traffic_information.first) {
      v = ethernet_traffic_information.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.8.4-1: Error Indication Report IE within PFCP Session Report
// Request
class error_indication_report : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::fteid_t> remote_fteid;

  error_indication_report() : remote_fteid() {}

  error_indication_report(const error_indication_report& e)
      : remote_fteid(e.remote_fteid) {}

  // virtual ~error_indication_report() {};
  void set(const pfcp::fteid_t& v) {
    remote_fteid.first  = true;
    remote_fteid.second = v;
  }

  bool get(pfcp::fteid_t& v) const {
    if (remote_fteid.first) {
      v = remote_fteid.second;
      return true;
    }
    return false;
  }
};

//------------------------------------------------------------------------------
// Table 7.5.9.2-1: Update BAR IE in PFCP Session Report Response
class update_bar_within_pfcp_session_report_response
    : public pfcp::pfcp_ies_container {
 public:
  std::pair<bool, pfcp::bar_id_t> bar_id;
  std::pair<bool, pfcp::downlink_data_notification_delay_t>
      downlink_data_notification_delay;
  std::pair<bool, pfcp::dl_buffering_duration_t> dl_buffering_duration;
  std::pair<bool, pfcp::dl_buffering_suggested_packet_count_t>
      dl_buffering_buffering_suggested_packet_count;
  std::pair<bool, pfcp::suggested_buffering_packets_count_t>
      suggested_buffering_packets_count;

  update_bar_within_pfcp_session_report_response()
      : bar_id(),
        downlink_data_notification_delay(),
        dl_buffering_duration(),
        dl_buffering_buffering_suggested_packet_count(),
        suggested_buffering_packets_count() {}

  update_bar_within_pfcp_session_report_response(
      const update_bar_within_pfcp_session_report_response& u)
      : bar_id(u.bar_id),
        downlink_data_notification_delay(u.downlink_data_notification_delay),
        dl_buffering_duration(u.dl_buffering_duration),
        dl_buffering_buffering_suggested_packet_count(
            u.dl_buffering_buffering_suggested_packet_count),
        suggested_buffering_packets_count(u.suggested_buffering_packets_count) {
  }

  // virtual ~update_bar_within_pfcp_session_report_response() {};
  void set(const pfcp::bar_id_t& v) {
    bar_id.first  = true;
    bar_id.second = v;
  }
  void set(const pfcp::downlink_data_notification_delay_t& v) {
    downlink_data_notification_delay.first  = true;
    downlink_data_notification_delay.second = v;
  }
  void set(const pfcp::dl_buffering_duration_t& v) {
    dl_buffering_duration.first  = true;
    dl_buffering_duration.second = v;
  }
  void set(const pfcp::dl_buffering_suggested_packet_count_t& v) {
    dl_buffering_buffering_suggested_packet_count.first  = true;
    dl_buffering_buffering_suggested_packet_count.second = v;
  }
  void set(const pfcp::suggested_buffering_packets_count_t& v) {
    suggested_buffering_packets_count.first  = true;
    suggested_buffering_packets_count.second = v;
  }

  bool get(pfcp::bar_id_t& v) const {
    if (bar_id.first) {
      v = bar_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::downlink_data_notification_delay_t& v) const {
    if (downlink_data_notification_delay.first) {
      v = downlink_data_notification_delay.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::dl_buffering_duration_t& v) const {
    if (dl_buffering_duration.first) {
      v = dl_buffering_duration.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::dl_buffering_suggested_packet_count_t& v) const {
    if (dl_buffering_buffering_suggested_packet_count.first) {
      v = dl_buffering_buffering_suggested_packet_count.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::suggested_buffering_packets_count_t& v) const {
    if (suggested_buffering_packets_count.first) {
      v = suggested_buffering_packets_count.second;
      return true;
    }
    return false;
  }
};
}  // namespace pfcp

namespace pfcp {
//------------------------------------------------------------------------------
class pfcp_heartbeat_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_HEARTBEAT_REQUEST;

  std::pair<bool, pfcp::recovery_time_stamp_t>
      recovery_time_stamp;  // This IE shall contain the time stamp when the
                            // node was started

  pfcp_heartbeat_request() : recovery_time_stamp() {}

  pfcp_heartbeat_request(const pfcp_heartbeat_request& i) {
    recovery_time_stamp = i.recovery_time_stamp;
  }
  const char* get_msg_name() const { return "HEARTBEAT_REQUEST"; };

  bool get(pfcp::recovery_time_stamp_t& v) const {
    if (recovery_time_stamp.first) {
      v = recovery_time_stamp.second;
      return true;
    }
    return false;
  }
  void set(const pfcp::recovery_time_stamp_t& v) {
    recovery_time_stamp.first  = true;
    recovery_time_stamp.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_heartbeat_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_HEARTBEAT_RESPONSE;

  std::pair<bool, pfcp::recovery_time_stamp_t>
      recovery_time_stamp;  // This IE shall contain the time stamp when the
                            // node was started

  pfcp_heartbeat_response() : recovery_time_stamp() {}

  pfcp_heartbeat_response(const pfcp_heartbeat_response& i) {
    recovery_time_stamp = i.recovery_time_stamp;
  }
  const char* get_msg_name() const { return "HEARTBEAT_RESPONSE"; };

  bool get(pfcp::recovery_time_stamp_t& v) const {
    if (recovery_time_stamp.first) {
      v = recovery_time_stamp.second;
      return true;
    }
    return false;
  }
  void set(const pfcp::recovery_time_stamp_t& v) {
    recovery_time_stamp.first  = true;
    recovery_time_stamp.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_pfd_management_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_PFCP_PFD_MANAGEMENT_REQUEST;

  std::pair<bool, pfcp::application_ids_pfds> application_ids_pfds;

  pfcp_pfd_management_request() : application_ids_pfds() {}

  pfcp_pfd_management_request(const pfcp_pfd_management_request& i) {
    application_ids_pfds = i.application_ids_pfds;
  }
  const char* get_msg_name() const { return "PFCP_PFD_MANAGEMENT_REQUEST"; };

  bool get(pfcp::application_ids_pfds& v) const {
    if (application_ids_pfds.first) {
      v = application_ids_pfds.second;
      return true;
    }
    return false;
  }
  void set(const pfcp::application_ids_pfds& v) {
    application_ids_pfds.first  = true;
    application_ids_pfds.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_pfd_management_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_PFCP_PFD_MANAGEMENT_RESPONSE;

  std::pair<bool, pfcp::cause_t> cause;
  std::pair<bool, pfcp::offending_ie_t> offending_ie;

  pfcp_pfd_management_response() : cause(), offending_ie() {}

  pfcp_pfd_management_response(const pfcp_pfd_management_response& i) {
    cause        = i.cause;
    offending_ie = i.offending_ie;
  }
  const char* get_msg_name() const { return "PFCP_PFD_MANAGEMENT_RESPONSE"; };

  bool get(pfcp::cause_t& v) const {
    if (cause.first) {
      v = cause.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::offending_ie_t& v) const {
    if (offending_ie.first) {
      v = offending_ie.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::cause_t& v) {
    cause.first  = true;
    cause.second = v;
  }
  void set(const pfcp::offending_ie_t& v) {
    offending_ie.first  = true;
    offending_ie.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_association_setup_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_ASSOCIATION_SETUP_REQUEST;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, pfcp::recovery_time_stamp_t> recovery_time_stamp;
  std::pair<bool, pfcp::up_function_features_s> up_function_features;
  std::pair<bool, pfcp::cp_function_features_t> cp_function_features;
  std::pair<bool, pfcp::user_plane_ip_resource_information_t>
      user_plane_ip_resource_information;

  pfcp_association_setup_request()
      : node_id(),
        recovery_time_stamp(),
        up_function_features(),
        cp_function_features(),
        user_plane_ip_resource_information() {}

  pfcp_association_setup_request(const pfcp_association_setup_request& i) {
    node_id                            = i.node_id;
    recovery_time_stamp                = i.recovery_time_stamp;
    up_function_features               = i.up_function_features;
    cp_function_features               = i.cp_function_features;
    user_plane_ip_resource_information = i.user_plane_ip_resource_information;
  }
  const char* get_msg_name() const { return "PFCP_ASSOCIATION_SETUP_REQUEST"; };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::recovery_time_stamp_t& v) const {
    if (recovery_time_stamp.first) {
      v = recovery_time_stamp.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::up_function_features_s& v) const {
    if (up_function_features.first) {
      v = up_function_features.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::cp_function_features_t& v) const {
    if (cp_function_features.first) {
      v = cp_function_features.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::user_plane_ip_resource_information_t& v) const {
    if (user_plane_ip_resource_information.first) {
      v = user_plane_ip_resource_information.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set(const pfcp::recovery_time_stamp_t& v) {
    recovery_time_stamp.first  = true;
    recovery_time_stamp.second = v;
  }
  void set(const pfcp::up_function_features_s& v) {
    up_function_features.first  = true;
    up_function_features.second = v;
  }
  void set(const pfcp::cp_function_features_t& v) {
    cp_function_features.first  = true;
    cp_function_features.second = v;
  }
  void set(const pfcp::user_plane_ip_resource_information_t& v) {
    user_plane_ip_resource_information.first  = true;
    user_plane_ip_resource_information.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_association_setup_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_ASSOCIATION_SETUP_RESPONSE;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, pfcp::cause_t> cause;
  std::pair<bool, pfcp::recovery_time_stamp_t> recovery_time_stamp;
  std::pair<bool, pfcp::up_function_features_s> up_function_features;
  std::pair<bool, pfcp::cp_function_features_t> cp_function_features;
  std::pair<bool, pfcp::user_plane_ip_resource_information_t>
      user_plane_ip_resource_information;

  pfcp_association_setup_response()
      : node_id(),
        cause(),
        recovery_time_stamp(),
        up_function_features(),
        cp_function_features(),
        user_plane_ip_resource_information() {}

  pfcp_association_setup_response(const pfcp_association_setup_response& i) {
    node_id                            = i.node_id;
    cause                              = i.cause;
    recovery_time_stamp                = i.recovery_time_stamp;
    up_function_features               = i.up_function_features;
    cp_function_features               = i.cp_function_features;
    user_plane_ip_resource_information = i.user_plane_ip_resource_information;
  }
  const char* get_msg_name() const {
    return "PFCP_ASSOCIATION_SETUP_RESPONSE";
  };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::cause_t& v) const {
    if (cause.first) {
      v = cause.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::recovery_time_stamp_t& v) const {
    if (recovery_time_stamp.first) {
      v = recovery_time_stamp.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::up_function_features_s& v) const {
    if (up_function_features.first) {
      v = up_function_features.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::cp_function_features_t& v) const {
    if (cp_function_features.first) {
      v = cp_function_features.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::user_plane_ip_resource_information_t& v) const {
    if (user_plane_ip_resource_information.first) {
      v = user_plane_ip_resource_information.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set(const pfcp::cause_t& v) {
    cause.first  = true;
    cause.second = v;
  }
  void set(const pfcp::recovery_time_stamp_t& v) {
    recovery_time_stamp.first  = true;
    recovery_time_stamp.second = v;
  }
  void set(const pfcp::up_function_features_s& v) {
    up_function_features.first  = true;
    up_function_features.second = v;
  }
  void set(const pfcp::cp_function_features_t& v) {
    cp_function_features.first  = true;
    cp_function_features.second = v;
  }
  void set(const pfcp::user_plane_ip_resource_information_t& v) {
    user_plane_ip_resource_information.first  = true;
    user_plane_ip_resource_information.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_association_update_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_ASSOCIATION_UPDATE_REQUEST;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, pfcp::up_function_features_s> up_function_features;
  std::pair<bool, pfcp::cp_function_features_t> cp_function_features;
  std::pair<bool, pfcp::pfcp_association_release_request_t>
      pfcp_association_release_request;
  std::pair<bool, pfcp::graceful_release_period_t> graceful_release_period;
  std::pair<bool, pfcp::user_plane_ip_resource_information_t>
      user_plane_ip_resource_information;

  pfcp_association_update_request()
      : node_id(),
        up_function_features(),
        cp_function_features(),
        pfcp_association_release_request(),
        graceful_release_period(),
        user_plane_ip_resource_information() {}

  pfcp_association_update_request(const pfcp_association_update_request& i) {
    node_id                            = i.node_id;
    up_function_features               = i.up_function_features;
    cp_function_features               = i.cp_function_features;
    pfcp_association_release_request   = i.pfcp_association_release_request;
    graceful_release_period            = i.graceful_release_period;
    user_plane_ip_resource_information = i.user_plane_ip_resource_information;
  }
  const char* get_msg_name() const {
    return "PFCP_ASSOCIATION_UPDATE_REQUEST";
  };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::up_function_features_s& v) const {
    if (up_function_features.first) {
      v = up_function_features.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::cp_function_features_t& v) const {
    if (cp_function_features.first) {
      v = cp_function_features.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::pfcp_association_release_request_t& v) const {
    if (pfcp_association_release_request.first) {
      v = pfcp_association_release_request.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::graceful_release_period_t& v) const {
    if (graceful_release_period.first) {
      v = graceful_release_period.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::user_plane_ip_resource_information_t& v) const {
    if (user_plane_ip_resource_information.first) {
      v = user_plane_ip_resource_information.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set(const pfcp::up_function_features_s& v) {
    up_function_features.first  = true;
    up_function_features.second = v;
  }
  void set(const pfcp::cp_function_features_t& v) {
    cp_function_features.first  = true;
    cp_function_features.second = v;
  }
  void set(const pfcp::pfcp_association_release_request_t& v) {
    pfcp_association_release_request.first  = true;
    pfcp_association_release_request.second = v;
  }
  void set(const pfcp::graceful_release_period_t& v) {
    graceful_release_period.first  = true;
    graceful_release_period.second = v;
  }
  void set(const pfcp::user_plane_ip_resource_information_t& v) {
    user_plane_ip_resource_information.first  = true;
    user_plane_ip_resource_information.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_association_update_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_ASSOCIATION_UPDATE_RESPONSE;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, pfcp::cause_t> cause;
  std::pair<bool, pfcp::up_function_features_s> up_function_features;
  std::pair<bool, pfcp::cp_function_features_t> cp_function_features;

  pfcp_association_update_response()
      : node_id(), cause(), up_function_features(), cp_function_features() {}

  pfcp_association_update_response(const pfcp_association_update_response& i) {
    node_id              = i.node_id;
    cause                = i.cause;
    up_function_features = i.up_function_features;
    cp_function_features = i.cp_function_features;
  }
  const char* get_msg_name() const {
    return "PFCP_ASSOCIATION_UPDATE_RESPONSE";
  };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::cause_t& v) const {
    if (cause.first) {
      v = cause.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::up_function_features_s& v) const {
    if (up_function_features.first) {
      v = up_function_features.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::cp_function_features_t& v) const {
    if (cp_function_features.first) {
      v = cp_function_features.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set(const pfcp::cause_t& v) {
    cause.first  = true;
    cause.second = v;
  }
  void set(const pfcp::up_function_features_s& v) {
    up_function_features.first  = true;
    up_function_features.second = v;
  }
  void set(const pfcp::cp_function_features_t& v) {
    cp_function_features.first  = true;
    cp_function_features.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_association_release_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_ASSOCIATION_RELEASE_REQUEST;

  std::pair<bool, pfcp::node_id_t> node_id;

  pfcp_association_release_request() : node_id() {}

  pfcp_association_release_request(const pfcp_association_release_request& i) {
    node_id = i.node_id;
  }
  const char* get_msg_name() const {
    return "PFCP_ASSOCIATION_RELEASE_REQUEST";
  };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_association_release_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_ASSOCIATION_RELEASE_RESPONSE;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, pfcp::cause_t> cause;

  pfcp_association_release_response() : node_id(), cause() {}

  pfcp_association_release_response(
      const pfcp_association_release_response& i) {
    node_id = i.node_id;
    cause   = i.cause;
  }
  const char* get_msg_name() const {
    return "PFCP_ASSOCIATION_RELEASE_RESPONSE";
  };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::cause_t& v) const {
    if (cause.first) {
      v = cause.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set(const pfcp::cause_t& v) {
    cause.first  = true;
    cause.second = v;
  }
};
//------------------------------------------------------------------------------
class pfcp_version_not_supported_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_VERSION_NOT_SUPPORTED_RESPONSE;

  pfcp_version_not_supported_response() {}

  pfcp_version_not_supported_response(
      const pfcp_version_not_supported_response& i) {}
  const char* get_msg_name() const {
    return "PFCP_VERSION_NOT_SUPPORTED_RESPONSE";
  };
};

//------------------------------------------------------------------------------
class pfcp_node_report_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_NODE_REPORT_REQUEST;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, pfcp::node_report_type_t> node_report_type;
  std::pair<bool, pfcp::user_plane_path_failure_report>
      user_plane_path_failure_report;

  pfcp_node_report_request()
      : node_id(), node_report_type(), user_plane_path_failure_report() {}

  pfcp_node_report_request(const pfcp_node_report_request& i) {
    node_id                        = i.node_id;
    node_report_type               = i.node_report_type;
    user_plane_path_failure_report = i.user_plane_path_failure_report;
  }
  const char* get_msg_name() const { return "PFCP_NODE_REPORT_REQUEST"; };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::node_report_type_t& v) const {
    if (node_report_type.first) {
      v = node_report_type.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::user_plane_path_failure_report& v) const {
    if (user_plane_path_failure_report.first) {
      v = user_plane_path_failure_report.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set(const pfcp::node_report_type_t& v) {
    node_report_type.first  = true;
    node_report_type.second = v;
  }
  void set(const pfcp::user_plane_path_failure_report& v) {
    user_plane_path_failure_report.first  = true;
    user_plane_path_failure_report.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_node_report_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_NODE_REPORT_RESPONSE;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, pfcp::cause_t> cause;
  std::pair<bool, pfcp::offending_ie_t> offending_ie;

  pfcp_node_report_response() : node_id(), cause(), offending_ie() {}

  pfcp_node_report_response(const pfcp_node_report_response& i) {
    node_id      = i.node_id;
    cause        = i.cause;
    offending_ie = i.offending_ie;
  }
  const char* get_msg_name() const { return "PFCP_NODE_REPORT_RESPONSE"; };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::cause_t& v) const {
    if (cause.first) {
      v = cause.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::offending_ie_t& v) const {
    if (offending_ie.first) {
      v = offending_ie.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set(const pfcp::cause_t& v) {
    cause.first  = true;
    cause.second = v;
  }
  void set(const pfcp::offending_ie_t& v) {
    offending_ie.first  = true;
    offending_ie.second = v;
  }
};
//------------------------------------------------------------------------------
class pfcp_session_set_deletion_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_SESSION_SET_DELETION_REQUEST;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, fq_csid_t> sgw_c_fq_csid;
  std::pair<bool, fq_csid_t> pgw_c_fq_csid;
  std::pair<bool, fq_csid_t> sgw_u_fq_csid;
  std::pair<bool, fq_csid_t> pgw_u_fq_csid;
  std::pair<bool, fq_csid_t> twan_fq_csid;
  std::pair<bool, fq_csid_t> epdg_fq_csid;
  std::pair<bool, fq_csid_t> mme_fq_csid;

  pfcp_session_set_deletion_request()
      : node_id(),
        sgw_c_fq_csid(),
        pgw_c_fq_csid(),
        sgw_u_fq_csid(),
        pgw_u_fq_csid(),
        twan_fq_csid(),
        epdg_fq_csid(),
        mme_fq_csid() {}

  pfcp_session_set_deletion_request(
      const pfcp_session_set_deletion_request& i) {
    node_id       = i.node_id;
    sgw_c_fq_csid = i.sgw_c_fq_csid;
    pgw_c_fq_csid = i.pgw_c_fq_csid;
    sgw_u_fq_csid = i.sgw_u_fq_csid;
    pgw_u_fq_csid = i.pgw_u_fq_csid;
    twan_fq_csid  = i.twan_fq_csid;
    epdg_fq_csid  = i.epdg_fq_csid;
    mme_fq_csid   = i.mme_fq_csid;
  }
  const char* get_msg_name() const {
    return "PFCP_SESSION_SET_DELETION_REQUEST";
  };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get_sgw_c_fq_csid(fq_csid_t& v) const {
    if (sgw_c_fq_csid.first) {
      v = sgw_c_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_pgw_c_fq_csid(fq_csid_t& v) const {
    if (pgw_c_fq_csid.first) {
      v = pgw_c_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_sgw_u_fq_csid(fq_csid_t& v) const {
    if (sgw_u_fq_csid.first) {
      v = sgw_u_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_pgw_u_fq_csid(fq_csid_t& v) const {
    if (pgw_u_fq_csid.first) {
      v = pgw_u_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_twan_fq_csid(fq_csid_t& v) const {
    if (twan_fq_csid.first) {
      v = twan_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_epdg_fq_csid(fq_csid_t& v) const {
    if (epdg_fq_csid.first) {
      v = epdg_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_mme_fq_csid(fq_csid_t& v) const {
    if (mme_fq_csid.first) {
      v = mme_fq_csid.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set_sgw_c_fq_csid(const fq_csid_t& v) {
    sgw_c_fq_csid.first  = true;
    sgw_c_fq_csid.second = v;
  }
  void set_pgw_c_fq_csid(const fq_csid_t& v) {
    pgw_c_fq_csid.first  = true;
    pgw_c_fq_csid.second = v;
  }
  void set_sgw_u_fq_csid(const fq_csid_t& v) {
    sgw_u_fq_csid.first  = true;
    sgw_u_fq_csid.second = v;
  }
  void set_pgw_u_fq_csid(const fq_csid_t& v) {
    pgw_u_fq_csid.first  = true;
    pgw_u_fq_csid.second = v;
  }
  void set_twan_fq_csid(const fq_csid_t& v) {
    twan_fq_csid.first  = true;
    twan_fq_csid.second = v;
  }
  void set_epdg_fq_csid(const fq_csid_t& v) {
    epdg_fq_csid.first  = true;
    epdg_fq_csid.second = v;
  }
  void set_mme_fq_csid(const fq_csid_t& v) {
    mme_fq_csid.first  = true;
    mme_fq_csid.second = v;
  }
};
//------------------------------------------------------------------------------
class pfcp_session_set_deletion_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_SESSION_SET_DELETION_RESPONSE;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, pfcp::cause_t> cause;
  std::pair<bool, pfcp::offending_ie_t> offending_ie;

  pfcp_session_set_deletion_response() : node_id(), cause(), offending_ie() {}

  pfcp_session_set_deletion_response(
      const pfcp_session_set_deletion_response& i) {
    node_id      = i.node_id;
    cause        = i.cause;
    offending_ie = i.offending_ie;
  }
  const char* get_msg_name() const {
    return "PFCP_SESSION_SET_DELETION_RESPONSE";
  };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::cause_t& v) const {
    if (cause.first) {
      v = cause.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::offending_ie_t& v) const {
    if (offending_ie.first) {
      v = offending_ie.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set(const pfcp::cause_t& v) {
    cause.first  = true;
    cause.second = v;
  }
  void set(const pfcp::offending_ie_t& v) {
    offending_ie.first  = true;
    offending_ie.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_session_establishment_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_SESSION_ESTABLISHMENT_REQUEST;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, pfcp::fseid_t> cp_fseid;
  std::vector<pfcp::create_pdr> create_pdrs;
  std::vector<pfcp::create_far> create_fars;
  std::vector<pfcp::create_urr> create_urrs;
  std::vector<pfcp::create_qer> create_qers;
  std::pair<bool, pfcp::create_bar> create_bar;
  std::pair<bool, pfcp::create_traffic_endpoint> create_traffic_endpoint;
  std::pair<bool, pfcp::pdn_type_t> pdn_type;
  std::pair<bool, fq_csid_t> sgw_c_fq_csid;
  std::pair<bool, fq_csid_t> mme_fq_csid;
  std::pair<bool, fq_csid_t> pgw_c_fq_csid;
  std::pair<bool, fq_csid_t> epdg_fq_csid;
  std::pair<bool, fq_csid_t> twan_fq_csid;
  std::pair<bool, pfcp::user_plane_inactivity_timer_t>
      user_plane_inactivity_timer;
  std::pair<bool, pfcp::user_id_t> user_id;
  std::pair<bool, pfcp::trace_information_t> trace_information;

  pfcp_session_establishment_request()
      : node_id(),
        cp_fseid(),
        create_pdrs(),
        create_fars(),
        create_urrs(),
        create_qers(),
        create_bar(),
        create_traffic_endpoint(),
        pdn_type(),
        sgw_c_fq_csid(),
        mme_fq_csid(),
        pgw_c_fq_csid(),
        epdg_fq_csid(),
        twan_fq_csid(),
        user_plane_inactivity_timer(),
        user_id(),
        trace_information() {}

  pfcp_session_establishment_request(
      const pfcp_session_establishment_request& i) {
    node_id                     = i.node_id;
    cp_fseid                    = i.cp_fseid;
    create_pdrs                 = i.create_pdrs;
    create_fars                 = i.create_fars;
    create_urrs                 = i.create_urrs;
    create_qers                 = i.create_qers;
    create_bar                  = i.create_bar;
    create_traffic_endpoint     = i.create_traffic_endpoint;
    pdn_type                    = i.pdn_type;
    sgw_c_fq_csid               = i.sgw_c_fq_csid;
    mme_fq_csid                 = i.mme_fq_csid;
    pgw_c_fq_csid               = i.pgw_c_fq_csid;
    epdg_fq_csid                = i.epdg_fq_csid;
    twan_fq_csid                = i.twan_fq_csid;
    user_plane_inactivity_timer = i.user_plane_inactivity_timer;
    user_id                     = i.user_id;
    trace_information           = i.trace_information;
  }
  const char* get_msg_name() const {
    return "PFCP_SESSION_ESTABLISHMENT_REQUEST";
  };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::fseid_t& v) const {
    if (cp_fseid.first) {
      v = cp_fseid.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::create_bar& v) const {
    if (create_bar.first) {
      v = create_bar.second;
      return true;
    }
    return false;
  }
  bool get_sgw_c_fq_csid(fq_csid_t& v) const {
    if (sgw_c_fq_csid.first) {
      v = sgw_c_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_mme_fq_csid(fq_csid_t& v) const {
    if (mme_fq_csid.first) {
      v = mme_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_pgw_c_fq_csid(fq_csid_t& v) const {
    if (pgw_c_fq_csid.first) {
      v = pgw_c_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_epdg_fq_csid(fq_csid_t& v) const {
    if (epdg_fq_csid.first) {
      v = epdg_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_twan_fq_csid(fq_csid_t& v) const {
    if (twan_fq_csid.first) {
      v = twan_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::user_plane_inactivity_timer_t& v) const {
    if (user_plane_inactivity_timer.first) {
      v = user_plane_inactivity_timer.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::user_id_t& v) const {
    if (user_id.first) {
      v = user_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::trace_information_t& v) const {
    if (trace_information.first) {
      v = trace_information.second;
      return true;
    }
    return false;
  }
  bool get(const pfcp::far_id_t& fid, pfcp::create_far& create_far) const {
    for (auto it : create_fars) {
      pfcp::far_id_t far_id = {};
      if ((it.get(far_id)) && (fid.far_id == far_id.far_id)) {
        create_far = it;
        return true;
      }
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set(const pfcp::fseid_t& v) {
    cp_fseid.first  = true;
    cp_fseid.second = v;
  }
  void set(const pfcp::create_pdr& v) { create_pdrs.push_back(v); }
  void set(const pfcp::create_far& v) { create_fars.push_back(v); }
  void set(const pfcp::create_urr& v) { create_urrs.push_back(v); }
  void set(const pfcp::create_qer& v) { create_qers.push_back(v); }
  void set(const pfcp::create_bar& v) {
    create_bar.first  = true;
    create_bar.second = v;
  }
  void set_sgw_c_fq_csid(const fq_csid_t& v) {
    sgw_c_fq_csid.first  = true;
    sgw_c_fq_csid.second = v;
  }
  void set_mme_fq_csid(const fq_csid_t& v) {
    mme_fq_csid.first  = true;
    mme_fq_csid.second = v;
  }
  void set_pgw_c_fq_csid(const fq_csid_t& v) {
    pgw_c_fq_csid.first  = true;
    pgw_c_fq_csid.second = v;
  }
  void set_epdg_fq_csid(const fq_csid_t& v) {
    epdg_fq_csid.first  = true;
    epdg_fq_csid.second = v;
  }
  void set_twan_fq_csid(const fq_csid_t& v) {
    twan_fq_csid.first  = true;
    twan_fq_csid.second = v;
  }
  void set(const pfcp::user_plane_inactivity_timer_t& v) {
    user_plane_inactivity_timer.first  = true;
    user_plane_inactivity_timer.second = v;
  }
  void set(const pfcp::user_id_t& v) {
    user_id.first  = true;
    user_id.second = v;
  }
  void set(const pfcp::trace_information_t& v) {
    trace_information.first  = true;
    trace_information.second = v;
  }
};
//------------------------------------------------------------------------------
class pfcp_session_establishment_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_SESSION_ESTABLISHMENT_RESPONSE;

  std::pair<bool, pfcp::node_id_t> node_id;
  std::pair<bool, pfcp::cause_t> cause;
  std::pair<bool, pfcp::offending_ie_t> offending_ie;
  std::pair<bool, pfcp::fseid_t> up_fseid;
  std::vector<pfcp::created_pdr> created_pdrs;
  std::pair<bool, pfcp::load_control_information> load_control_information;
  std::pair<bool, pfcp::overload_control_information>
      overload_control_information;
  std::pair<bool, fq_csid_t> sgw_u_fq_csid;
  std::pair<bool, fq_csid_t> pgw_u_fq_csid;
  std::pair<bool, pfcp::failed_rule_id_t> failed_rule_id;
  std::pair<bool, pfcp::created_traffic_endpoint> created_traffic_endpoint;

  pfcp_session_establishment_response()
      : node_id(),
        cause(),
        offending_ie(),
        up_fseid(),
        created_pdrs(),
        load_control_information(),
        overload_control_information(),
        sgw_u_fq_csid(),
        pgw_u_fq_csid(),
        failed_rule_id(),
        created_traffic_endpoint() {}

  pfcp_session_establishment_response(
      const pfcp_session_establishment_response& i) {
    node_id                      = i.node_id;
    cause                        = i.cause;
    offending_ie                 = i.offending_ie;
    up_fseid                     = i.up_fseid;
    created_pdrs                 = i.created_pdrs;
    load_control_information     = i.load_control_information;
    overload_control_information = i.overload_control_information;
    sgw_u_fq_csid                = i.sgw_u_fq_csid;
    pgw_u_fq_csid                = i.pgw_u_fq_csid;
    failed_rule_id               = i.failed_rule_id;
    created_traffic_endpoint     = i.created_traffic_endpoint;
  }
  const char* get_msg_name() const {
    return "PFCP_SESSION_ESTABLISHMENT_RESPONSE";
  };

  bool get(pfcp::node_id_t& v) const {
    if (node_id.first) {
      v = node_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::cause_t& v) const {
    if (cause.first) {
      v = cause.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::offending_ie_t& v) const {
    if (offending_ie.first) {
      v = offending_ie.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::fseid_t& v) const {
    if (up_fseid.first) {
      v = up_fseid.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::load_control_information& v) const {
    if (load_control_information.first) {
      v = load_control_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::overload_control_information& v) const {
    if (overload_control_information.first) {
      v = overload_control_information.second;
      return true;
    }
    return false;
  }
  bool get_sgw_u_fq_csid(fq_csid_t& v) const {
    if (sgw_u_fq_csid.first) {
      v = sgw_u_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_pgw_u_fq_csid(fq_csid_t& v) const {
    if (pgw_u_fq_csid.first) {
      v = pgw_u_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::failed_rule_id_t& v) const {
    if (failed_rule_id.first) {
      v = failed_rule_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::created_traffic_endpoint& v) const {
    if (created_traffic_endpoint.first) {
      v = created_traffic_endpoint.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::node_id_t& v) {
    node_id.first  = true;
    node_id.second = v;
  }
  void set(const pfcp::cause_t& v) {
    cause.first  = true;
    cause.second = v;
  }
  void set(const pfcp::offending_ie_t& v) {
    offending_ie.first  = true;
    offending_ie.second = v;
  }
  void set(const pfcp::fseid_t& v) {
    up_fseid.first  = true;
    up_fseid.second = v;
  }
  void set(const pfcp::created_pdr& v) { created_pdrs.push_back(v); }
  void set(const pfcp::load_control_information& v) {
    load_control_information.first  = true;
    load_control_information.second = v;
  }
  void set(const pfcp::overload_control_information& v) {
    overload_control_information.first  = true;
    overload_control_information.second = v;
  }
  void set_sgw_u_fq_csid(const fq_csid_t& v) {
    sgw_u_fq_csid.first  = true;
    sgw_u_fq_csid.second = v;
  }
  void set_pgw_u_fq_csid(const fq_csid_t& v) {
    pgw_u_fq_csid.first  = true;
    pgw_u_fq_csid.second = v;
  }
  void set(const pfcp::failed_rule_id_t& v) {
    failed_rule_id.first  = true;
    failed_rule_id.second = v;
  }
  void set(const pfcp::created_traffic_endpoint& v) {
    created_traffic_endpoint.first  = true;
    created_traffic_endpoint.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_session_modification_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_SESSION_MODIFICATION_REQUEST;

  std::pair<bool, pfcp::fseid_t> cp_fseid;
  std::vector<pfcp::remove_pdr> remove_pdrs;
  std::vector<pfcp::remove_far> remove_fars;
  std::vector<pfcp::remove_urr> remove_urrs;
  std::vector<pfcp::remove_qer> remove_qers;
  std::pair<bool, pfcp::remove_bar> remove_bar;
  std::pair<bool, pfcp::remove_traffic_endpoint> remove_traffic_endpoint;
  std::vector<pfcp::create_pdr> create_pdrs;
  std::vector<pfcp::create_far> create_fars;
  std::vector<pfcp::create_urr> create_urrs;
  std::vector<pfcp::create_qer> create_qers;
  std::pair<bool, pfcp::create_bar> create_bar;
  std::pair<bool, pfcp::create_traffic_endpoint> create_traffic_endpoint;
  std::vector<pfcp::update_pdr> update_pdrs;
  std::vector<pfcp::update_far> update_fars;
  std::vector<pfcp::update_urr> update_urrs;
  std::vector<pfcp::update_qer> update_qers;
  std::pair<bool, pfcp::update_bar_within_pfcp_session_modification_request>
      update_bar;
  std::pair<bool, pfcp::update_traffic_endpoint> update_traffic_endpoint;
  std::pair<bool, pfcp::pfcpsmreq_flags_t> pfcpsmreq_flags;
  std::pair<bool, fq_csid_t> pgw_c_fq_csid;
  std::pair<bool, fq_csid_t> sgw_c_fq_csid;
  std::pair<bool, fq_csid_t> mme_fq_csid;
  std::pair<bool, fq_csid_t> epdg_fq_csid;
  std::pair<bool, fq_csid_t> twan_fq_csid;
  std::pair<bool, pfcp::user_plane_inactivity_timer_t>
      user_plane_inactivity_timer;
  std::pair<bool, pfcp::query_urr_reference_t> query_urr_reference;
  std::pair<bool, pfcp::trace_information_t> trace_information;

  pfcp_session_modification_request()
      : cp_fseid(),
        remove_pdrs(),
        remove_fars(),
        remove_urrs(),
        remove_qers(),
        remove_bar(),
        remove_traffic_endpoint(),
        create_pdrs(),
        create_fars(),
        create_urrs(),
        create_qers(),
        create_bar(),
        create_traffic_endpoint(),
        update_pdrs(),
        update_fars(),
        update_urrs(),
        update_qers(),
        update_bar(),
        update_traffic_endpoint(),
        pfcpsmreq_flags(),
        pgw_c_fq_csid(),
        sgw_c_fq_csid(),
        mme_fq_csid(),
        epdg_fq_csid(),
        twan_fq_csid(),
        user_plane_inactivity_timer(),
        query_urr_reference(),
        trace_information() {}

  pfcp_session_modification_request(const pfcp_session_modification_request& i)
      : cp_fseid(i.cp_fseid),
        remove_pdrs(i.remove_pdrs),
        remove_fars(i.remove_fars),
        remove_urrs(i.remove_urrs),
        remove_qers(i.remove_qers),
        remove_bar(i.remove_bar),
        remove_traffic_endpoint(i.remove_traffic_endpoint),
        create_pdrs(i.create_pdrs),
        create_fars(i.create_fars),
        create_urrs(i.create_urrs),
        create_qers(i.create_qers),
        create_bar(i.create_bar),
        create_traffic_endpoint(i.create_traffic_endpoint),
        update_pdrs(i.update_pdrs),
        update_fars(i.update_fars),
        update_urrs(i.update_urrs),
        update_qers(i.update_qers),
        update_bar(i.update_bar),
        update_traffic_endpoint(i.update_traffic_endpoint),
        pfcpsmreq_flags(i.pfcpsmreq_flags),
        pgw_c_fq_csid(i.pgw_c_fq_csid),
        sgw_c_fq_csid(i.sgw_c_fq_csid),
        mme_fq_csid(i.mme_fq_csid),
        epdg_fq_csid(i.epdg_fq_csid),
        twan_fq_csid(i.twan_fq_csid),
        user_plane_inactivity_timer(i.user_plane_inactivity_timer),
        query_urr_reference(i.query_urr_reference),
        trace_information(i.trace_information) {}

  const char* get_msg_name() const {
    return "PFCP_SESSION_MODIFICATION_REQUEST";
  };

  bool get(pfcp::fseid_t& v) const {
    if (cp_fseid.first) {
      v = cp_fseid.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::remove_bar& v) const {
    if (remove_bar.first) {
      v = remove_bar.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::remove_traffic_endpoint& v) const {
    if (remove_traffic_endpoint.first) {
      v = remove_traffic_endpoint.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::create_bar& v) const {
    if (create_bar.first) {
      v = create_bar.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::create_traffic_endpoint& v) const {
    if (create_traffic_endpoint.first) {
      v = create_traffic_endpoint.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::update_bar_within_pfcp_session_modification_request& v) const {
    if (update_bar.first) {
      v = update_bar.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::update_traffic_endpoint& v) const {
    if (update_traffic_endpoint.first) {
      v = update_traffic_endpoint.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::pfcpsmreq_flags_t& v) const {
    if (pfcpsmreq_flags.first) {
      v = pfcpsmreq_flags.second;
      return true;
    }
    return false;
  }
  bool get_pgw_c_fq_csid(fq_csid_t& v) const {
    if (pgw_c_fq_csid.first) {
      v = pgw_c_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_sgw_c_fq_csid(fq_csid_t& v) const {
    if (sgw_c_fq_csid.first) {
      v = sgw_c_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_mme_fq_csid(fq_csid_t& v) const {
    if (mme_fq_csid.first) {
      v = mme_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_epdg_fq_csid(fq_csid_t& v) const {
    if (epdg_fq_csid.first) {
      v = epdg_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get_twan_fq_csid(fq_csid_t& v) const {
    if (twan_fq_csid.first) {
      v = twan_fq_csid.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::user_plane_inactivity_timer_t& v) const {
    if (user_plane_inactivity_timer.first) {
      v = user_plane_inactivity_timer.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::query_urr_reference_t& v) const {
    if (query_urr_reference.first) {
      v = query_urr_reference.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::trace_information_t& v) const {
    if (trace_information.first) {
      v = trace_information.second;
      return true;
    }
    return false;
  }
  bool get(const pfcp::far_id_t& fid, pfcp::create_far& create_far) const {
    for (auto it : create_fars) {
      pfcp::far_id_t far_id = {};
      if ((it.get(far_id)) && (fid.far_id == far_id.far_id)) {
        create_far = it;
        return true;
      }
    }
    return false;
  }
  void set(const pfcp::fseid_t& v) {
    cp_fseid.first  = true;
    cp_fseid.second = v;
  }
  void set(const pfcp::remove_pdr& v) { remove_pdrs.push_back(v); }
  void set(const pfcp::remove_far& v) { remove_fars.push_back(v); }
  void set(const pfcp::remove_urr& v) { remove_urrs.push_back(v); }
  void set(const pfcp::remove_qer& v) { remove_qers.push_back(v); }
  void set(const pfcp::remove_bar& v) {
    remove_bar.first  = true;
    remove_bar.second = v;
  }
  void set(const pfcp::remove_traffic_endpoint& v) {
    remove_traffic_endpoint.first  = true;
    remove_traffic_endpoint.second = v;
  }
  void set(const pfcp::create_pdr& v) { create_pdrs.push_back(v); }
  void set(const pfcp::create_far& v) { create_fars.push_back(v); }
  void set(const pfcp::create_urr& v) { create_urrs.push_back(v); }
  void set(const pfcp::create_qer& v) { create_qers.push_back(v); }
  void set(const pfcp::create_bar& v) {
    create_bar.first  = true;
    create_bar.second = v;
  }
  void set(const pfcp::create_traffic_endpoint& v) {
    create_traffic_endpoint.first  = true;
    create_traffic_endpoint.second = v;
  }
  void set(const pfcp::update_pdr& v) { update_pdrs.push_back(v); }
  void set(const pfcp::update_far& v) { update_fars.push_back(v); }
  void set(const pfcp::update_urr& v) { update_urrs.push_back(v); }
  void set(const pfcp::update_qer& v) { update_qers.push_back(v); }
  void set(const pfcp::update_bar_within_pfcp_session_modification_request& v) {
    update_bar.first  = true;
    update_bar.second = v;
  }
  void set(const pfcp::update_traffic_endpoint& v) {
    update_traffic_endpoint.first  = true;
    update_traffic_endpoint.second = v;
  }
  void set(const pfcp::pfcpsmreq_flags_t& v) {
    pfcpsmreq_flags.first  = true;
    pfcpsmreq_flags.second = v;
  }
  void set_pgw_c_fq_csid(const fq_csid_t& v) {
    pgw_c_fq_csid.first  = true;
    pgw_c_fq_csid.second = v;
  }
  void set_sgw_c_fq_csid(const fq_csid_t& v) {
    sgw_c_fq_csid.first  = true;
    sgw_c_fq_csid.second = v;
  }
  void set_mme_fq_csid(const fq_csid_t& v) {
    mme_fq_csid.first  = true;
    mme_fq_csid.second = v;
  }
  void set_epdg_fq_csid(const fq_csid_t& v) {
    epdg_fq_csid.first  = true;
    epdg_fq_csid.second = v;
  }
  void set_twan_fq_csid(const fq_csid_t& v) {
    twan_fq_csid.first  = true;
    twan_fq_csid.second = v;
  }
  void set(const pfcp::user_plane_inactivity_timer_t& v) {
    user_plane_inactivity_timer.first  = true;
    user_plane_inactivity_timer.second = v;
  }
  void set(const pfcp::query_urr_reference_t& v) {
    query_urr_reference.first  = true;
    query_urr_reference.second = v;
  }
  void set(const pfcp::trace_information_t& v) {
    trace_information.first  = true;
    trace_information.second = v;
  }
};
//------------------------------------------------------------------------------
class pfcp_session_modification_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_SESSION_MODIFICATION_RESPONSE;

  std::pair<bool, pfcp::cause_t> cause;
  std::pair<bool, pfcp::offending_ie_t> offending_ie;
  std::vector<pfcp::created_pdr> created_pdrs;
  std::pair<bool, pfcp::load_control_information> load_control_information;
  std::pair<bool, pfcp::overload_control_information>
      overload_control_information;
  std::pair<bool, pfcp::usage_report_within_pfcp_session_modification_response>
      usage_report;
  std::pair<bool, pfcp::failed_rule_id_t> failed_rule_id;
  std::pair<bool, pfcp::additional_usage_reports_information_t>
      additional_usage_reports_information;
  std::pair<bool, pfcp::created_traffic_endpoint> created_traffic_endpoint;

  pfcp_session_modification_response()
      : cause(),
        offending_ie(),
        created_pdrs(),
        load_control_information(),
        overload_control_information(),
        usage_report(),
        failed_rule_id(),
        additional_usage_reports_information(),
        created_traffic_endpoint() {}

  pfcp_session_modification_response(
      const pfcp_session_modification_response& i)
      : cause(i.cause),
        offending_ie(i.offending_ie),
        created_pdrs(i.created_pdrs),
        load_control_information(i.load_control_information),
        overload_control_information(i.overload_control_information),
        usage_report(i.usage_report),
        failed_rule_id(i.failed_rule_id),
        additional_usage_reports_information(
            i.additional_usage_reports_information),
        created_traffic_endpoint(i.created_traffic_endpoint) {}

  const char* get_msg_name() const {
    return "PFCP_SESSION_MODIFICATION_RESPONSE";
  };

  bool get(pfcp::cause_t& v) const {
    if (cause.first) {
      v = cause.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::offending_ie_t& v) const {
    if (offending_ie.first) {
      v = offending_ie.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::load_control_information& v) const {
    if (load_control_information.first) {
      v = load_control_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::overload_control_information& v) const {
    if (overload_control_information.first) {
      v = overload_control_information.second;
      return true;
    }
    return false;
  }
  bool get(
      pfcp::usage_report_within_pfcp_session_modification_response& v) const {
    if (usage_report.first) {
      v = usage_report.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::failed_rule_id_t& v) const {
    if (failed_rule_id.first) {
      v = failed_rule_id.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::additional_usage_reports_information_t& v) const {
    if (additional_usage_reports_information.first) {
      v = additional_usage_reports_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::created_traffic_endpoint& v) const {
    if (created_traffic_endpoint.first) {
      v = created_traffic_endpoint.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::cause_t& v) {
    cause.first  = true;
    cause.second = v;
  }
  void set(const pfcp::offending_ie_t& v) {
    offending_ie.first  = true;
    offending_ie.second = v;
  }
  void set(const pfcp::created_pdr& v) { created_pdrs.push_back(v); }
  void set(const pfcp::load_control_information& v) {
    load_control_information.first  = true;
    load_control_information.second = v;
  }
  void set(const pfcp::overload_control_information& v) {
    overload_control_information.first  = true;
    overload_control_information.second = v;
  }
  void set(
      const pfcp::usage_report_within_pfcp_session_modification_response& v) {
    usage_report.first  = true;
    usage_report.second = v;
  }
  void set(const pfcp::failed_rule_id_t& v) {
    failed_rule_id.first  = true;
    failed_rule_id.second = v;
  }
  void set(const pfcp::additional_usage_reports_information_t& v) {
    additional_usage_reports_information.first  = true;
    additional_usage_reports_information.second = v;
  }
  void set(const pfcp::created_traffic_endpoint& v) {
    created_traffic_endpoint.first  = true;
    created_traffic_endpoint.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_session_deletion_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_SESSION_DELETION_REQUEST;

  pfcp_session_deletion_request() {}
  pfcp_session_deletion_request(const pfcp_session_deletion_request& i) {}
  const char* get_msg_name() const { return "PFCP_SESSION_DELETION_REQUEST"; };
};
//------------------------------------------------------------------------------
class pfcp_session_deletion_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_SESSION_DELETION_RESPONSE;

  std::pair<bool, pfcp::cause_t> cause;
  std::pair<bool, pfcp::offending_ie_t> offending_ie;

  pfcp_session_deletion_response() : cause(), offending_ie() {}

  pfcp_session_deletion_response(const pfcp_session_deletion_response& i)
      : cause(i.cause), offending_ie(i.offending_ie) {}

  const char* get_msg_name() const { return "PFCP_SESSION_DELETION_RESPONSE"; };

  bool get(pfcp::cause_t& v) const {
    if (cause.first) {
      v = cause.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::offending_ie_t& v) const {
    if (offending_ie.first) {
      v = offending_ie.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::cause_t& v) {
    cause.first  = true;
    cause.second = v;
  }
  void set(const pfcp::offending_ie_t& v) {
    offending_ie.first  = true;
    offending_ie.second = v;
  }
};
//------------------------------------------------------------------------------
class pfcp_session_report_request : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_SESSION_REPORT_REQUEST;

  std::pair<bool, pfcp::report_type_t> report_type;
  std::pair<bool, pfcp::downlink_data_report> downlink_data_report;
  std::pair<bool, pfcp::usage_report_within_pfcp_session_report_request>
      usage_report;
  std::pair<bool, pfcp::error_indication_report> error_indication_report;
  std::pair<bool, pfcp::load_control_information> load_control_information;
  std::pair<bool, pfcp::overload_control_information>
      overload_control_information;
  std::pair<bool, pfcp::additional_usage_reports_information_t>
      additional_usage_reports_information;

  pfcp_session_report_request()
      : report_type(),
        downlink_data_report(),
        usage_report(),
        error_indication_report(),
        load_control_information(),
        overload_control_information(),
        additional_usage_reports_information() {}

  pfcp_session_report_request(const pfcp_session_report_request& i)
      : report_type(i.report_type),
        downlink_data_report(i.downlink_data_report),
        usage_report(i.usage_report),
        error_indication_report(i.error_indication_report),
        load_control_information(i.load_control_information),
        overload_control_information(i.overload_control_information),
        additional_usage_reports_information(
            i.additional_usage_reports_information) {}

  const char* get_msg_name() const { return "PFCP_SESSION_REPORT_REQUEST"; };

  bool get(pfcp::report_type_t& v) const {
    if (report_type.first) {
      v = report_type.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::downlink_data_report& v) const {
    if (downlink_data_report.first) {
      v = downlink_data_report.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::usage_report_within_pfcp_session_report_request& v) const {
    if (usage_report.first) {
      v = usage_report.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::error_indication_report& v) const {
    if (error_indication_report.first) {
      v = error_indication_report.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::load_control_information& v) const {
    if (load_control_information.first) {
      v = load_control_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::overload_control_information& v) const {
    if (overload_control_information.first) {
      v = overload_control_information.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::additional_usage_reports_information_t& v) const {
    if (additional_usage_reports_information.first) {
      v = additional_usage_reports_information.second;
      return true;
    }
    return false;
  }

  void set(const pfcp::report_type_t& v) {
    report_type.first  = true;
    report_type.second = v;
  }
  void set(const pfcp::downlink_data_report& v) {
    downlink_data_report.first  = true;
    downlink_data_report.second = v;
  }
  void set(const pfcp::usage_report_within_pfcp_session_report_request& v) {
    usage_report.first  = true;
    usage_report.second = v;
  }
  void set(const pfcp::error_indication_report& v) {
    error_indication_report.first  = true;
    error_indication_report.second = v;
  }
  void set(const pfcp::load_control_information& v) {
    load_control_information.first  = true;
    load_control_information.second = v;
  }
  void set(const pfcp::overload_control_information& v) {
    overload_control_information.first  = true;
    overload_control_information.second = v;
  }
  void set(const pfcp::additional_usage_reports_information_t& v) {
    additional_usage_reports_information.first  = true;
    additional_usage_reports_information.second = v;
  }
};

//------------------------------------------------------------------------------
class pfcp_session_report_response : public pfcp_ies_container {
 public:
  static const uint8_t msg_id = PFCP_SESSION_REPORT_RESPONSE;

  std::pair<bool, pfcp::cause_t> cause;
  std::pair<bool, pfcp::offending_ie_t> offending_ie;
  std::pair<bool, pfcp::update_bar_within_pfcp_session_report_response>
      update_bar;
  // TODO SPEC std::pair<bool, pfcp::sxsrrsp_flags_t>            sxsrrsp_flags;

  pfcp_session_report_response() : cause(), offending_ie(), update_bar() {
    // sxsrrsp_flags = {};
  }

  pfcp_session_report_response(const pfcp_session_report_response& i)
      : cause(i.cause), offending_ie(i.offending_ie), update_bar(i.update_bar) {
    // sxsrrsp_flags = i.sxsrrsp_flags;
  }
  const char* get_msg_name() const { return "PFCP_SESSION_REPORT_RESPONSE"; };

  bool get(pfcp::cause_t& v) const {
    if (cause.first) {
      v = cause.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::offending_ie_t& v) const {
    if (offending_ie.first) {
      v = offending_ie.second;
      return true;
    }
    return false;
  }
  bool get(pfcp::update_bar_within_pfcp_session_report_response& v) const {
    if (update_bar.first) {
      v = update_bar.second;
      return true;
    }
    return false;
  }
  // bool get(pfcp::sxsrrsp_flags_t& v) const {if (sxsrrsp_flags.first) {v =
  // sxsrrsp_flags.second;return true;}return false;}

  void set(const pfcp::cause_t& v) {
    cause.first  = true;
    cause.second = v;
  }
  void set(const pfcp::offending_ie_t& v) {
    offending_ie.first  = true;
    offending_ie.second = v;
  }
  void set(const pfcp::update_bar_within_pfcp_session_report_response& v) {
    update_bar.first  = true;
    update_bar.second = v;
  }
  // void set(const pfcp::sxsrrsp_flags_t& v) {sxsrrsp_flags.first = true;
  // sxsrrsp_flags.second = v;}
};

}  // namespace pfcp

#endif /* MSG_PFCP_HPP_INCLUDED_ */
