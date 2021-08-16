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

/*! \file pid_file.hpp
   \brief
   \author  Lionel GAUTHIER
   \date 2016
   \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_PID_FILE_SEEN
#define FILE_PID_FILE_SEEN
#include <string>

namespace util {

/*
 * Generate the exe absolute path using a specified base_path.
 *
 * @param base_path
 *        the root directory to use.
 *
 * @return a string for the exe absolute path.
 */
std::string get_exe_absolute_path(
    const std::string& base_path, const unsigned int instance);

bool is_pid_file_lock_success(const char* pid_file_name);

void pid_file_unlock(void);

int lockfile(int fd, int lock_type);

}  // namespace util
#endif
