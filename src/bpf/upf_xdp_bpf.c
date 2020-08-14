#define KBUILD_MODNAME upf_xdp_bpf

// clang-format off
#include <types.h>
// clang-format on
#include <bpf_helpers.h>
#include <endian.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <maps.h>
#include <pfcp/pfcp_far.h>
#include <pfcp/pfcp_pdr.h>
#include <protocols/eth.h>
#include <protocols/gtpu.h>
#include <protocols/ip.h>
#include <protocols/udp.h>
#include <utils/logger.h>
#include <utils/utils.h>

/* Defines xdp_stats_map */
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
static u32 pfcp_far_apply(struct xdp_md *p_ctx, struct gtpuhdr *p_gtpuh, pfcp_session_t *p_session)
{
  u32 index = p_session->fars_counter - 1;
  pfcp_far_t *p_far;
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
 * @brief Match the PDRs attribuites accoding to data flow (UP, DL).
 * - The TEID from GTP GPDU with the TEID stored in PDR.
 * - Source IP from IP header with source address stored in PDI.
 * - Interface from PDI with ACCESS or CORE interface (it depends on if it is UL or DL).
 *
 * @param p_pdr The PDR to be match with the header.
 * @param p_iph The IP header.
 * @return u8 True if match. False cc.
 */
static u32 pfcp_pdr_match_attribute(pfcp_pdr_t *p_pdr, struct iphdr *p_iph, teid_t teid)
{
  // clang-format off
  if(p_pdr->outer_header_removal.outer_header_removal_description != OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4
      || p_pdr->pdi.source_interface.interface_value != INTERFACE_VALUE_ACCESS
      || p_pdr->pdi.fteit.teid != teid
      || p_pdr->pdi.ue_ip_address.ipv4_address != p_iph->saddr
    ){
        bpf_debug("Not match:\n");
        bpf_debug("OHRD: %d\n", OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4 );
        bpf_debug("OHRD: %d\n", p_pdr->outer_header_removal.outer_header_removal_description);
        bpf_debug("Interface: %d\n", INTERFACE_VALUE_ACCESS);
        bpf_debug("Interface: %d\n", p_pdr->pdi.source_interface.interface_value);
        bpf_debug("TEID: %d\n", teid);
        bpf_debug("TEID: %d\n", p_pdr->pdi.fteit.teid);
        bpf_debug("IPv4: %d\n", p_iph->saddr);
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
  pfcp_pdr_t *p_pdr_list;
  pfcp_session_t *p_session;
  u32 *p_pdr_counter;
  u32 pdrs_counter;
  teid_t teid;
  seid_t seid;
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
      if(pfcp_pdr_match_attribute(&p_pdr_list[i], (struct iphdr *)p_iph, teid) == 0) {
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
 * GTP SECTION.
 */

/**
 * @brief Check if GTP packet is a GPDU. If so, process the next block chain.
 *
 * @param p_ctx The user accessible metadata for xdp packet hook.
 * @param p_gtpuh The GTP header.
 * @return u32 The XDP action.
 */
static u32 gtp_handle(struct xdp_md *p_ctx, struct gtpuhdr *p_gtpuh)
{
  void *p_data_end = (void *)(long)p_ctx->data_end;

  if((void *)p_gtpuh + sizeof(*p_gtpuh) > p_data_end) {
    bpf_debug("Invalid GTPU packet\n");
    return XDP_DROP;
  }

  // TODO navarrothiago - handle other PDU.
  if(p_gtpuh->message_type != GTPU_G_PDU) {
    bpf_debug("Message type 0x%x is not GTPU GPDU(0x%x)\n", p_gtpuh->message_type, GTPU_G_PDU);
    return XDP_PASS;
  }

  bpf_debug("GTP GPDU received\n");

  if(!ip_inner_check_ipv4(p_ctx, (struct iphdr *)(p_gtpuh + 1))) {
    bpf_debug("Invalid IP inner");
    return XDP_DROP;
  }

  return pfcp_session_lookup(p_ctx, p_gtpuh);
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
static u32 udp_handle(struct xdp_md *p_ctx, struct udphdr *udph)
{
  void *p_data_end = (void *)(long)p_ctx->data_end;
  u32 dport;

  /* Hint: +1 is sizeof(struct udphdr) */
  if((void *)udph + sizeof(*udph) > p_data_end) {
    bpf_debug("Invalid UDP packet\n");
    return XDP_ABORTED;
  }

  bpf_debug("UDP packet validated\n");
  dport = htons(udph->dest);

  switch(dport) {
  case GTP_UDP_PORT:
    return gtp_handle(p_ctx, (struct gtpuhdr *)(udph + 1));
  default:
    bpf_debug("GTP port %lu not valid\n", dport);
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
    bpf_debug("Invalid IPv4 packet\n");
    return XDP_ABORTED;
  }
  ip_dest = iph->daddr;

  bpf_debug("Valid IPv4 packet: raw daddr:0x%x\n", ip_dest);
  switch(iph->protocol) {
  case IPPROTO_UDP:
    return udp_handle(p_ctx, (struct udphdr *)(iph + 1));
  case IPPROTO_TCP:
  default:
    bpf_debug("TCP protocol L4\n");
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
  u32 action = xdp_stats_record_action(p_ctx, eth_handle(p_ctx, eth));
  bpf_debug("Action %d\n", action);
  return action;
}

char _license[] SEC("license") = "GPL";
