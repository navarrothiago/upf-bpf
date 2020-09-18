#ifndef __SESSIONBPF_H__
#define __SESSIONBPF_H__

#include <ie/fseid.h>
#include <pfcp/pfcp_session.h>

class SessionBpf
{
public:
  SessionBpf(/* args */) {}
  virtual ~SessionBpf() {}
  virtual seid_t_ getSeid() = 0;
  virtual pfcp_session_t_ getData() = 0;
};

#endif // __SESSIONBPF_H__