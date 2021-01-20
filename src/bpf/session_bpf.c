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
#include <session_bpf_maps.h>
#include <utils/logger.h>
#include <utils/utils.h>
#include <endian.h>

// TODO navarrothiago - Put dummy in test folder.
/**
 * @brief Redirect require an XDP bpf_prog loaded on the TX device.
 */
SEC("xdp_redirect_dummy")
int xdp_redirect_gtpu(struct xdp_md *p_ctx)
{
  // PASS.
  bpf_debug("Redirecting packets");
  return XDP_PASS;
}

/**
 * @brief Apply forwarding action rules.
 *
 * @param p_gtpuh The GTPU header.
 * @param p_session The session of this context.
 * @return u32 XDP action.
 */

static u32 pfcp_far_apply(pfcp_far_t_ *p_far)
{
  // TODO dupl
  // TODO nocp
  // TODO buff

  // Check if it is a forward action.
  if(!p_far){
    bpf_debug("Invalid FAR!");
    return XDP_DROP;
  }

  if(p_far->apply_action.forw) {
    if(p_far->forwarding_parameters.destination_interface.interface_value == INTERFACE_VALUE_CORE) {
      bpf_debug("Destination is to INTERFACE_VALUE_ACCESS");
      // Check Outer header creation - IPv4 or IPv6
      switch(p_far->forwarding_parameters.outer_header_creation.outer_header_creation_description) {
      case OUTER_HEADER_CREATION_UDP_IPV4:
        bpf_debug("OUTER_HEADER_CREATION_UDP_IPV4");
        // swap_src_dst_mac(p_data);
        return bpf_redirect_map(&m_redirect_interfaces, 0, 0);
        break;
      case OUTER_HEADER_CREATION_UDP_IPV6:
        bpf_debug("OUTER_HEADER_CREATION_UDP_IPV6");
        // TODO navarrothiago
        break;
      }
    } else{
      bpf_debug("Destination is not INTERFACE_VALUE_ACCESS");
    }
  }else{
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
  if(!p_iph){
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
static u32 pfcp_pdr_lookup(struct xdp_md *p_ctx)
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
    return pfcp_far_apply(p_far);
  }

  return XDP_PASS;
}

// Simple XDP BPF program. Everything packet will be dropped.
SEC("xdp_entry_point")
int entry_point(struct xdp_md *p_ctx)
{
  bpf_debug("XDP SESSION CONTEXT");
  u32 action = xdp_stats_record_action(p_ctx, pfcp_pdr_lookup(p_ctx));
  return XDP_PASS;
}

char _license[] SEC("license") = "GPL";