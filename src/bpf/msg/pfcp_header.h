#ifndef __PFCP_HEADER_H__
#define __PFCP_HEADER_H__

#include <vmlinux.h>
#include <ie/ie_base.h>

namespace pfcp
{
  //--------------------------------------------------------
  // 7.2.2.2-1: PFCP Message Header for node related messages
  typedef struct header_node
  {
    u8 s : 1;
    u8 mp : 1;
    u8 spare : 3;
    u8 version : 3 u16 message_length;
    u32 sequence_number : 24;
    u32 spare : 8;
  } header_node_t_;

  //--------------------------------------------------------
  // 7.2.2.3-1: PFCP message Header for session related messages
  // TODO
  typedef struct header_node
  {
    u8 s : 1;
    u8 mp : 1;
    u8 spare : 3;
    u8 version : 3
  } header_session_t_;
} // namespace pfcp
#endif // __PFCP_HEADER_H__