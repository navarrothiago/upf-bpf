#include <linux/bpf.h>
#include <bpf_helpers.h>
#include <session_bpf_maps.h>
#include <types.h>
#include <pfcp/pfcp_far.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>
#include <protocols/eth.h>
#include <utils/logger.h>
// #include <protocols/gtpu.h>
#include <protocols/ip.h>
// #include <protocols/udp.h>

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

// /**
//  * @brief Apply forwarding action rules.
//  *
//  * @param p_gtpuh The GTPU header.
//  * @param p_session The session of this context.
//  * @return u32 XDP action.
//  */

// // FIXME (navarro) - Actualy, we have to receive the FAR of the PDR that was matched.
// static u32 pfcp_far_apply(struct xdp_md *p_ctx, struct gtpuhdr *p_gtpuh, pfcp_session_t_ *p_session)
// {
//   u32 index = p_session->fars_counter - 1;
//   pfcp_far_t_ *p_far;
//   void *p_data_end = (void *)(long)p_ctx->data_end;
//   void *p_data = (void *)(long)p_ctx->data;
//   struct ethhdr *eth = p_ctx->data;
//   u64 nh_off;

//   nh_off = sizeof(*eth);
//   if(p_data + nh_off > p_data_end)
//     return XDP_DROP;

//   // Check if it is a valid index.
//   if(index >= SESSION_FARS_MAX_SIZE)
//     return XDP_DROP;

//   // Get FAR from the selected session.
//   p_far = &p_session->fars[index];

//   // TODO dupl
//   // TODO nocp
//   // TODO buff

//   // Check if it is a forward action.
//   if(p_far->apply_action.forw) {
//     if(p_far->forwarding_parameters.destination_interface.interface_value == INTERFACE_VALUE_ACCESS) {
//       // Check Outer header creation - IPv4 or IPv6
//       switch(p_far->forwarding_parameters.outer_header_creation.outer_header_creation_description) {
//       case OUTER_HEADER_CREATION_UDP_IPV4:
//         bpf_debug("OUTER_HEADER_CREATION_UDP_IPV4\n");
//         // swap_src_dst_mac(p_data);
//         return bpf_redirect_map(&m_id_txport, 0, 0);
//         break;
//       case OUTER_HEADER_CREATION_UDP_IPV6:
//         bpf_debug("OUTER_HEADER_CREATION_UDP_IPV6\n");
//         // TODO navarrothiago
//         break;
//       }
//     }
//   }
//   return XDP_PASS;
// }

// /**
//  * @brief Match the PDRs attribuites for UL data flow.
//  * - The TEID from GTP GPDU with the TEID stored in PDR.
//  * - Source IP from IP header with source address stored in PDI.
//  * - Interface from PDI with ACCESS interface value.
//  *
//  * @param p_pdr The PDR to be match with the header.
//  * @param p_iph The IP header.
//  * @return u8 True if match. False cc.
//  */
// static u32 pfcp_pdr_match_pdi_access(pfcp_pdr_t_ *p_pdr, struct iphdr *p_iph, teid_t_ teid)
// {
//   // clang-format off
//   if(p_pdr->outer_header_removal.outer_header_removal_description != OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4
//       || p_pdr->pdi.source_interface.interface_value != INTERFACE_VALUE_ACCESS
//       || p_pdr->pdi.fteid.teid != teid
//       || p_pdr->pdi.ue_ip_address.ipv4_address != p_iph->saddr
//     ){
//         bpf_debug("Not match:\n");
//         bpf_debug("OHRD: %d\n", OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4 );
//         bpf_debug("OHRD: %d\n", p_pdr->outer_header_removal.outer_header_removal_description);
//         bpf_debug("Interface: %d\n", INTERFACE_VALUE_ACCESS);
//         bpf_debug("Interface: %d\n", p_pdr->pdi.source_interface.interface_value);
//         bpf_debug("TEID: %d\n", teid);
//         bpf_debug("TEID: %d\n", p_pdr->pdi.fteid.teid);
//         bpf_debug("IPv4: %d\n", p_iph->saddr);
//         bpf_debug("IPv4: %d\n", p_pdr->pdi.ue_ip_address.ipv4_address);
//         return 1;
//     }
//   // clang-format on

//   // All the attributes were matched.
//   bpf_debug("All atrributes were matched!!\n");
//   return 0;
// }

