#include "SessionBpfImpl.h"
#include <utils/LogDefines.h>

SessionBpfImpl::SessionBpfImpl(pfcp_session_t_ &session)
  : SessionBpf()
{
  LOG_FUNC();
  mSession = session;
}

SessionBpfImpl::~SessionBpfImpl()
{
  LOG_FUNC();
}

seid_t_ SessionBpfImpl::getSeid()
{
  LOG_FUNC();
  return mSession.seid;
}

pfcp_session_t_ SessionBpfImpl::getData()
{
  LOG_FUNC();
  return mSession;
}
