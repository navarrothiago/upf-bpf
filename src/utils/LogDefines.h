#ifndef __LOGDEFINES_H__
#define __LOGDEFINES_H__

#include <utils/Logger.h>

#define LOG_FUNC() Logger::FuncLogger funcLogger(__PRETTY_FUNCTION__)
#define LOG_ERROR(message, ...) Logger::getInstance().error(message, ##__VA_ARGS__);
#define LOG_DEBUG(message, ...) Logger::getInstance().debug(message, ##__VA_ARGS__);
#define LOG_INFO(message, ...) Logger::getInstance().info(message, ##__VA_ARGS__);



#endif // __LOGDEFINES_H__
