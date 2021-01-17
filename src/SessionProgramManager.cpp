#include "SessionProgramManager.h"
#include <utils/LogDefines.h>
#include <wrappers/BPFMap.hpp>
#include <observer/OnStateChangeSessionProgramObserver.h>
#include <SessionProgram.h>
#include <UPFProgram.h>

SessionProgramManager::~SessionProgramManager() 
{
  LOG_FUNC();  
  removeAll();
}

SessionProgramManager& SessionProgramManager::getInstance() 
{
  LOG_FUNC(); 
  static SessionProgramManager sInstance;
  return sInstance;
}

void SessionProgramManager::setProgramsMap(std::shared_ptr<BPFMap> pProgramsMaps) 
{
  LOG_FUNC();
  mpProgramsMap = pProgramsMaps;
}

void SessionProgramManager::create(uint32_t seid) 
{
  LOG_FUNC(); 
  uint32_t *pProgramFd = NULL;

  if(!mpOnNewSessionProgramObserver){
    LOG_ERROR("Observer is invalid")
    throw std::runtime_error("Observer is invalid");
  }

  // Check if there is a key with seid value.
  // TODO navarrothiago - check if can be abstract the programMap.
  if(!mpProgramsMap->lookup(seid, pProgramFd)){
    LOG_ERROR("Cannot create a new program with key (seid) %d", seid);
    throw std::runtime_error("Cannot create a new program with key (seid)");
  }

  // Instantiate a new SessionProgram
  std::shared_ptr<SessionProgram> pSessionProgram = std::make_shared<SessionProgram>();
  pSessionProgram->setup(seid);

  // Update the SessionProgram map.
  mSessionProgramMap.insert(std::pair<uint32_t, std::shared_ptr<SessionProgram>>(seid, pSessionProgram));

  // Notify observer that a SessionProgram was created.
  // TODO - Pass the pSessionProgram in the arg.
  // mpOnNewSessionProgramObserver->onNewSessionProgram(seid, pSessionProgram->getFileDescriptor());
}

void SessionProgramManager::removeAll() 
{
  LOG_FUNC();

  for(auto pair : mSessionProgramMap){
    pair.second->tearDown();

    // Notify observer that a SessionProgram was removed.
    mpOnNewSessionProgramObserver->onDestroySessionProgram(pair.first);
  }
  mSessionProgramMap.clear();
}

void SessionProgramManager::setOnNewSessionObserver(OnStateChangeSessionProgramObserver *pObserver) 
{
  LOG_FUNC();
  mpOnNewSessionProgramObserver = pObserver;
}

SessionProgramManager::SessionProgramManager() 
{
  LOG_FUNC();
}