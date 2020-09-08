#if !defined(IE_UE_IP_ADDRESS_H)
#define IE_UE_IP_ADDRESS_H

#include <types.h>

#ifdef KERNEL_SPACE
#include <linux/in.h>
#else
#include <netinet/in.h>
#endif


//-------------------------------------
// 8.2.62 UE IP Address
typedef struct ue_ip_address
{
  ie_base_t_ base;
  u8 ipv6d : 1; // This bit is only applicable to the UE IP address IE in the PDI IE and whhen V6 bit is set to "1". If this bit is set to "1", then the IPv6 Prefix Delegation Bits field shall be present, otherwise the UP function shall consider IPv6 prefix is default /64.
  u8 sd : 1;    // This bit is only applicable to the UE IP Address IE in the PDI IE. It shall be set to "0" and ignored by the receiver in IEs other than PDI IE. In the PDI IE, if this bit is set to "0", this indicates a Source IP address; if this bit is set to "1", this indicates a Destination IP address.
  u8 v4 : 1;    // If this bit is set to "1", then the IPv4 address field shall be present in the UE IP Address, otherwise the IPv4 address field shall not be present.
  u8 v6 : 1;    // If this bit is set to "1", then the IPv6 address field shall be present in the UE IP Address, otherwise the IPv6 address field shall not be present.
  u32 ipv4_address;
  u8 ipv6_address[16];
  u8 ipv6_prefix_delegation_bits;
}ue_ip_address_t_;

#endif // IE_UE_IP_ADDRESS_H
