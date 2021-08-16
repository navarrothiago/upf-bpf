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

/*! \file thread_sched.cpp
  \brief
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "thread_sched.hpp"

//------------------------------------------------------------------------------
void util::thread_sched_params::apply(
    const int task_id, _Logger& logger) const {
  if (cpu_id >= 0) {
    cpu_set_t cpuset;
    CPU_SET(cpu_id, &cpuset);
    if (int rc = pthread_setaffinity_np(
            pthread_self(), sizeof(cpu_set_t), &cpuset)) {
      logger.warn(
          "Could not set affinity to ITTI task %d, err=%d", task_id, rc);
    }
  }

  struct sched_param sparam;
  memset(&sparam, 0, sizeof(sparam));
  sparam.sched_priority = sched_priority;
  if (int rc = pthread_setschedparam(pthread_self(), sched_policy, &sparam)) {
    logger.warn(
        "Could not set schedparam to ITTI task %d, err=%d", task_id, rc);
  }
}