// static u32 pfcp_far_lookup(struct xdp_md *p_ctx, far_id_t_ far_id, pfcp_session_t_ *p_session)
// {
//   bpf_debug("FAR lookup\n");
//   // bpf_tail_call(p_ctx, &m_jmp_table, XDP_PROGRAM_SESSION_CONTEXT);
// }

// /**
//  * @brief Lookup all PDRs based on teid.
//  * After that, for each PDR, check the its attribuites with match with access way.
//  * - The TEID from GTP GPDU with the TEID stored in PDR.
//  * - Source IP from IP header with source address stored in PDI.
//  * - Interface from PDI with ACCESS or CORE interface (it depends on if it is UL or DL).
//  * After match all field, get the session id in the found PDR.
//  *
//  * @param p_ctx The user accessible metadata for xdp packet hook.
//  * @param p_gtpuh
//  * @return u32
//  */
// static u32 pfcp_session_lookup(struct xdp_md *p_ctx, struct gtpuhdr *p_gtpuh)
// {
//   pfcp_pdr_t_ *p_pdr_list;
//   pfcp_session_t_ *p_session;
//   u32 *p_pdr_counter;
//   u32 pdrs_counter;
//   teid_t_ teid;
//   seid_t_ seid;
//   u32 index = 0;
//   u32 i;

//   // We have already assumed that the packet is a GPDU.
//   u8 *p_iph = (u8 *)p_gtpuh + GTPV1U_MSG_HEADER_MIN_SIZE;

//   teid = htonl(p_gtpuh->teid);
//   bpf_debug("GTP GPDU teid %d with IPv4 payload received\n", teid);

//   // Get number of allocated PDR by teid.
//   p_pdr_counter = bpf_map_lookup_elem(&m_teid_pdrs_counter, &teid);

//   // Check if there are PDRs. If not, DROP the packet.
//   if(!p_pdr_counter || *p_pdr_counter == 0) {
//     bpf_debug("Invalid counter");
//     return XDP_DROP;
//   }

// // For each PDR, check the its attributes.
// #pragma unroll
//   for(i = 0; i < MAX_LENGTH; i++) {

//     // Unbounded condition in for control is not supported.
//     if(i < *p_pdr_counter) {
//       bpf_debug("Number of PDRs found %d by teid %d\n", *p_pdr_counter, teid);
//       p_pdr_list = bpf_map_lookup_elem(&m_teid_pdrs, &teid);

//       if(!p_pdr_list) {
//         bpf_debug("Error - unsync counter (= %d) and teid->pdrs map.", p_pdr_counter);
//         return XDP_DROP;
//       }

//       // For each PDR, check parameters.
//       if(pfcp_pdr_match_pdi_access(&p_pdr_list[i], (struct iphdr *)p_iph, teid) == 0) {
//         seid = p_pdr_list[i].local_seid;
//         bpf_debug("PDR associated with teid %d found! PDR id is %d and seid is %d\n", teid, p_pdr_list->pdr_id.rule_id, seid);

//         // Get session based on SEID.
//         p_session = bpf_map_lookup_elem(&m_seid_session, &seid);

//         // Check if the session is valid.
//         if(!p_session) {
//           bpf_debug("Invalid session");
//           return XDP_DROP;
//         }

//         // Number of valid PDRs.
//         pdrs_counter = p_session->pdrs_counter;
//         bpf_debug("Number of PDRs found %d by seid %d\n", pdrs_counter, seid);
//         index = pdrs_counter - 2;

//         // Check if index is valid.
//         if(index >= SESSION_PDRS_MAX_SIZE) {
//           bpf_debug("Index greater than the maximum");
//           return XDP_DROP;
//         }

//         bpf_debug("Session %d FOUND! Rule id is %d. PDR index %d\n", p_session->seid, p_session->pdrs[index].pdr_id.rule_id, index);

//         // Lets apply the forwarding actions rule.
//         return pfcp_far_apply(p_ctx, p_gtpuh, p_session);
//       }
//     }
//   }
//   return XDP_PASS;
// }

// Simple XDP BPF program. Everything packet will be dropped.
SEC("xdp_entry_point")
int entry_point(struct xdp_md *ctx){
  char drop_message[] = "XDP SESSION CONTEXT %d\n";
  u32 *value;
  u32 key = 0;

  bpf_trace_printk(&drop_message, sizeof(drop_message), *value);
  
  return XDP_PASS;
}

char _license[] SEC("license") = "GPL";