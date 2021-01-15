#define KBUILD_MODNAME upf_xdp_bpf

// clang-format off
#include <types.h>
// clang-format on
#include <bpf_helpers.h>
#include <endian.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <maps.h>
#include <pfcp/pfcp_far.h>
#include <pfcp/pfcp_pdr.h>
#include <protocols/eth.h>
#include <protocols/gtpu.h>
#include <protocols/ip.h>
#include <protocols/udp.h>
#include <utils/logger.h>
#include <utils/utils.h>

#ifdef KERNEL_SPACE
#include <linux/in.h>
#else
#include <netinet/in.h>
#endif

// Defines xdp_stats_map
#include "xdp_stats_kern_user.h"
#include "xdp_stats_kern.h"

// TODO navarrothiago - Put dummy in test folder.
/**
 * @brief Redirect require an XDP bpf_prog loaded on the TX device.
 */
SEC("xdp_redirect_dummy")
int xdp_redirect_gtpu(struct xdp_md *p_ctx)
{
  // PASS.
  bpf_debug("Redirecting packets\n");
  return XDP_PASS;
}

/**
 * PFCP SECTION.
 */

/**
 * @brief Apply forwarding action rules.
 *
 * @param p_gtpuh The GTPU header.
 * @param p_session The session of this context.
 * @return u32 XDP action.
 */
static u32 pfcp_far_apply(struct xdp_md *p_ctx, struct gtpuhdr *p_gtpuh, pfcp_session_t_ *p_session)
{
  u32 index = p_session->fars_counter - 1;
  pfcp_far_t_ *p_far;
  void *p_data_end = (void *)(long)p_ctx->data_end;
  void *p_data = (void *)(long)p_ctx->data;
  struct ethhdr *eth = p_ctx->data;
  u64 nh_off;

  nh_off = sizeof(*eth);
  if(p_data + nh_off > p_data_end)
    return XDP_DROP;

  // Check if it is a valid index.
  if(index >= SESSION_FARS_MAX_SIZE)
    return XDP_DROP;

  // Get FAR from the selected session.
  p_far = &p_session->fars[index];

  // TODO dupl
  // TODO nocp
  // TODO buff

  // Check if it is a forward action.
  if(p_far->apply_action.forw) {
    if(p_far->forwarding_parameters.destination_interface.interface_value == INTERFACE_VALUE_ACCESS) {
      // Check Outer header creation - IPv4 or IPv6
      switch(p_far->forwarding_parameters.outer_header_creation.outer_header_creation_description) {
      case OUTER_HEADER_CREATION_UDP_IPV4:
        bpf_debug("OUTER_HEADER_CREATION_UDP_IPV4\n");
        // swap_src_dst_mac(p_data);
        return bpf_redirect_map(&m_id_txport, 0, 0);
        break;
      case OUTER_HEADER_CREATION_UDP_IPV6:
        bpf_debug("OUTER_HEADER_CREATION_UDP_IPV6\n");
        // TODO navarrothiago
        break;
      }
    }
  }
  return XDP_PASS;
}

/**
 * @brief Match the PDRs attribuites for DL data flow.
 * - Source IP from IP header with source address stored in PDI.
 * - Interface from PDI with ACCESS interface value.
 *
 * @param p_pdr The PDR to be match with the header.
 * @param p_iph The IP header.
 * @return u8 True if match. False cc.
 */
