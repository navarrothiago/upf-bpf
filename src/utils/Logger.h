#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <memory>
// #include <spdlog/fwd.h>
#include <spdlog/spdlog.h>
#include <string>

/**
 * Defines the log types.
 */
enum LOG_TYPE_T {
  LOG_FUNCTION = 0,    //!< Log function.
  LOG_BEGIN,           //!< Log function enter.
  LOG_END,             //!< Log function exit.
  LOG_INF,            //!< Log info messages.
  LOG_WRN,         //!< Log warning messages.
  LOG_ERROR,           //!< Log error messages.
  LOG_NEW,             //!< Log object new.
  LOG_DELETE,          //!< Log object delete.
  LOG_LOCK,            //!< Log mutex messages.
  LOG_DBG,           //!< Log debug messages.
  LOG_MAX              //!< Max log types.
};

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
  /**
   * General log function. The function takes further arguments that may be used for a format string.
   * @param pMessage The message to be logged. Or a format string.
   * @param type The log type.
   * @param pFile The caller file name.
   * @param line The caller line number.
   */
  void print(const char *pMessage, LOG_TYPE_T type, const char *pFile, int line = 0);
  /**
   * @brief Wrapper the spdlog calls (e.g. error, debug, ...).
   *
   * @param type Type of the log.
   * @param out  The output.
   */
  void doPrint(LOG_TYPE_T type, const std::string &out);

  // TODO navarrothiago - exclude the return and the parameters.
  class FuncLogger
  {
  public:
    FuncLogger(const std::string &funcName);
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
