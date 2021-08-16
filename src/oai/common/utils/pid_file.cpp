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

/*! \file pid_file.cpp
   \brief
   \author  Lionel GAUTHIER
   \date 2016
   \email: lionel.gauthier@eurecom.fr
*/

#include "logger.hpp"
#include "pid_file.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>

int g_fd_pid_file = -1;
__pid_t g_pid     = -1;
//------------------------------------------------------------------------------
std::string util::get_exe_absolute_path(
    const std::string& basepath, const unsigned int instance) {
#define MAX_FILE_PATH_LENGTH 255
  char pid_file_name[MAX_FILE_PATH_LENGTH + 1] = {0};
  char* exe_basename                           = NULL;
  int rv                                       = 0;
  int num_chars                                = 0;

  // get executable name
  rv = readlink("/proc/self/exe", pid_file_name, 256);
  if (-1 == rv) {
    return NULL;
  }
  pid_file_name[rv] = 0;
  exe_basename      = basename(pid_file_name);

  // Add 6 for the other 5 characters in the path + null terminator + 2 chars
  // for instance.
  num_chars = basepath.size() + strlen(exe_basename) + 6 + 2;
  if (num_chars > MAX_FILE_PATH_LENGTH) {
    num_chars = MAX_FILE_PATH_LENGTH;
  }
  snprintf(
      pid_file_name, num_chars, "%s/%s%02u.pid", basepath.c_str(), exe_basename,
      instance);
  return std::string(pid_file_name);
}

//------------------------------------------------------------------------------
int util::lockfile(int fd, int lock_type) {
  // lock on fd only, not on file on disk (do not prevent another process from
  // modifying the file)
  return lockf(fd, F_TLOCK, 0);
}

//------------------------------------------------------------------------------
bool util::is_pid_file_lock_success(const char* pid_file_name) {
  char pid_dec[64] = {0};

  g_fd_pid_file = open(
      pid_file_name, O_RDWR | O_CREAT,
      S_IRUSR | S_IWUSR | S_IRGRP |
          S_IROTH); /* Read/write by owner, read by grp, others */
  if (0 > g_fd_pid_file) {
    Logger::sgwc_app().error(
        "open filename %s failed %d:%s\n", pid_file_name, errno,
        strerror(errno));
    return false;
  }

  if (0 > util::lockfile(g_fd_pid_file, F_TLOCK)) {
    Logger::sgwc_app().error(
        "lockfile filename %s failed %d:%s\n", pid_file_name, errno,
        strerror(errno));
    if (EACCES == errno || EAGAIN == errno) {
      close(g_fd_pid_file);
    }
    return false;
  }
  // fruncate file content
  if (ftruncate(g_fd_pid_file, 0)) {
    Logger::sgwc_app().error(
        "truncate %s failed %d:%s\n", pid_file_name, errno, strerror(errno));
    close(g_fd_pid_file);
    return false;
  }
  // write PID in file
  g_pid = getpid();
  snprintf(pid_dec, 64 /* should be big enough */, "%ld", (long) g_pid);
  if ((ssize_t) -1 == write(g_fd_pid_file, pid_dec, strlen(pid_dec))) {
    Logger::sgwc_app().error(
        "write PID to filename %s failed %d:%s\n", pid_file_name, errno,
        strerror(errno));
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void util::pid_file_unlock(void) {
  util::lockfile(g_fd_pid_file, F_ULOCK);
  close(g_fd_pid_file);
  g_fd_pid_file = -1;
}
