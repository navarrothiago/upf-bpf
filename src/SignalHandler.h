#include <signal.h>

class SignalHandler
{
public:
  static SignalHandler &getInstance();
  virtual ~SignalHandler();
  void enable();
  static void tearDown(int signal = SIGTERM);
};
