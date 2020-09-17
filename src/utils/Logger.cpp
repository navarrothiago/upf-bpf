#include "Logger.h"
#include <stdarg.h>
#include <unistd.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

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
    mpLogger->error(out);
    break;
  case LOG_INF:
    mpLogger->info(out);
    break;
  case LOG_DBG:
    mpLogger->debug(out);
    break;
  case LOG_WRN:
    mpLogger->warn(out);
    break;
  default:
    break;
  }
}

Logger::Logger()
{ 
  // TODO navarrothiago - put definition depending on the spdlog version.
  std::shared_ptr<spdlog::sinks::sink> pColorSink;
  pColorSink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
  pColorSink->set_level(spdlog::level::debug);
  mpLogger = std::make_shared<spdlog::logger>("upf_logger", pColorSink);
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
