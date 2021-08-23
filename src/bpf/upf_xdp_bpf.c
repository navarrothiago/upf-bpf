#define KBUILD_MODNAME upf_xdp_bpf

// clang-format off
#include <types.h>
// clang-format on
#include <bpf_helpers.h>
#include <endian.h>
#include <lib/crc16.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <protocols/eth.h>
#include <protocols/gtpu.h>
#include <protocols/ip.h>
#include <protocols/udp.h>
#include <upf_xdp_bpf_maps.h>
#include <utils/logger.h>
#include <utils/utils.h>
#include <next_prog_rule_key.h>

#ifdef KERNEL_SPACE
#include <linux/in.h>
#else
#include <netinet/in.h>
#endif

/* Defines xdp_stats_map */
#include "xdp_stats_kern.h"
#include "xdp_stats_kern_user.h"

// static u32 tail_call_next_prog(struct xdp_md *p_ctx, teid_t_ teid, u8 source_value, u32 ipv4_address){
//   struct next_rule_prog_index_key map_key;
//   u32 index_prog;
//   map_key.teid = teid;
//   map_key.source_value = INTERFACE_VALUE_CORE;
//   map_key.ipv4_address = ipv4_address;
//   index_prog = bpf_map_lookup_elem(&m_next_rule_prog_index, &map_key);
//   bpf_tail_call(p_ctx, &m_next_rule_prog, index_prog);
// }
/**
 * GTP SECTION.
 */

/**
 * @brief Check if GTP packet is a GPDU. If so, process the next block chain.
 *
 * @param p_ctx The user accessible metadata for xdp packet hook.
 * @param p_gtpuh The GTP header.
 * @return u32 The XDP action.
 */
static u32 gtp_handle(struct xdp_md *p_ctx, struct gtpuhdr *p_gtpuh, u32 dest_ip)
{
  void *p_data_end = (void *)(long)p_ctx->data_end;

  if((void *)p_gtpuh + sizeof(*p_gtpuh) > p_data_end) {
    bpf_debug("Invalid GTPU packet");
    return XDP_DROP;
  }

  // TODO navarrothiago - handle other PDU.
  if(p_gtpuh->message_type != GTPU_G_PDU) {
    bpf_debug("Message type 0x%x is not GTPU GPDU(0x%x)", p_gtpuh->message_type, GTPU_G_PDU);
    return XDP_PASS;
  }

  bpf_debug("GTP GPDU received");

  if(!ip_inner_check_ipv4(p_ctx, (struct iphdr *)(p_gtpuh + 1))) {
    bpf_debug("Invalid IP inner");
    return XDP_DROP;
  }

  // Jump to session context.
  bpf_debug("BPF tail call to %d tunnel\n", htonl(p_gtpuh->teid));
  bpf_tail_call(p_ctx, &m_teid_session, htonl(p_gtpuh->teid));
  bpf_debug("BPF tail call was not executed! teid %d\n", htonl(p_gtpuh->teid));

  // tail_call_next_prog(p_ctx, p_gtpuh->teid, INTERFACE_VALUE_ACCESS, dest_ip);
  return XDP_PASS;
}

/**
 * UDP SECTION.
 */

/**
 * @brief Handle UDP header.
 *
 * @param p_ctx The user accessible metadata for xdp packet hook.
 * @param udph The UDP header.
 * @return u32 The XDP action.
 */
static u32 udp_handle(struct xdp_md *p_ctx, struct udphdr *udph, u32 dest_ip)
{
  void *p_data_end = (void *)(long)p_ctx->data_end;
  struct next_rule_prog_index_key map_key;
  u32 index_prog;
  u32 dport;

  // Apply hash function due to limitation of size of the program map.
  uint32_t key = (uint32_t)((dest_ip * 0x80008001) >> 16);

  /* Hint: +1 is sizeof(struct udphdr) */
  if((void *)udph + sizeof(*udph) > p_data_end) {
    bpf_debug("Invalid UDP packet");
    return XDP_ABORTED;
  }

  bpf_debug("UDP packet validated");
  dport = htons(udph->dest);

  switch(dport) {
  case GTP_UDP_PORT:
    return gtp_handle(p_ctx, (struct gtpuhdr *)(udph + 1), dest_ip);
  default:
    bpf_debug("BPF tail call to 0x%x address %d key", dest_ip, key);
    // TODO navarrothiago - Assuming there is a map one-to-one IP-Session
    bpf_tail_call(p_ctx, &m_ueip_session, key);
    bpf_debug("BPF tail call was not executed!");

    // tail_call_next_prog(p_ctx, 0, INTERFACE_VALUE_CORE, dest_ip);

    return XDP_PASS;
  }
}

