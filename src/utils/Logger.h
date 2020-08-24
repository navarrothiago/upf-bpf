#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <memory>
#include <spdlog/fwd.h>
#include <spdlog/spdlog.h>
#include <string>

/**
 * Maximum method length.
 */
#define MAX_METHOD_LENGTH 100

class FuncLogger;

class Logger : public spdlog::logger
{
public:
  static Logger &getInstance();
  virtual ~Logger();

  // TODO navarrothiago - exclude the return and the parameters.
  class FuncLogger
  {
  public:
    FuncLogger(const std::string& funcName);
    virtual ~FuncLogger();

  private:
    /**
     * Function name. Adds +1 to put the null character.
     */
    char mFunctionName[MAX_METHOD_LENGTH + 1];
  };

private:
  Logger();
};
#endif // __LOGGER_H__
