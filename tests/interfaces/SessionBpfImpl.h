#ifndef __SESSIONBPFIMPL_H__
#define __SESSIONBPFIMPL_H__

#include <ie/fseid.h>
#include <interfaces/SessionBpf.h>
#include <pfcp/pfcp_session.h>

class SessionBpfImpl : public SessionBpf
{
public:
  SessionBpfImpl(pfcp_session_t_ &session);
  virtual ~SessionBpfImpl();
  seid_t_ getSeid() override;
  pfcp_session_t_ getData() override;

private:
  pfcp_session_t_ mSession;
};

#endif // __SESSIONBPFIMPL_H__