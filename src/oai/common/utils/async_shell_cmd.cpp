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

/*! \file async_shell_cmd.cpp
   \brief
   \author  Lionel GAUTHIER
   \date 2017
   \email: lionel.gauthier@eurecom.fr
*/
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <inttypes.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>

#include "itti.hpp"
#include "async_shell_cmd.hpp"
#include "itti_async_shell_cmd.hpp"
#include "logger.hpp"
#include "common_defs.h"

#include <stdexcept>

using namespace util;

extern itti_mw* itti_inst;
void async_cmd_task(void*);

//------------------------------------------------------------------------------
void async_cmd_task(void* args_p) {
  const task_id_t task_id = TASK_ASYNC_SHELL_CMD;

  const thread_sched_params* const sched_params =
      (const util::thread_sched_params* const) args_p;
  sched_params->apply(task_id, Logger::async_cmd());

  itti_inst->notify_task_ready(task_id);

  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case ASYNC_SHELL_CMD:
        if (itti_async_shell_cmd* to =
                dynamic_cast<itti_async_shell_cmd*>(msg)) {
          int rc = system((const char*) to->system_command.c_str());

          if (rc) {
            Logger::async_cmd().error(
                "Failed cmd from %d: %s ", to->origin,
                (const char*) to->system_command.c_str());
            if (to->is_abort_on_error) {
              Logger::async_cmd().error(
                  "Terminate cause failed cmd %s at %s:%d",
                  to->system_command.c_str(), to->src_file.c_str(),
                  to->src_line);
              itti_inst->send_terminate_msg(to->origin);
            }
          }
        }
        break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          Logger::async_cmd().info("TIME-OUT event timer id %d", to->timer_id);
        }
        break;

      case TERMINATE:
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::async_cmd().info("Received terminate message");
          return;
        }
        break;

      case HEALTH_PING:
        break;

      default:
        Logger::sgwc_app().info("no handler for msg type %d", msg->msg_type);
    }

  } while (true);
}

//------------------------------------------------------------------------------
async_shell_cmd::async_shell_cmd(util::thread_sched_params& sched_params) {
  Logger::async_cmd().startup("Starting...");

  if (itti_inst->create_task(
          TASK_ASYNC_SHELL_CMD, async_cmd_task, &sched_params)) {
    Logger::async_cmd().error("Cannot create task TASK_ASYNC_SHELL_CMD");
    throw std::runtime_error("Cannot create task TASK_ASYNC_SHELL_CMD");
  }
  Logger::async_cmd().startup("Started");
}

//------------------------------------------------------------------------------
int async_shell_cmd::run_command(
    const task_id_t sender_itti_task, const bool is_abort_on_error,
    const char* src_file, const int src_line, const std::string& cmd_str) {
  itti_async_shell_cmd cmd(
      sender_itti_task, TASK_ASYNC_SHELL_CMD, cmd_str, is_abort_on_error,
      src_file, src_line);
  std::shared_ptr<itti_async_shell_cmd> msg =
      std::make_shared<itti_async_shell_cmd>(cmd);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::async_cmd().error(
        "Could not send ITTI message to task TASK_ASYNC_SHELL_CMD");
    return RETURNerror;
  }
  return RETURNok;
}
