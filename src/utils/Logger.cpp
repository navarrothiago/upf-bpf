#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <stdarg.h>
#include <unistd.h>

namespace Utils{
Logger &Logger::getInstance()
{
  static Logger sInstance;
  return sInstance;
}

Logger::~Logger() {}

void Logger::print(const char *pMessage, LOG_TYPE_T type, const char *pFile, int line)
{
  unsigned int threadId = (unsigned int)pthread_self();
  unsigned int processId = (unsigned int)getpid();
  std::string out;

  if(pFile != NULL) {
    if(pMessage != NULL) {
      out = fmt::format("[{}.{}] ({}, {}) {}", processId, threadId, pFile, line, pMessage);
      // Free the pointer used to store the list.
    } else {
      out = fmt::format("[{}.{}] ({}, {}))", processId, threadId, pFile, line);
    }
  } else {
    if(pMessage != NULL) {
      out = fmt::format("[{}.{}] {}", processId, threadId, pMessage);
    } else {
      out = fmt::format("[{}.{}] {}", processId, threadId);
    }
  }

  doPrint(type, out);
}

void Logger::doPrint(LOG_TYPE_T type, const std::string &out)
{
  switch(type) {
  case LOG_ERROR:
    error(out);
    break;
  case LOG_INF:
    info(out);
    break;
  case LOG_DBG:
    debug(out);
    break;
  case LOG_WRN:
    warn(out);
    break;
  default:
    break;
  }
}

Logger::Logger()
    : spdlog::logger(*spdlog::stdout_color_mt("upf_logger"))
{
  set_level(spdlog::level::debug);
}

Logger::FuncLogger::FuncLogger(const std::string &funcName)
{
  // Func name has type and return. Format to: classname::methodname().
  size_t begin = 0;
  size_t colon = funcName.find("::");
  size_t secondColon = funcName.find("::", colon + 1);
  // Check if there is more the one ':' .
  if(secondColon != std::string::npos) {
    size_t brackets = funcName.find("(", colon);
    if(brackets > secondColon) {
      // If there isnt "(" between the first and the second ':', it must use the ':'.
      colon = secondColon;
    }
  }
  begin = funcName.rfind(" ", colon) + 1;
  size_t length = funcName.rfind("(") - begin;
  if(length > MAX_METHOD_LENGTH) {
    length = MAX_METHOD_LENGTH;
  }

  memcpy(mFunctionName, funcName.data() + begin, length);
  mFunctionName[length] = '\0';

  // Begin function.
  std::string pMessageString = fmt::format("[bgn] {}", mFunctionName);
  Logger::getInstance().print(pMessageString.c_str(), LOG_DBG, NULL);
}

Logger::FuncLogger::~FuncLogger()
{
  // End function.
  std::string pMessageString = fmt::format("[end] {}", mFunctionName);
  Logger::getInstance().print(pMessageString.c_str(), LOG_DBG, NULL);
}
}