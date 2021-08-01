#if !defined(PROTOCOLS_ETH_H)
#define PROTOCOLS_ETH_H

#include <vmlinux.h>

static u32 eth_handle(struct xdp_md *ctx, struct ethhdr *ethh);


#endif // PROTOCOLS_ETH_H
