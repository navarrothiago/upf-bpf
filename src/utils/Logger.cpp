#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"


Logger &Logger::getInstance()
{
  static Logger sInstance;
  return sInstance;
}

Logger::~Logger() {}

Logger::Logger()
    : spdlog::logger(*spdlog::stdout_color_mt("upf_logger"))
{
  set_level(spdlog::level::debug);
}

Logger::FuncLogger::FuncLogger(const std::string& funcName)
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

  // BEGIN
  Logger::getInstance().debug("[bgn] {0}", mFunctionName);
}

Logger::FuncLogger::~FuncLogger()
{
  // END
  Logger::getInstance().debug("[end] {0}", mFunctionName);
}