static u32 pfcp_pdr_match_pdi_core(pfcp_pdr_t_ *p_pdr, struct iphdr *p_iph)
{
  // clang-format off
  if(
      // TODO navarrothiago - Think a way to check is there is this IEs.
      // p_pdr->outer_header_removal.outer_header_removal_description != OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4
      p_pdr->pdi.source_interface.interface_value != INTERFACE_VALUE_CORE
      || p_pdr->pdi.ue_ip_address.ipv4_address != p_iph->daddr
    ){
        bpf_debug("Not match:\n");
        // bpf_debug("OHRD: %d\n", OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4 );
        // bpf_debug("OHRD: %d\n", p_pdr->outer_header_removal.outer_header_removal_description);
        bpf_debug("Interface: %d\n", INTERFACE_VALUE_CORE);
        bpf_debug("Interface: %d\n", p_pdr->pdi.source_interface.interface_value);
        bpf_debug("IPv4: %d\n", p_iph->daddr);
        bpf_debug("IPv4: %d\n", p_pdr->pdi.ue_ip_address.ipv4_address);
        return 1;
    }
  // clang-format on

  // All the attributes were matched.
  bpf_debug("All atrributes were matched!!\n");
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
static u32 pfcp_session_lookup(struct xdp_md *p_ctx, struct gtpuhdr *p_gtpuh)
{
  pfcp_pdr_t_ *p_pdr_list;
  pfcp_session_t_ *p_session;
  u32 *p_pdr_counter;
  u32 pdrs_counter;
  teid_t_ teid;
  seid_t_ seid;
  u32 index = 0;
  u32 i;

  // We have already assumed that the packet is a GPDU.
  u8 *p_iph = (u8 *)p_gtpuh + GTPV1U_MSG_HEADER_MIN_SIZE;

  teid = htonl(p_gtpuh->teid);
  bpf_debug("GTP GPDU teid %d with IPv4 payload received\n", teid);

  // Get number of allocated PDR by teid.
  p_pdr_counter = bpf_map_lookup_elem(&m_teid_pdrs_counter, &teid);

  // Check if there are PDRs. If not, DROP the packet.
  if(!p_pdr_counter || *p_pdr_counter == 0) {
    bpf_debug("Invalid counter");
    return XDP_DROP;
  }

// For each PDR, check the its attributes.
#pragma unroll
  for(i = 0; i < MAX_LENGTH; i++) {

    // Unbounded condition in for control is not supported.
    if(i < *p_pdr_counter) {
      bpf_debug("Number of PDRs found %d by teid %d\n", *p_pdr_counter, teid);
      p_pdr_list = bpf_map_lookup_elem(&m_teid_pdrs, &teid);

      if(!p_pdr_list) {
        bpf_debug("Error - unsync counter (= %d) and teid->pdrs map.", p_pdr_counter);
        return XDP_DROP;
      }

      // For each PDR, check parameters.
      if(pfcp_pdr_match_pdi_access(&p_pdr_list[i], (struct iphdr *)p_iph, teid) == 0) {
        seid = p_pdr_list[i].local_seid;
        bpf_debug("PDR associated with teid %d found! Rule id is %d and seid is %d\n", teid, p_pdr_list->pdr_id.rule_id, seid);

        // Get session based on seid.
        p_session = bpf_map_lookup_elem(&m_seid_session, &seid);

        // Check if the session is valid.
        if(!p_session) {
          bpf_debug("Invalid session");
          return XDP_DROP;
        }

        // Number of valid PDRs.
        pdrs_counter = p_session->pdrs_counter;
        bpf_debug("Number of PDRs found %d by seid %d\n", pdrs_counter, seid);
        index = pdrs_counter - 1;

        // Check if index is valid.
        if(index >= SESSION_PDRS_MAX_SIZE) {
          bpf_debug("Index greater than the maximum");
          return XDP_DROP;
        }

        bpf_debug("Session %d FOUND! Rule id is %d. PDR index %d\n", p_session->seid, p_session->pdrs[index].pdr_id.rule_id, index);

        // Lets apply the forwarding actions rule.
        return pfcp_far_apply(p_ctx, p_gtpuh, p_session);
      }
    }
  }
  return XDP_PASS;
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
  void *p_data = (void *)(long)p_ctx->data;
  void *p_data_end = (void *)(long)p_ctx->data_end;
  // Type need to match map.
  u32 ue_ip_address;
  u32 *p_pdr_counter;
  u32 *p_far_counter;
  pfcp_pdr_t_ *p_pdr_list;
  struct ethhdr *p_eth;
  struct iphdr *p_ip;
  seid_t_ seid;
  pfcp_session_t_ *p_session;
  int i, j;

  // Hint: +1 is sizeof(struct iphdr)
  if((void *)iph + sizeof(*iph) > p_data_end) {
    bpf_debug("Invalid IPv4 packet\n");
    return XDP_ABORTED;
  }
  ue_ip_address = iph->daddr;

  // Get number of allocated PDR by teid.
  p_pdr_counter = bpf_map_lookup_elem(&m_ue_ip_pdrs_counter, &ue_ip_address);

  // Check if there are PDRs. If not, DROP the packet.
  if(!p_pdr_counter || *p_pdr_counter == 0) {
    bpf_debug("Invalid counter or Empty PDR DL");
    return XDP_DROP;
  }

  // Get PDR list based on UE IP address (PDI).
  // TODO navarrothiago - Check other match field (e.g. SDF).
  p_pdr_list = bpf_map_lookup_elem(&m_ue_ip_pdrs, &ue_ip_address);

  if(!p_pdr_list) {
    bpf_debug("Error - unsync counter (= %d) and ue_ip->pdrs map.", p_pdr_counter);
    return XDP_DROP;
  }

  // For each PDR, check the its attributes.
#pragma unroll
  for(i = 0; i < MAX_LENGTH; i++) {

      // For each PDR, check parameters.
      if(pfcp_pdr_match_pdi_core(&p_pdr_list[i], p_ip) == 0) {
        seid = p_pdr_list[i].local_seid;
        bpf_debug("PDR associated with UE IP %d found! Rule id is %d and seid is %d\n", ue_ip_address, p_pdr_list->pdr_id.rule_id, seid);

        // Get session based on seid.
        p_session = bpf_map_lookup_elem(&m_seid_session, &seid);

        // Check if the session is valid.
        if(!p_session) {
          bpf_debug("Invalid session");
          return XDP_DROP;
        }

  #pragma unroll
  for(j = 0; j < MAX_LENGTH; j++) {
  }
        // Number of valid FARs.
        p_far_counter = p_session->fars_counter;
        bpf_debug("Number of PDRs found %d by seid %d\n", pdrs_counter, seid);
        index = pdrs_counter - 1;

        // Check if index is valid.
        if(index >= SESSION_PDRS_MAX_SIZE) {
          bpf_debug("Index greater than the maximum");
          return XDP_DROP;
        }

        bpf_debug("Session %d FOUND! Rule id is %d. PDR index %d\n", p_session->seid, p_session->pdrs[index].pdr_id.rule_id, index);

        // Lets apply the forwarding actions rule.
        return pfcp_far_apply(p_ctx, p_gtpuh, p_session);
      }
    }
  }

  // KISS - Lets start using the first PDR (high priority).
  // Resize the header in order to put the GTP/UPD/IP headers.
  bpf_xdp_adjust_head(p_ctx, (int32_t)-GTP_ENCAPSULATED_SIZE);

  // Packet buffer changed, all pointers need to be recomputed
  p_data = (void *)(long)p_ctx->data;
  p_data_end = (void *)(long)p_ctx->data_end;

  p_eth = p_data;
  if ((void *)(p_eth + 1) > p_data_end) {
    return XDP_DROP;
  }

 // Space allocated before packet buffer, move eth header
  struct ethhdr *p_orig_eth = p_data + GTP_ENCAPSULATED_SIZE;
  if ((void *)(p_orig_eth + 1) > p_data_end) {
    return XDP_DROP;
  }
  memcpy(p_eth, p_orig_eth, sizeof(*p_eth));

  p_ip = (void *)(p_eth + 1);
  if ((void *)(p_ip + 1) > p_data_end) {
    return XDP_DROP;
  }

  struct iphdr *p_inner_ip = (void *)p_ip + GTP_ENCAPSULATED_SIZE;
  if ((void *)(p_inner_ip + 1) > p_data_end) {
    return XDP_DROP;
  }

  // Add the outer IP header
  p_ip->version = 4;
  p_ip->ihl = 5;  // No options
  p_ip->tos = 0;
  p_ip->tot_len = htons(ntohs(p_inner_ip->tot_len) + GTP_ENCAPSULATED_SIZE);
  p_ip->id = 0;  // No fragmentation
  p_ip->frag_off = 0x0040;  // Don't fragment; Fragment offset = 0
  p_ip->ttl = 64;
  p_ip->protocol = IPPROTO_UDP;
  p_ip->check = 0;
  p_ip->saddr = LOCAL_IP;
  p_ip->daddr = p_pdr_list[0].;

  // Add the UDP header
  struct udphdr *udp = (void *)(ip + 1);
  if ((void *)(udp + 1) > data_end) {
    return RX_DROP;
  }
  udp->source = htons(GTP_PORT);
  udp->dest = htons(GTP_PORT);
  udp->len = htons(ntohs(inner_ip->tot_len) +
             sizeof(*udp) + sizeof(struct gtp1_header));
  udp->check = 0;

  // Add the GTP header
  struct gtp1_header *gtp = (void *)(udp + 1);
  if ((void *)(gtp + 1) > data_end) {
    return RX_DROP;
  }
  gtp->flags = GTP_FLAGS;
  gtp->type = GTP_TYPE_GPDU;
  gtp->length = inner_ip->tot_len;
  gtp->tid = md->traffic_class;

  // Compute l3 checksum
  __wsum l3sum = pcn_csum_diff(0, 0, (__be32 *)ip, sizeof(*ip), 0);
  pcn_l3_csum_replace(ctx, IP_CSUM_OFFSET, 0, l3sum, 0);

  return RX_OK
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
    bpf_debug("Invalid IPv4 packet\n");
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


// TODO navarrothiago - Check the MAC address based on configuration file.
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
    bpf_debug("Cannot parse L2\n");
    return XDP_PASS;
  }

  eth_type = htons(ethh->h_proto);
  bpf_debug("Debug: eth_type:0x%x\n", eth_type);

  switch(eth_type) {
  case ETH_P_8021Q:
  case ETH_P_8021AD:
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
    bpf_debug("Cannot parse L2: L3off:%llu proto:0x%x\n", offset, eth_type);
    return XDP_PASS;
  }
}

SEC("xdp_redirect_map")
int upf_chain(struct xdp_md *p_ctx)
{
  void *p_data = (void *)(long)p_ctx->data;
  struct ethhdr *eth = p_data;

  // Start to handle the ethernet header.
  u32 action = xdp_stats_record_action(p_ctx, eth_handle(p_ctx, eth));
  bpf_debug("Action %d\n", action);

  return action;
}

char _license[] SEC("license") = "GPL";
