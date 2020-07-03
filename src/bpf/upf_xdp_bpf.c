#include <linux/bpf.h>
#include <bpf_helpers.h>
#include <types.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/udp.h>
#include "gtpu.h"

#ifndef htons
#define htons(x) __constant_htons((x))
#endif

// Filter gtp packets.
SEC("xdp_filter_gtp")
int xdp_prog1(struct xdp_md *ctx)
{
  void *data_end = (void *)(long)ctx->data_end;
  void *data = (void *)(long)ctx->data;
  int rc = XDP_PASS;
  struct ethhdr *eth = data;
  struct iphdr *iph;
  struct udphdr *udph;
  struct gtpuhdr *gtpuh;
  u32 dport;

  // Size of non-header.
  u64 nh_off = sizeof(*eth);

  if (data + nh_off > data_end)
  {
    bpf_printk("Ethernet packet not valid\n");
    return rc;
  }

  // Check if the packet is a ip packet.
  if (eth->h_proto != htons(ETH_P_IP))
  {
    bpf_printk("Packet is not ETH_P_IP\n");
    return rc;
  }
  bpf_printk("ETH_P_IP received\n");

  // Get IP header.
  iph = data + nh_off;

  // Check if there is data
  if (iph + 1 > data_end)
  {
    bpf_printk("IP packet not valid\n");
    return rc;
  }

  if (iph->protocol != IPPROTO_UDP)
  {
    bpf_printk("IP protocol is not UDP\n");
    return rc;
  }
  bpf_printk("Valid IPv4 packet: raw saddr:0x%x\n", iph->daddr);


  bpf_printk("IP packet validated\n");

  // why it isnt the exemple below? udph = iph + nh_off;
  // Becaude when you sum 1, one struct size is added.
  udph = iph + 1;

  if (udph + 1 > data_end)
  {
    bpf_printk("UDP packet not valid\n");
    return rc;
  }

  nh_off = sizeof(*udph);

  bpf_printk("UDP packet validated\n");
  dport = htons(udph->dest);

  if (dport != GTP_UDP_PORT)
  {
    bpf_printk("GTP port %lu not valid\n", dport);
    return rc;
  }

  bpf_printk("GTP_UDP_PORT received\n");

  gtpuh = udph + 1;

  if (gtpuh + 1 > data_end)
  {
    bpf_printk("Packet not valid.\n");
    return rc;
  }

  if (gtpuh->message_type != GTPU_G_PDU)
  {
    bpf_printk("Message type 0x%x is not GTPU GPDU(0x%x)\n", gtpuh->message_type, GTPU_G_PDU);
    return rc;
  }

  bpf_printk("GTP GPDU received\n");
  return rc;
}

char _license[] SEC("license") = "GPL";