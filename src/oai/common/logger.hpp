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

#ifndef __LOGGER_H
#define __LOGGER_H

#include <cstdarg>
#include <stdexcept>
#include <vector>

//#define SPDLOG_LEVEL_NAMES { "trace", "debug", "info",  "warning", "error",
//"critical", "off" };
#define SPDLOG_LEVEL_NAMES                                                     \
  {"trace", "debug", "info ", "start", "warn ", "error", "off  "};

#define SPDLOG_ENABLE_SYSLOG
#include "spdlog/spdlog.h"

class LoggerException : public std::runtime_error {
 public:
  explicit LoggerException(const char* m) : std::runtime_error(m) {}
  explicit LoggerException(const std::string& m) : std::runtime_error(m) {}
};

class _Logger {
 public:
  _Logger(
      const char* category, std::vector<spdlog::sink_ptr>& sinks,
      const char* pattern);

  void trace(const char* format, ...);
  void trace(const std::string& format, ...);
  void debug(const char* format, ...);
  void debug(const std::string& format, ...);
  void info(const char* format, ...);
  void info(const std::string& format, ...);
  void startup(const char* format, ...);
  void startup(const std::string& format, ...);
  void warn(const char* format, ...);
  void warn(const std::string& format, ...);
  void error(const char* format, ...);
  void error(const std::string& format, ...);

 private:
  _Logger();

  enum _LogType { _ltTrace, _ltDebug, _ltInfo, _ltStartup, _ltWarn, _ltError };

  void log(_LogType lt, const char* format, va_list& args);

  spdlog::logger m_log;
};

class Logger {
 public:
  static void init(
      const char* app, const bool log_stdout, const bool log_rot_file) {
    singleton()._init(app, log_stdout, log_rot_file);
  }
  static void init(
      const std::string& app, const bool log_stdout, const bool log_rot_file) {
    init(app.c_str(), log_stdout, log_rot_file);
  }

  static _Logger& async_cmd() { return *singleton().m_async_cmd; }
  static _Logger& enb_s1u() { return *singleton().m_enb_s1u; }
  static _Logger& gtpv1_u() { return *singleton().m_gtpv1_u; }
  static _Logger& gtpv2_c() { return *singleton().m_gtpv2_c; }
  // static _Logger &gx() { return *singleton().m_gx; }
  static _Logger& itti() { return *singleton().m_itti; }
  static _Logger& mme_s11() { return *singleton().m_mme_s11; }
  static _Logger& pgwc_app() { return *singleton().m_pgwc_app; }
  // static _Logger &pgwu_app() { return *singleton().m_pgwu_app; }
  static _Logger& pgwc_s5s8() { return *singleton().m_pgwc_s5s8; }
  static _Logger& pgwc_sx() { return *singleton().m_pgwc_sx; }
  // static _Logger &pgwu_sx() { return *singleton().m_pgwu_sx; }
  // static _Logger &pgw_udp() { return *singleton().m_pgw_udp; }
  static _Logger& sgwc_app() { return *singleton().m_sgwc_app; }
  // static _Logger &sgwu_app() { return *singleton().m_sgwu_app; }
  // static _Logger &sgwu_sx() { return *singleton().m_sgwu_sx; }
  static _Logger& sgwc_s11() { return *singleton().m_sgwc_s11; }
  static _Logger& sgwc_s5s8() { return *singleton().m_sgwc_s5s8; }
  static _Logger& sgwc_sx() { return *singleton().m_sgwc_sx; }
  // static _Logger &sgw_udp() { return *singleton().m_sgw_udp; }
  static _Logger& spgwu_app() { return *singleton().m_spgwu_app; }
  static _Logger& spgwu_s1u() { return *singleton().m_spgwu_s1u; }
  static _Logger& spgwu_sx() { return *singleton().m_spgwu_sx; }
  static _Logger& system() { return *singleton().m_system; }
  static _Logger& udp() { return *singleton().m_udp; }
  static _Logger& pfcp() { return *singleton().m_pfcp; }
  static _Logger& pfcp_switch() { return *singleton().m_pfcp_switch; }

 private:
  static Logger* m_singleton;
  static Logger& singleton() {
    if (!m_singleton) m_singleton = new Logger();
    return *m_singleton;
  }

  Logger() {}
  ~Logger() {}

  void _init(const char* app, const bool log_stdout, const bool log_rot_file);

  std::vector<spdlog::sink_ptr> m_sinks;

  std::string m_pattern;

  _Logger* m_async_cmd;
  _Logger* m_enb_s1u;
  _Logger* m_gtpv1_u;
  _Logger* m_gtpv2_c;
  //_Logger *m_gx;
  _Logger* m_itti;
  _Logger* m_mme_s11;
  _Logger* m_pgwc_app;
  //_Logger *m_pgwu_app;
  _Logger* m_pgwc_s5s8;
  _Logger* m_pgwc_sx;
  //_Logger *m_pgwu_sx;
  //_Logger *m_pgw_udp;
  _Logger* m_sgwc_app;
  //_Logger *m_sgwu_app;
  //_Logger *m_sgwu_sx;
  _Logger* m_sgwc_s11;
  _Logger* m_sgwc_s5s8;
  _Logger* m_sgwc_sx;
  //_Logger *m_sgw_udp;
  _Logger* m_spgwu_app;
  _Logger* m_spgwu_s1u;
  _Logger* m_spgwu_sx;
  _Logger* m_system;
  _Logger* m_udp;
  _Logger* m_pfcp;
  _Logger* m_pfcp_switch;
};

#endif  // __LOGGER_H
