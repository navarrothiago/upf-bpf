#ifndef __LOGDEFINES_H__
#define __LOGDEFINES_H__

#include <utils/Logger.h>
#include <spdlog/spdlog.h>
#include <string>

#define LOG_FUNC() Logger::FuncLogger funcLogger(__PRETTY_FUNCTION__)
#define LOG_ERROR(message, ...) \
  Logger::getInstance().print(std::string(fmt::format(message, ##__VA_ARGS__)).c_str(), LOG_ERROR, __FILE__, __LINE__);
#define LOG_DBG(message, ...) \
  Logger::getInstance().print(std::string(fmt::format(message, ##__VA_ARGS__)).c_str(), LOG_DBG, __FILE__, __LINE__);
#define LOG_INF(message, ...) \
  Logger::getInstance().print(std::string(fmt::format(message, ##__VA_ARGS__)).c_str(), LOG_INF, __FILE__, __LINE__);
#define LOG_WARN(message, ...) \
  Logger::getInstance().print(std::string(fmt::format(message, ##__VA_ARGS__)).c_str(), LOG_WRN, __FILE__, __LINE__);

#endif // __LOGDEFINES_H__