/**
 * IP SECTION.
 */

/**
 * @brief Handle IPv4 header.
 *
 * @param p_ctx The user accessible metadata for xdp packet hook.
 * @param iph The IP header.
 * @return u32 The XDP action.
 */
static u32 ipv4_handle(struct xdp_md *p_ctx, struct iphdr *iph)
{
  void *p_data_end = (void *)(long)p_ctx->data_end;
  // Type need to match map.
  u32 ip_dest;

  // Hint: +1 is sizeof(struct iphdr)
  if((void *)iph + sizeof(*iph) > p_data_end) {
    bpf_debug("Invalid IPv4 packet");
    return XDP_ABORTED;
  }
  ip_dest = iph->daddr;

  bpf_debug("Valid IPv4 packet: raw daddr:0x%x", ip_dest);
  switch(iph->protocol) {
  case IPPROTO_UDP:
    return udp_handle(p_ctx, (struct udphdr *)(iph + 1), ip_dest);
  case IPPROTO_TCP:
  default:
    bpf_debug("TCP protocol L4");
    return XDP_PASS;
  }
}

/**
 * @brief Check if inner IP header is IPv4.
 *
 * @param p_ctx The user accessible metadata for xdp packet hook.
 * @param iph The IP header.
 * @return u8 The XDP action.
 */
static u8 ip_inner_check_ipv4(struct xdp_md *p_ctx, struct iphdr *iph)
{
  void *p_data_end = (void *)(long)p_ctx->data_end;

  // Hint: +1 is sizeof(struct iphdr)
  if((void *)iph + sizeof(*iph) > p_data_end) {
    bpf_debug("Invalid IPv4 packet");
    return XDP_ABORTED;
  }

  return iph->version == 4;
}

/**
 * ETHERNET SECTION.
 */

struct vlan_hdr {
  __be16 h_vlan_TCI;
  __be16 h_vlan_encapsulated_proto;
};

/**
 *
 * @brief Parse Ethernet layer 2, extract network layer 3 offset and protocol
 * Call next protocol handler (e.g. ipv4).
 *
 * @param p_ctx
 * @param ethh
 * @return u32 The XDP action.
 */
static u32 eth_handle(struct xdp_md *p_ctx, struct ethhdr *ethh)
{
  void *p_data_end = (void *)(long)p_ctx->data_end;
  u16 eth_type;
  u64 offset;
  struct vlan_hdr *vlan_hdr;

  offset = sizeof(*ethh);
  if((void *)ethh + offset > p_data_end) {
    bpf_debug("Cannot parse L2");
    return XDP_PASS;
  }

  eth_type = htons(ethh->h_proto);
  bpf_debug("Debug: eth_type:0x%x", eth_type);

  switch(eth_type) {
  case ETH_P_8021Q:
  case ETH_P_8021AD:
    bpf_debug("VLAN!! Changing the offset");
    vlan_hdr = (void *)ethh + offset;
    offset += sizeof(*vlan_hdr);
    if(!((void *)ethh + offset > p_data_end))
      eth_type = htons(vlan_hdr->h_vlan_encapsulated_proto);
    // Enter in next case.
  case ETH_P_IP:
    return ipv4_handle(p_ctx, (struct iphdr *)((void *)ethh + offset));
  case ETH_P_IPV6:
  // Skip non 802.3 Ethertypes
  case ETH_P_ARP:
  // Skip non 802.3 Ethertypes
  // Fall-through
  default:
    bpf_debug("Cannot parse L2: L3off:%llu proto:0x%x", offset, eth_type);
    return XDP_PASS;
  }
}

SEC("xdp_entry_point")
int entry_point(struct xdp_md *p_ctx)
{
  void *p_data = (void *)(long)p_ctx->data;
  struct ethhdr *eth = p_data;

  bpf_debug("XDP ENTRY POINT");

  // Start to handle the ethernet header.
  u32 action = xdp_stats_record_action(p_ctx, eth_handle(p_ctx, eth));
  bpf_debug("Action %d", action);

  return action;
}

char _license[] SEC("license") = "GPL";
