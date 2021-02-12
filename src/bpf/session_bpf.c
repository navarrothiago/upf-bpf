// clang-format off
#include <types.h>
// clang-format on

#include <bpf_helpers.h>
#include "xdp_stats_kern.h"
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <pfcp/pfcp_far.h>
#include <pfcp/pfcp_pdr.h>
#include <protocols/gtpu.h>
#include <protocols/ip.h>
#include <session_bpf_maps.h>
#include <utils/logger.h>
#include <utils/utils.h>
#include <utils/csum.h>
#include <endian.h>

#ifndef LOCAL_IP
#define LOCAL_IP 0
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

static u32 create_outer_header_gtpu_ipv4(struct xdp_md *p_ctx, pfcp_far_t_ *p_far)
{
  bpf_debug("create_outer_header_gtpu_ipv4");
  struct ethhdr *p_eth;
  struct iphdr *p_ip;
  void *p_data = (void *)(long)p_ctx->data;
  void *p_data_end = (void *)(long)p_ctx->data_end;

  // KISS - Lets start using the first PDR (high priority).
  // Resize the header in order to put the GTP/UPD/IP headers.
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

  bpf_debug("Destination MAC:%x:%x:%x", p_eth->h_dest[0], p_eth->h_dest[1], p_eth->h_dest[2]);
  bpf_debug("Destination MAC:%x:%x:%x", p_eth->h_dest[3], p_eth->h_dest[4], p_eth->h_dest[5]);
  swap_src_dst_mac(p_data);
  bpf_debug("Destination MAC:%x:%x:%x", p_eth->h_dest[0], p_eth->h_dest[1], p_eth->h_dest[2]);
  bpf_debug("Destination MAC:%x:%x:%x", p_eth->h_dest[3], p_eth->h_dest[4], p_eth->h_dest[5]);
  bpf_debug("Destination IP:%d Port:%d", p_ip->daddr, p_far->forwarding_parameters.outer_header_creation.port_number);

  // Add the GTP header
  struct gtpuhdr *p_gtpuh = (void *)(p_udp + 1);
  if((void *)(p_gtpuh + 1) > p_data_end) {
    return XDP_DROP;
  }

  u8 flags = GTP_FLAGS;
  memcpy(p_gtpuh, &flags, sizeof(u8));
  p_gtpuh->message_type = GTPU_G_PDU;
  p_gtpuh->message_length = p_inner_ip->tot_len;
  p_gtpuh->teid =  p_far->forwarding_parameters.outer_header_creation.teid;

  // Compute l3 checksum
  __wsum l3sum = pcn_csum_diff(0, 0, (__be32 *)p_ip, sizeof(*p_ip), 0);
  pcn_l3_csum_replace(p_ctx, IP_CSUM_OFFSET, 0, l3sum, 0);

  bpf_debug("GTPU header were pushed!");

  return bpf_redirect_map(&m_redirect_interfaces, 0, 0);
}

/**
 * @brief Apply forwarding action rules.
 *
 * @param p_gtpuh The GTPU header.
 * @param p_session The session of this context.
 * @return u32 XDP action.
 */

