#ifndef __SESSIONPROGRAMMANAGER_H__
#define __SESSIONPROGRAMMANAGER_H__

#include <stdint.h>
#include <memory>
#include <map>

class BPFMap;
class OnStateChangeSessionProgramObserver;
class SessionProgram;

/**
 * @brief This class is used to manager the SessionProgramManager.
 * 
 */
class SessionProgramManager
{
public:
  /**
   * @brief Destroy the Session Program Manager object.
   *
   */
  virtual ~SessionProgramManager();
  /**
   * @brief Get the Instance object.
   *
   * @return SessionProgramManager& The unique instance.
   */
  static SessionProgramManager &getInstance();

  /**
   * @brief Set the Programs Map object.
   * 
   * @param pProgramsMaps  The programs wrapper for BPF map.
   */
  void setTeidSessionMap(std::shared_ptr<BPFMap> pProgramsMaps);

  /**
   * @brief Create a new program related to the SEID.
   * The context will be empty.
   *
   * @param seid  The session identifier.
   */
  void create(uint32_t seid);
  /**
   * @brief Remove program session context.
   * 
   * @param seid The session identifier.
   */
  void remove(uint32_t seid);
  /**
   * @brief Remove all programs.
   * 
   */
  void removeAll();
  /**
   * @brief Set the On New Session Observer object.
   * 
   * @param pObserver The observer which will be notify when a SessionProgram is created.
   */
  void setOnNewSessionObserver(OnStateChangeSessionProgramObserver *pObserver);
  /**
   * @brief Find the Session Program object.
   * 
   * @param seid The session identifier.
   * @return std::shared_ptr<SessionProgram> The program, which represents the session.
   */
  std::shared_ptr<SessionProgram> findSessionProgram(uint32_t seid);

private:
  /**
   * @brief Construct a new Session Program Manager object.
   * 
   */
  SessionProgramManager();
  
  // The program eBPF map.
  std::shared_ptr<BPFMap> mpTeidSessionMap;

  // The program eBPF map.
  std::shared_ptr<BPFMap> mpUeIpSessionMap;

  // The observer which will be notify when a SessionProgram is created.
  OnStateChangeSessionProgramObserver *mpOnNewSessionProgramObserver;

  // The Maps to store the instance of the programs.
  std::map<uint32_t, std::shared_ptr<SessionProgram>> mSessionProgramMap;
};

#endif // __SESSIONPROGRAMMANAGER_H__