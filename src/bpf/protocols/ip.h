#if !defined(BPF_PROTOCOLS_IP_H)
#define BPF_PROTOCOLS_IP_H

#include <types.h>
#include <linux/bpf.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <utils/logger.h>
#include <utils/utils.h>
#include <protocols/udp.h>

static u32 ipv4_parse(struct xdp_md *ctx, struct iphdr *iph)
{
  void *data_end = (void *)(long)ctx->data_end;
  // Type need to match map.
  u32 ip_dest; 

  // Hint: +1 is sizeof(struct iphdr) 
  if (iph + 1 > data_end)
  {
    bpf_debug("Invalid IPv4 packet\n");
    return XDP_ABORTED;
  }
  ip_dest = iph->daddr;

  bpf_debug("Valid IPv4 packet: raw daddr:0x%x\n", ip_dest);
  switch (iph->protocol)
  {
  case IPPROTO_UDP:
    return udp_handle(ctx, (struct udphdr *)(iph + 1));
  case IPPROTO_TCP:
  default:
    return XDP_PASS;
  }
}

#endif // BPF_PROTOCOLS_IP_H
