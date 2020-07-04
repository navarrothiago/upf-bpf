#ifndef BPF_PROTOCOLS_ETH_H
#define BPF_PROTOCOLS_ETH_H

#include <types.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/bpf.h>
#include <protocols/ip.h>
#include <utils/utils.h>

struct vlan_hdr
{
  __be16 h_vlan_TCI;
  __be16 h_vlan_encapsulated_proto;
};

/* 
 * Parse Ethernet layer 2, extract network layer 3 offset and protocol
 * Call next protocol handler (e.g. ipv4).
 * Returns The XDP action.
 */
static u32 eth_handle(struct xdp_md *ctx, struct ethhdr *ethh)
{
  void *data_end = (void *)(long)ctx->data_end;
  u16 eth_type;
  u64 offset;
  struct vlan_hdr *vlan_hdr;

  offset = sizeof(*ethh);
  if ((void *)ethh + offset > data_end)
  {
    bpf_debug("Cannot parse L2\n");
    return XDP_PASS;
  }

  eth_type = htons(ethh->h_proto);
  bpf_debug("Debug: eth_type:0x%x\n", eth_type);

  switch (eth_type)
  {
  case ETH_P_8021Q:
  case ETH_P_8021AD:
    vlan_hdr = (void *)ethh + offset;
    offset += sizeof(*vlan_hdr);
    if (!((void *)ethh + offset > data_end))
      eth_type = htons(vlan_hdr->h_vlan_encapsulated_proto);
  case ETH_P_IP:
    return ipv4_parse(ctx, (struct iphdr *)((void *)ethh + offset));
  case ETH_P_IPV6:
  // Skip non 802.3 Ethertypes
  case ETH_P_ARP:
  // Skip non 802.3 Ethertypes
  // Fall-through
  default:
    bpf_debug("Cannot parse L2: L3off:%llu proto:0x%x\n", offset, eth_type);
    return XDP_PASS;
  }
}
#endif