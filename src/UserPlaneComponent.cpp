#include "UserPlaneComponent.h"
#include <RulesUtilities.h>
#include <SessionManager.h>
#include <SessionProgram.h>
#include <SessionProgramManager.h>
#include <SignalHandler.h>
#include <UPFProgram.h>
#include <utils/LogDefines.h>

UserPlaneComponent::UserPlaneComponent()
{
  LOG_FUNC();
// Set new handlers for libbpf.
#ifdef DEBUG_LIBBPF
  libbpf_set_print(UserPlaneComponent::printLibbpfLog);
#endif
  // TODO navarrothiago - Create the programs according to the load configuration
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

void UserPlaneComponent::onNewSessionProgram(u_int32_t programId, u_int32_t fileDescriptor)
{
  LOG_FUNC();
  mpUPFProgram->updateProgramMap(programId, fileDescriptor);
}

void UserPlaneComponent::onDestroySessionProgram(u_int32_t programId)
{
  LOG_FUNC();
  mpUPFProgram->removeProgramMap(programId);
}

int UserPlaneComponent::printLibbpfLog(enum libbpf_print_level lvl, const char *fmt, va_list args)
{
  // Do not put LOG_FUNC() here.
  return vfprintf(stderr, fmt, args);
}

UserPlaneComponent &UserPlaneComponent::getInstance()
{
  LOG_FUNC();
  static UserPlaneComponent sInstance;
  return sInstance;
}

void UserPlaneComponent::setup(std::shared_ptr<RulesUtilities> pRulesUtilities)
{
  LOG_FUNC();

  mpRulesUtilities = pRulesUtilities;

  if(!mpUPFProgram) {
    LOG_ERROR("Program not initialized");
    throw std::runtime_error("Program not initialized");
  }

  SignalHandler::getInstance().enable();
  mpUPFProgram->setup();

  // Set program map in SesssionProgramManager.
  SessionProgramManager::getInstance().setProgramsMap(mpUPFProgram->getProgramsMap());
  
  // Pass maps to sessionManager.
  mpSessionManager = std::make_shared<SessionManager>();

  // Set this observer in SessionProgramManager.
  // Now It will receive notification when a SessionProgram is created.
  SessionProgramManager::getInstance().setOnNewSessionObserver(this);
}

void UserPlaneComponent::tearDown()
{
  LOG_FUNC();
  mpUPFProgram->tearDown();
  SessionProgramManager::getInstance().removeAll();
}
