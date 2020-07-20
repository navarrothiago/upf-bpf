#if !defined(IE_ETHERNET_PDU_SESSION_INFORMATION)
#define IE_ETHERNET_PDU_SESSION_INFORMATION

#include <ie/ie_base.h>
#include <types.h>

//-------------------------------------
// 8.2.102 Ethernet PDU Session Information
typedef struct ethernet_pdu_session_information
{
  ie_base_t base;
  u8 spare : 7;
  u8 ethi : 1;
} ethernet_pdu_session_information_t;

#endif // IE_ETHERNET_PDU_SESSION_INFORMATION
