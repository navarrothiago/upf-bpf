#if !defined(PROTOCOLS_UDP_H)
#define PROTOCOLS_UDP_H

#include <linux/udp.h>
#include <types.h>

static u32 udp_handle(struct xdp_md *ctx, struct udphdr *udph, u32 src_ip, u32 dest_ip);


#endif // PROTOCOLS_UDP_H
