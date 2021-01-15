#include "SignalHandler.h"
#include <utils/LogDefines.h>
#include <UserPlaneComponent.h>

SignalHandler& SignalHandler::getInstance() 
{
  LOG_FUNC();
  static SignalHandler sInstance;
  return sInstance;
}

SignalHandler::~SignalHandler() 
{
  LOG_FUNC();
}

void SignalHandler::enable() 
{
  LOG_FUNC();
  signal(SIGINT, SignalHandler::tearDown);
  signal(SIGTERM, SignalHandler::tearDown);
  signal(SIGSEGV, SignalHandler::tearDown);
}

void SignalHandler::tearDown(int signal) 
{
  LOG_FUNC();  
  UserPlaneComponent::getInstance().tearDown();
}
