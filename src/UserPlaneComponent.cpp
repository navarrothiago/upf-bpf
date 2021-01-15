#include "UserPlaneComponent.h"
#include <utils/LogDefines.h>
#include <UPFProgram.h>
#include <SessionManager.h>
#include <SessionProgram.h>
#include <RulesUtilities.h>
#include <SignalHandler.h>

UserPlaneComponent::UserPlaneComponent() 
{
  LOG_FUNC();
  // Set new handlers for libbpf.
  libbpf_set_print(UserPlaneComponent::printLibbpfLog);
  mpUPFProgram = std::make_shared<UPFProgram>();
}

UserPlaneComponent::~UserPlaneComponent() 
{
  LOG_FUNC();
  tearDown();
}

std::shared_ptr<SessionManager> UserPlaneComponent::getSessionManager() const
{
  LOG_FUNC();
  return mpSessionManager;
}

std::shared_ptr<RulesUtilities> UserPlaneComponent::getRulesUtilities() const
{
  LOG_FUNC();
  return mpRulesUtilities;
}

std::shared_ptr<UPFProgram> UserPlaneComponent::getUPFProgram() const
{
  LOG_FUNC();
  return mpUPFProgram;
}

int UserPlaneComponent::printLibbpfLog(enum libbpf_print_level lvl, const char *fmt, va_list args)
{
  // Do not put LOG_FUNC() here.
  return vfprintf(stderr, fmt, args);
}

UserPlaneComponent& UserPlaneComponent::getInstance() 
{
  LOG_FUNC();
  static UserPlaneComponent sInstance;
  return sInstance;
}

void UserPlaneComponent::setup(std::shared_ptr<RulesUtilities> pRulesUtilities) 
{
  LOG_FUNC();

  mpRulesUtilities = pRulesUtilities;

  if(!mpUPFProgram){
    LOG_ERROR("Program not initialized");
    throw std::runtime_error("Program not initialized");
  }

  SignalHandler::getInstance().enable();
  mpUPFProgram->setup();

  // Pass maps to sessionManager.
  mpSessionManager = std::make_shared<SessionManager>(mpUPFProgram->getSessionsMap(), mpUPFProgram->getUplinkPDRsMap());
}

void UserPlaneComponent::tearDown() 
{
  LOG_FUNC();
  mpUPFProgram->tearDown();
}