static u32 pfcp_far_apply(struct xdp_md *p_ctx, pfcp_far_t_ *p_far)
{
  u8 dest_interface;
  u16 outer_header_creation;
  // TODO dupl
  // TODO nocp
  // TODO buff

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
      bpf_debug("Destination is to INTERFACE_VALUE_CORE");
      // Check Outer header creation - IPv4 or IPv6
      switch(outer_header_creation) {
      case OUTER_HEADER_CREATION_UDP_IPV4:
        bpf_debug("OUTER_HEADER_CREATION_UDP_IPV4");
        // swap_src_dst_mac(p_data);
        return bpf_redirect_map(&m_redirect_interfaces, 0, 0);
        break;
      case OUTER_HEADER_CREATION_UDP_IPV6:
        bpf_debug("OUTER_HEADER_CREATION_UDP_IPV6");
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
        return create_outer_header_gtpu_ipv4(p_ctx, p_far);
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

/**
 * @brief Match the PDRs attribuites for UL data flow.
 * - The TEID from GTP GPDU with the TEID stored in PDR.
 * - Source IP from IP header with source address stored in PDI.
 * - Interface from PDI with ACCESS interface value.
 *
 * @param p_pdr The PDR to be match with the header.
 * @param p_iph The IP header.
 * @return u8 True if match. False cc.
 */
static u32 pfcp_pdr_match_pdi_access(pfcp_pdr_t_ *p_pdr, struct iphdr *p_iph, teid_t_ teid)
{
  if(!p_iph) {
    bpf_debug("IP header is NULL!!");
    return 0;
  }

  // clang-format off
  if(p_pdr->outer_header_removal.outer_header_removal_description != OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4
      || p_pdr->pdi.source_interface.interface_value != INTERFACE_VALUE_ACCESS
      || p_pdr->pdi.fteid.teid != teid
      // FIXME navarrothiago
      // || p_pdr->pdi.ue_ip_address.ipv4_address != p_iph->saddr
    ){
        bpf_debug("Not match:");
        bpf_debug("OHRD: %d", OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4 );
        bpf_debug("OHRD: %d", p_pdr->outer_header_removal.outer_header_removal_description);
        bpf_debug("Interface: %d", INTERFACE_VALUE_ACCESS);
        bpf_debug("Interface: %d", p_pdr->pdi.source_interface.interface_value);
        bpf_debug("TEID: %d", teid);
        bpf_debug("TEID: %d", p_pdr->pdi.fteid.teid);
        // bpf_debug("IPv4: %d", p_iph->saddr);
        bpf_debug("IPv4: %d", p_pdr->pdi.ue_ip_address.ipv4_address);
        return 1;
    }
  // clang-format on

  // All the attributes were matched.
  bpf_debug("All atrributes were matched!!");
  return 0;
}

/**
 * @brief Match the PDRs attribuites for DL data flow.
 * - Destination IP from IP header with source address stored in PDI.
 * - Interface from PDI with CORE interface value.
 *
 * @param p_pdr The PDR to be match with the header.
 * @param p_iph The IP header.
 * @return u8 True if match. False cc.
 */
static u32 pfcp_pdr_match_pdi_downlink(pfcp_pdr_t_ *p_pdr, struct iphdr *p_iph)
{
  if(!p_iph) {
    bpf_debug("IP header is NULL!!");
    return 0;
  }

  // clang-format off
  if(p_pdr->outer_header_removal.outer_header_removal_description != OUTER_HEADER_REMOVAL_UDP_IPV4
      || p_pdr->pdi.source_interface.interface_value != INTERFACE_VALUE_CORE
      // || p_pdr->pdi.fteid.teid != teid
      // FIXME navarrothiago
      || p_pdr->pdi.ue_ip_address.ipv4_address != p_iph->daddr
    ){
        bpf_debug("Not match:");
        bpf_debug("OHRD: %d", OUTER_HEADER_REMOVAL_UDP_IPV4 );
        bpf_debug("OHRD: %d", p_pdr->outer_header_removal.outer_header_removal_description);
        bpf_debug("Interface: %d", INTERFACE_VALUE_CORE);
        bpf_debug("Interface: %d", p_pdr->pdi.source_interface.interface_value);
        // bpf_debug("TEID: %d", teid);
        // bpf_debug("TEID: %d", p_pdr->pdi.fteid.teid);
        bpf_debug("IPv4: %d", p_iph->daddr);
        bpf_debug("IPv4: %d", p_pdr->pdi.ue_ip_address.ipv4_address);
        return 1;
    }
  // clang-format on

  // All the attributes were matched.
  bpf_debug("All atrributes were matched!!");
  return 0;
}
/**
 * @brief Lookup all PDRs based on teid.
 * After that, for each PDR, check the its attribuites with match with access way.
 * - The TEID from GTP GPDU with the TEID stored in PDR.
 * - Source IP from IP header with source address stored in PDI.
 * - Interface from PDI with ACCESS or CORE interface (it depends on if it is UL or DL).
 * After match all field, get the session id in the found PDR.
 *
 * @param p_ctx The user accessible metadata for xdp packet hook.
 * @param p_gtpuh
 * @return u32
 */
static u32 pfcp_pdr_lookup_uplink(struct xdp_md *p_ctx)
{
  pfcp_pdr_t_ *p_pdr;
  pfcp_far_t_ *p_far;
  struct gtpuhdr *p_gtpuh;
  u64 offset;
  teid_t_ teid;
  seid_t_ seid;
  u32 index = 0;
  u32 i;

  void *p_data = (void *)(long)p_ctx->data;
  void *p_data_end = (void *)(long)p_ctx->data_end;

  offset = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct gtpuhdr);

  if(p_data + offset > p_data_end) {
    bpf_debug("Invalid GTP packet!");
    return XDP_PASS;
  }

  // Get GTP base address.
  p_gtpuh = p_data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);

  // We have already assumed that the packet is a GPDU.
  u8 *p_iph = (u8 *)p_gtpuh + GTPV1U_MSG_HEADER_MIN_SIZE;

  teid = htonl(p_gtpuh->teid);
  bpf_debug("GTP GPDU teid %d with IPv4 payload received", teid);

  p_pdr = bpf_map_lookup_elem(&m_teid_pdr, &teid);

  if(!p_pdr) {
    bpf_debug("Error - unsync teid->pdrs map.");
    return XDP_DROP;
  }

  // For each PDR, check parameters.
  if(pfcp_pdr_match_pdi_access(&p_pdr[i], (struct iphdr *)p_iph, teid) == 0) {
    bpf_debug("PDR associated with teid %d found! PDR id is %d", teid, p_pdr->pdr_id.rule_id);

    // Lets apply the forwarding actions rule.
    p_far = bpf_map_lookup_elem(&m_fars, &p_pdr->far_id.far_id);
    return pfcp_far_apply(p_ctx, p_far);
  }

  return XDP_PASS;
}
/**
 * @brief Lookup all PDRs based on IP.
 * After that, for each PDR, check the its attribuites with match with access way.
 * - The TEID from GTP GPDU with the TEID stored in PDR.
 * - Source IP from IP header with source address stored in PDI.
 * - Interface from PDI with ACCESS or CORE interface (it depends on if it is UL or DL).
 * After match all field, get the session id in the found PDR.
 *
 * @param p_ctx The user accessible metadata for xdp packet hook.
 * @param p_gtpuh
 * @return u32
 */
