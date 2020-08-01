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

/**
 * PFCP SECTION.
 */

/**
 * @brief Apply forwarding action rules.
 *
 * @param gtpuh
 * @param p_session
 * @return u32
 */
static u32 pfcp_far_apply(struct gtpuhdr *gtpuh, pfcp_session_t *p_session)
{
  u32 index = p_session->fars_counter - 1;
  pfcp_far_t *p_far;

  // Check if it is a valid index.
  if(index >= SESSION_FARS_MAX_SIZE) return XDP_DROP;

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
        break;
      case OUTER_HEADER_CREATION_UDP_IPV6:
        bpf_debug("OUTER_HEADER_CREATION_UDP_IPV6\n");
        break;
      }
    }
  }
  return XDP_PASS;
}

/**
 * @brief Check the PDRs attribuites with match with access way.
 * - The TEID from GTP GPDU with the TEID stored in PDR.
 * - Source IP from IP header with source address stored in PDI.
 * - Interface from PDI with ACCESS or CORE interface (it depends on if it is UL or DL).
 *
 * @param p_pdr
 * @param p_iph
 * @return u8
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
 * @param ctx
 * @param gtpuh
 * @return u32
 */
static u32 pfcp_session_lookup(struct xdp_md *ctx, struct gtpuhdr *gtpuh)
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
  u8 *p_iph = (u8 *)gtpuh + GTPV1U_MSG_HEADER_MIN_SIZE;

  teid = htonl(gtpuh->teid);
  bpf_debug("GTP GPDU teid %d with IPv4 payload received\n", teid);

  // Get number of allocated PDR by teid.
  p_pdr_counter = bpf_map_lookup_elem(&m_teid_pdrs_counter, &teid);

  // Check if there are PDRs. If not, DROP the packet.
  if(!p_pdr_counter || *p_pdr_counter == 0) return XDP_DROP;

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
        if(!p_session) return XDP_DROP;

        // Number of valid PDRs.
        pdrs_counter = p_session->pdrs_counter;
        bpf_debug("Number of PDRs found %d by seid %d\n", pdrs_counter, seid);
        index = pdrs_counter - 1;

        // Check if index is valid.
        if(index >= SESSION_PDRS_MAX_SIZE) return XDP_DROP;

        bpf_debug("Session %d FOUND! Rule id is %d. PDR index %d\n", p_session->seid, p_session->pdrs[index].pdr_id.rule_id, index);

        // Lets apply the forwarding actions rule.
        pfcp_far_apply(gtpuh, p_session);
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
 * @param ctx
 * @param gtpuh
 * @return u32
 */
static u32 gtp_handle(struct xdp_md *ctx, struct gtpuhdr *gtpuh)
{
  void *data_end = (void *)(long)ctx->data_end;

  if((void *)gtpuh + sizeof(*gtpuh) > data_end) {
    bpf_debug("Invalid GTPU packet\n");
    return XDP_DROP;
  }

  // TODO navarrothiago - handle other PDU.
  if(gtpuh->message_type != GTPU_G_PDU) bpf_debug("Message type 0x%x is not GTPU GPDU(0x%x)\n", gtpuh->message_type, GTPU_G_PDU);

  bpf_debug("GTP GPDU received\n");

  if(!ip_inner_check_ipv4(ctx, (struct iphdr *)(gtpuh + 1))) return XDP_DROP;

  return pfcp_session_lookup(ctx, gtpuh);
}

/**
 * UDP SECTION.
 */

/**
 * @brief Handle UDP header.
 *
 * @param ctx
 * @param udph
 * @return u32
 */
static u32 udp_handle(struct xdp_md *ctx, struct udphdr *udph)
{
  void *data_end = (void *)(long)ctx->data_end;
  u32 dport;

  /* Hint: +1 is sizeof(struct udphdr) */
  if((void *)udph + sizeof(*udph) > data_end) {
    bpf_debug("Invalid UDP packet\n");
    return XDP_ABORTED;
  }

  bpf_debug("UDP packet validated\n");
  dport = htons(udph->dest);

  switch(dport) {
  case GTP_UDP_PORT:
    return gtp_handle(ctx, (struct gtpuhdr *)(udph + 1));
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
 * @param ctx
 * @param iph
 * @return u32
 */
static u32 ipv4_handle(struct xdp_md *ctx, struct iphdr *iph)
{
  void *data_end = (void *)(long)ctx->data_end;
  // Type need to match map.
  u32 ip_dest;

  // Hint: +1 is sizeof(struct iphdr)
  if((void *)iph + sizeof(*iph) > data_end) {
    bpf_debug("Invalid IPv4 packet\n");
    return XDP_ABORTED;
  }
  ip_dest = iph->daddr;

  bpf_debug("Valid IPv4 packet: raw daddr:0x%x\n", ip_dest);
  switch(iph->protocol) {
  case IPPROTO_UDP:
    return udp_handle(ctx, (struct udphdr *)(iph + 1));
  case IPPROTO_TCP:
  default:
    return XDP_PASS;
  }
}

static u8 ip_inner_check_ipv4(struct xdp_md *ctx, struct iphdr *iph)
{
  void *data_end = (void *)(long)ctx->data_end;

  // Hint: +1 is sizeof(struct iphdr)
  if((void *)iph + sizeof(*iph) > data_end) {
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
  if((void *)ethh + offset > data_end) {
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
    if(!((void *)ethh + offset > data_end)) eth_type = htons(vlan_hdr->h_vlan_encapsulated_proto);
  case ETH_P_IP:
    return ipv4_handle(ctx, (struct iphdr *)((void *)ethh + offset));
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

SEC("xdp")
int upf_chain(struct xdp_md *ctx)
{
  void *data = (void *)(long)ctx->data;
  struct ethhdr *eth = data;

  return eth_handle(ctx, eth);
}

char _license[] SEC("license") = "GPL";