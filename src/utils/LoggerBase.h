#ifndef LOGGERBASE_H_
#define LOGGERBASE_H_

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

/**
 * Defines the log types.
 */
enum LOG_TYPE_T {
  LOG_FUNCTION = 0,    //!< Log function.
  LOG_BEGIN,           //!< Log function enter.
  LOG_END,             //!< Log function exit.
  LOG_INFO,            //!< Log info messages.
  LOG_WARNING,         //!< Log warning messages.
  LOG_ERROR,           //!< Log error messages.
  LOG_NEW,             //!< Log object new.
  LOG_DELETE,          //!< Log object delete.
  LOG_LOCK,            //!< Log mutex messages.
  LOG_DEBUG,           //!< Log debug messages.
  LOG_MAX              //!< Max log types.
};

/**
 * Maximum method name length.
 */
#define MAX_METHOD_LENGTH 100

/**
 * Maximum string size in a log.
 */
#define MAX_LOG_SIZE 200

/**
 * Maximum string size in a memory log.
 */
#define MAX_MEMORY_LOG_SIZE 20

/**
 * The base class of log.
 */
template <class T>
class LoggerBase
{
public:
  /**
   * Constructor.
   */
  LoggerBase();

  /**
   * Destructor.
   */
  virtual ~LoggerBase();

  /**
   * General log function. The function takes further arguments that may be used for a format string.
   * @param pMessage The message to be logged. Or a format string.
   * @param type The log type.
   * @param pFile The caller file name.
   * @param line The caller line number.
   */
  static void print(const char *pMessage, LOG_TYPE_T type, const char *pFile, int line, ...);

protected:
  /**
   * Performs the concrete logging.
   * @param pMessage The message to be logged.
   * @param type The log type.
   * @param pFile The caller file name.
   * @param line The caller line number.
   */
  static void doPrint(const char *pMessage, LOG_TYPE_T type, const char *pFile = NULL, int line = 0, ...);

  /**
   * Reference to the function name being logged. Adding +1 for String termination.
   */
  char mFunctionName[MAX_METHOD_LENGTH + 1];

  /**
   * Reference to the component being logged. Adding +1 for String termination.
   */
  char mComponent[MAX_COMPONENT_LENGTH + 1];

  /**
   * Reference to the module being logged. Adding +1 for String termination.
   */
  char mModule[MAX_MODULE_LENGTH + 1];
};

template <class T>
LoggerBase<T>::LoggerBase()
{
}

template <class T>
LoggerBase<T>::~LoggerBase()
{
}

template <class T>
void LoggerBase<T>::print(const char *pMessage, LOG_TYPE_T type, const char *pFile, int line, ...)
{
  if(pMessage != NULL) {
    // Create a pointer to store the list o arguments
    // Do NOT initialize with NULL in order to make it compatible with ARM.
    va_list argPtr;
    // Initialize it with all arguments after "line"
    va_start(argPtr, line);
    doPrint(pMessage, type, pFile, line, argPtr);
    // Free the pointer used to store the list.
    va_end(argPtr);
  } else {
    doPrint("NULL", type, pFile, line);
  }
}

template <class T>
void LoggerBase<T>::doPrint(const char *pMessage, LOG_TYPE_T type, const char *pFile, int line, ...)
{
  unsigned int threadId = (unsigned int)pthread_self();
  unsigned int processId = (unsigned int)getpid();
  va_list argPtr;
  // Initialize it with all arguments after "line"
  va_start(argPtr, line);

  T::doPrint(processId, threadId, type, pMessage, pFile, line, argPtr);
  // Free the pointer used to store the list.
  va_end(argPtr);
}

#endif /* LOGGERBASE_H_ */
