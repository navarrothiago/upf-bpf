#if !defined(PROTOCOLS_UDP_H)
#define PROTOCOLS_UDP_H

#include <linux/udp.h>
#include <types.h>

static u32 udp_handle(struct xdp_md *ctx, struct udphdr *udph);


#endif // PROTOCOLS_UDP_H
