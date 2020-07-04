#if !defined(BPF_PROTOCOLS_UDP_H)
#define BPF_PROTOCOLS_UDP_H

#include <linux/bpf.h>
#include <linux/in.h>
#include <protocols/gtpu.h>
#include <utils/utils.h>
#include <types.h>

static u32 udp_handle(struct xdp_md *ctx, struct udphdr *udph)
{
  void *data_end = (void *)(long)ctx->data_end;
  u32 dport;
  /* Hint: +1 is sizeof(struct udphdr) */
  if (udph + 1 > data_end)
  {
    bpf_debug("Invalid UDP packet\n");
    return XDP_ABORTED;
  }

  bpf_debug("UDP packet validated\n");
  dport = htons(udph->dest);
  switch (dport)
  {
  case GTP_UDP_PORT:
    return gtp_handle(ctx, (struct gtpuhdr *)(udph + 1));
  default:
    bpf_debug("GTP port %lu not valid\n", dport);
    return XDP_PASS;
  }
}

#endif // BPF_PROTOCOLS_UDP_H
