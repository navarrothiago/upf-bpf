
// clang-format off
#include <types.h>
// clang-format on

#include "xdp_stats_kern.h"
#include <bpf_helpers.h>
#include <endian.h>
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <pfcp/pfcp_far.h>
#include <pfcp/pfcp_pdr.h>
#include <protocols/gtpu.h>
#include <protocols/ip.h>
#include <utils/csum.h>
#include <utils/logger.h>
#include <utils/utils.h>
#include <far_maps.h>

#ifndef LOCAL_IP
// 10.1.3.30
#define LOCAL_IP 503513354
#endif
#ifndef LOCAL_MAC
#define LOCAL_MAC 0
#endif

// TODO navarrothiago - Put dummy in test folder.
/**
 * WARNING: Redirect require an XDP bpf_prog loaded on the TX device.
 */
SEC("xdp_redirect_dummy")
int xdp_redirect_gtpu(struct xdp_md *p_ctx)
{
  // PASS.
  bpf_debug("Redirecting packets");
  return XDP_PASS;
}

/**
 * @brief Update MAC address
 *
 * @param p_ip The IP header which has the destination IP address.
 * @param [out] p_eth  The eth header with the new mac address.
 * @return u32 0 (Success), 1 (Fail).
 */
static u32 update_dst_mac_address(struct iphdr *p_ip, struct ethhdr *p_eth)
{
  void *p_mac_address;

  p_mac_address = bpf_map_lookup_elem(&m_arp_table, &p_ip->daddr);
  if(!p_mac_address) {
    bpf_debug("mac not found!!\n");
    return 1;
  }
  memcpy(p_eth->h_dest, p_mac_address, sizeof(p_eth->h_dest));
  return 0;
}

static u32 create_outer_header_gtpu_ipv4(struct xdp_md *p_ctx, pfcp_far_t_ *p_far)
{
  bpf_debug("create_outer_header_gtpu_ipv4");
  struct ethhdr *p_eth;
  struct iphdr *p_ip;
  void *p_data = (void *)(long)p_ctx->data;
  void *p_data_end = (void *)(long)p_ctx->data_end;
  void *p_mac_address;
  struct bpf_fib_lookup fib_params = {};

  // KISS - Lets start using the first PDR (high priority).
  // Resize the header in order to put the GTP/UPD/IP headers.
  // Adjust space to the left.
  bpf_xdp_adjust_head(p_ctx, (int32_t)-GTP_ENCAPSULATED_SIZE);

  // Packet buffer changed, all pointers need to be recomputed
  p_data = (void *)(long)p_ctx->data;
  p_data_end = (void *)(long)p_ctx->data_end;

  p_eth = p_data;
  if((void *)(p_eth + 1) > p_data_end) {
    bpf_debug("Invalid pointer");
    return XDP_DROP;
  }

  // Space allocated before packet buffer, move eth header
  struct ethhdr *p_orig_eth = p_data + GTP_ENCAPSULATED_SIZE;
  if((void *)(p_orig_eth + 1) > p_data_end) {
    return XDP_DROP;
  }
  memcpy(p_eth, p_orig_eth, sizeof(*p_eth));

  p_ip = (void *)(p_eth + 1);
  if((void *)(p_ip + 1) > p_data_end) {
    return XDP_DROP;
  }

  struct iphdr *p_inner_ip = (void *)p_ip + GTP_ENCAPSULATED_SIZE;
  if((void *)(p_inner_ip + 1) > p_data_end) {
    return XDP_DROP;
  }

  // TODO navarrothiago -  Check this code
  // https://github.com/atoonk/xdp-tutorial/blob/master/packet-solutions/xdp_prog_kern_03.c#L225-L320

  // Add the outer IP header
  p_ip->version = 4;
  p_ip->ihl = 5; // No options
  p_ip->tos = 0;
  p_ip->tot_len = htons(ntohs(p_inner_ip->tot_len) + GTP_ENCAPSULATED_SIZE);
  p_ip->id = 0;            // No fragmentation
  p_ip->frag_off = 0x0040; // Don't fragment; Fragment offset = 0
  p_ip->ttl = 64;
  p_ip->protocol = IPPROTO_UDP;
  p_ip->check = 0;
  p_ip->saddr = LOCAL_IP;
  p_ip->daddr = p_far->forwarding_parameters.outer_header_creation.ipv4_address.s_addr;

  // Add the UDP header
  struct udphdr *p_udp = (void *)(p_ip + 1);
  if((void *)(p_udp + 1) > p_data_end) {
    return XDP_DROP;
  }

  p_udp->source = htons(GTP_UDP_PORT);
  p_udp->dest = htons(p_far->forwarding_parameters.outer_header_creation.port_number);
  p_udp->len = htons(ntohs(p_inner_ip->tot_len) + sizeof(*p_udp) + sizeof(struct gtpuhdr));
  p_udp->check = 0;

  bpf_debug("Destination MAC:%x:%x:%x\n", p_eth->h_dest[0], p_eth->h_dest[1], p_eth->h_dest[2]);
  bpf_debug("Destination MAC:%x:%x:%x\n", p_eth->h_dest[3], p_eth->h_dest[4], p_eth->h_dest[5]);
  p_mac_address = bpf_map_lookup_elem(&m_arp_table, &p_ip->daddr);
  if(!p_mac_address) {
    bpf_debug("mac not found!!\n");
    return XDP_DROP;
  }
  // swap_src_dst_mac(p_data);
  memcpy(p_eth->h_dest, p_mac_address, sizeof(p_eth->h_dest));
  bpf_debug("Destination MAC:%x:%x:%x\n", p_eth->h_dest[0], p_eth->h_dest[1], p_eth->h_dest[2]);
  bpf_debug("Destination MAC:%x:%x:%x\n", p_eth->h_dest[3], p_eth->h_dest[4], p_eth->h_dest[5]);
  bpf_debug("Destination IP:%d Port:%d\n", p_ip->daddr, p_far->forwarding_parameters.outer_header_creation.port_number);

  // Add the GTP header
  struct gtpuhdr *p_gtpuh = (void *)(p_udp + 1);
  if((void *)(p_gtpuh + 1) > p_data_end) {
    return XDP_DROP;
  }

  u8 flags = GTP_FLAGS;
  memcpy(p_gtpuh, &flags, sizeof(u8));
  p_gtpuh->message_type = GTPU_G_PDU;
  p_gtpuh->message_length = p_inner_ip->tot_len;
  p_gtpuh->teid = p_far->forwarding_parameters.outer_header_creation.teid;

  // Compute l3 checksum
  __wsum l3sum = pcn_csum_diff(0, 0, (__be32 *)p_ip, sizeof(*p_ip), 0);
  pcn_l3_csum_replace(p_ctx, IP_CSUM_OFFSET, 0, l3sum, 0);

  bpf_debug("GTPU header were pushed!");
}

