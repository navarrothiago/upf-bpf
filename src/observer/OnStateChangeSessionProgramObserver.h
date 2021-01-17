#ifndef __ONNEWSESSIONPROGRAMOBSERVER_H__
#define __ONNEWSESSIONPROGRAMOBSERVER_H__

#include <stdlib.h>

class OnStateChangeSessionProgramObserver
{
public:
  virtual ~OnStateChangeSessionProgramObserver(){};
  virtual void onNewSessionProgram(u_int32_t programId, u_int32_t fileDescriptor) = 0;
  virtual void onDestroySessionProgram(u_int32_t programId) = 0;
};

#endif // __ONNEWSESSIONPROGRAMOBSERVER_H__