static u32 pfcp_pdr_lookup_downlink(struct xdp_md *p_ctx)
{
  pfcp_pdr_t_ *p_pdr;
  pfcp_far_t_ *p_far;
  struct iphdr *p_iph;
  u64 offset;
  u32 dest_ip;
  seid_t_ seid;
  u32 index = 0;
  u32 i;

  void *p_data = (void *)(long)p_ctx->data;
  void *p_data_end = (void *)(long)p_ctx->data_end;

  offset = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);


  if(p_data + offset > p_data_end) {
    bpf_debug("Invalid GTP packet!");
    return XDP_PASS;
  }

  // Get GTP base address.
  p_iph = p_data + sizeof(struct ethhdr);

  dest_ip = p_iph->daddr;
  bpf_debug("Destination IP %d in IPv4 payload received", dest_ip);

  p_pdr = bpf_map_lookup_elem(&m_ueip_pdr, &dest_ip);

  if(!p_pdr) {
    bpf_debug("Error - unsync teid->pdrs map.");
    return XDP_DROP;
  }

  // For each PDR, check parameters.
  if(pfcp_pdr_match_pdi_downlink(&p_pdr[i], (struct iphdr *)p_iph) == 0) {
    // Lets apply the forwarding actions rule.
    p_far = bpf_map_lookup_elem(&m_fars, &p_pdr->far_id.far_id);
    bpf_debug("PDR associated with UP IP %d found! PDR id:%d and FAR id:%d", htonl(p_iph->daddr), p_pdr->pdr_id.rule_id, p_pdr->far_id.far_id);
    return pfcp_far_apply(p_ctx, p_far);
  }

  return XDP_PASS;
}

// Uplink entry point.
SEC("xdp_uplink_entry_point")
int uplink_entry_point(struct xdp_md *p_ctx)
{
  bpf_debug("XDP SESSION CONTEXT - UPLINK");
  u32 action = pfcp_pdr_lookup_uplink(p_ctx);
  return xdp_stats_record_action(p_ctx, action);
}

// Downlink entry point.
SEC("xdp_downlink_entry_point")
int downlink_entry_point(struct xdp_md *p_ctx)
{
  bpf_debug("XDP SESSION CONTEXT - DOWNLINK");
  u32 action = pfcp_pdr_lookup_downlink(p_ctx);
  return xdp_stats_record_action(p_ctx, action);
}

char _license[] SEC("license") = "GPL";