/**
 * @brief Apply forwarding action rules.
 *
 * @param p_gtpuh The GTPU header.
 * @param p_session The session of this context.
 * @return u32 XDP action.
 */

static u32 pfcp_far_apply(struct xdp_md *p_ctx, pfcp_far_t_ *p_far, enum FlowDirection direction)
{
  void *p_data = (void *)(long)p_ctx->data;
  void *p_data_end = (void *)(long)p_ctx->data_end;
  struct ethhdr *p_eth = p_data;
  void *p_mac_address;

  u8 dest_interface;
  u16 outer_header_creation;
  // TODO dupl
  // TODO nocp
  // TODO buff

  if((void *)(p_eth + 1) > p_data_end) {
    bpf_debug("Invalid pointer");
    return XDP_DROP;
  }

  // Check if it is a forward action.
  if(!p_far) {
    bpf_debug("Invalid FAR!");
    return XDP_DROP;
  }

  dest_interface = p_far->forwarding_parameters.destination_interface.interface_value;
  outer_header_creation = p_far->forwarding_parameters.outer_header_creation.outer_header_creation_description;

  // TODO navarrothiago - reorder the if's
  if(p_far->apply_action.forw) {
    if(dest_interface == INTERFACE_VALUE_CORE) {
      // Redirect to data network.
      bpf_debug("Destination is to INTERFACE_VALUE_CORE\n");
      // Check Outer header creation - IPv4 or IPv6
      switch(outer_header_creation) {
      case OUTER_HEADER_CREATION_UDP_IPV4:
        bpf_debug("OUTER_HEADER_CREATION_UDP_IPV4\n");
        struct ethhdr *p_new_eth = p_data + GTP_ENCAPSULATED_SIZE;

        // Move eth header forward.
        if((void *)(p_new_eth + 1) > p_data_end) {
          return 1;
        }
        __builtin_memcpy(p_new_eth, p_eth, sizeof(*p_eth));

        // Update destination mac address.
        struct iphdr *p_ip = (void *)(p_new_eth + 1);

        if((void *)(p_ip + 1) > p_data_end) {
          return XDP_DROP;
        }

        if(update_dst_mac_address(p_ip, p_new_eth)) {
          return XDP_DROP;
        }

        // Adjust head to the right.
        bpf_xdp_adjust_head(p_ctx, GTP_ENCAPSULATED_SIZE);

        return bpf_redirect_map(&m_redirect_interfaces, direction, 0);
        bpf_debug("OUTER_HEADER_CREATION_UDP_IPV4 REDIRECT FAILED\n");
        break;
      case OUTER_HEADER_CREATION_UDP_IPV6:
        bpf_debug("OUTER_HEADER_CREATION_UDP_IPV6\n");
        // TODO navarrothiago
        break;
      default:
        bpf_debug("In destination to CORE - Invalid option: %d", outer_header_creation);
      }
    } else if(dest_interface == INTERFACE_VALUE_ACCESS) {
      // Redirect to core network.
      bpf_debug("Destination is to INTERFACE_VALUE_ACCESS");
      switch(outer_header_creation) {
      case OUTER_HEADER_CREATION_GTPU_UDP_IPV4:
        bpf_debug("OUTER_HEADER_CREATION_GTPU_UDP_IPV4");
        create_outer_header_gtpu_ipv4(p_ctx, p_far);
        return bpf_redirect_map(&m_redirect_interfaces, direction, 0);
        break;
      case OUTER_HEADER_CREATION_GTPU_UDP_IPV6:
        bpf_debug("OUTER_HEADER_CREATION_GTPU_UDP_IPV6");
        break;
      default:
        bpf_debug("In destination to ACCESS - Invalid option: %d", outer_header_creation);
      }
    }
  } else {
    bpf_debug("Forward action unset");
  }
  return XDP_PASS;
}

SEC("xdp_far")
int far_entry_point(struct xdp_md *p_ctx)
{
  pfcp_far_t_ *p_far;
  u32 key = 0;
  bpf_debug("XDP SESSION CONTEXT - DOWNLINK\n");

  // Lets apply the forwarding actions rule.
  p_far = bpf_map_lookup_elem(&m_far, &key);
  return pfcp_far_apply(p_ctx, p_far, DOWNLINK);
}

// For printk.
char _license[] SEC("license") = "GPL";
