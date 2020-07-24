#define KBUILD_MODNAME upf_xdp_bpf

#include <protocols/eth.h>
#include <protocols/ip.h>
#include <protocols/udp.h>
#include <protocols/gtpu.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <bpf_helpers.h>
#include <endian.h>
#include <types.h>
#include <utils/logger.h>
#include <utils/utils.h>
#include <maps.h>
#include <pfcp/pfcp_pdr.h>

/////////////////////////////////////////////////////////////////////////
///////////////////// GTP FUNCTIONS /////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
static u32 gtp_handle(struct xdp_md *ctx, struct gtpuhdr *gtpuh)
{
  void *data_end = (void *)(long)ctx->data_end;
  struct iphdr *iph;
  u8 ret;
  pfcp_pdr_t *p_pdr_list;
  pfcp_session_t *p_session;
  u32 *p_pdr_counter;
  u32 *p_session_pdrs_counter;
  teid_t teid;
  seid_t seid;
  u32 index = 0;

  if (gtpuh + 1 > data_end)
  {
    bpf_debug("Invalid GTPU packet\n");
    return XDP_DROP;
  }
  if (gtpuh->message_type != GTPU_G_PDU)
    bpf_debug("Message type 0x%x is not GTPU GPDU(0x%x)\n", gtpuh->message_type, GTPU_G_PDU);

  bpf_debug("GTP GPDU received\n");

  if (!ip_inner_check_ipv4(ctx, (struct iphdr *)(gtpuh + 1)))
    return XDP_DROP;

  teid = htonl(gtpuh->teid);
  bpf_debug("GTP GPDU teid %d with IPv4 payload received\n", teid);

  // Get number of allocated PDR by teid.
  p_pdr_counter = bpf_map_lookup_elem(&m_teid_pdrs_counter, &teid);
  if (!p_pdr_counter || *p_pdr_counter <= 0)
    return XDP_DROP;

  bpf_debug("Number of PDRs found %d by teid %d\n", *p_pdr_counter, teid);
  p_pdr_list = bpf_map_lookup_elem(&m_teid_pdrs, &teid);
  if (!p_pdr_list)
    return XDP_DROP;

  seid = p_pdr_list->local_seid;
  bpf_debug("PDR associated with teid %d found! Rule id is %d and seid is %d\n", teid, p_pdr_list->pdr.pdr_id.rule_id, seid);

  // Get number of PDRs in session.
  p_session_pdrs_counter = bpf_map_lookup_elem(&m_seid_pdrs_counter, &seid);
  if (!p_session_pdrs_counter || *p_session_pdrs_counter <= 0)
    return XDP_DROP;

  bpf_debug("Number of PDRs found %d by seid %d\n", *p_session_pdrs_counter, seid);
  p_session = bpf_map_lookup_elem(&m_seid_session, &seid);

  if (!p_session)
    return XDP_DROP;

  index = 0;
  if (*p_session_pdrs_counter == 0 && *p_session_pdrs_counter > SESSION_PDRS_SIZE)
    return XDP_DROP;

  bpf_debug("Session %d FOUND! Rule id is %d. PDR index %d\n", p_session->seid, p_session->pdrs[index].pdr.pdr_id.rule_id, index);
  return XDP_PASS;
}

/////////////////////////////////////////////////////////////////////////
////////////////////// UDP FUNCTIONS ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
static u32 udp_handle(struct xdp_md *ctx, struct udphdr *udph)
{
  void *data_end = (void *)(long)ctx->data_end;
  u32 dport;
  /* Hint: +1 is sizeof(struct udphdr) */
  if (udph + 1 > data_end)
  {
    bpf_debug("Invalid UDP packet\n");
    return XDP_ABORTED;
  }

  bpf_debug("UDP packet validated\n");
  dport = htons(udph->dest);
  switch (dport)
  {
  case GTP_UDP_PORT:
    return gtp_handle(ctx, (struct gtpuhdr *)(udph + 1));
  default:
    bpf_debug("GTP port %lu not valid\n", dport);
    return XDP_PASS;
  }
}

/////////////////////////////////////////////////////////////////////////
////////////////////// IP FUNCTIONS /////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
static u32 ipv4_handle(struct xdp_md *ctx, struct iphdr *iph)
{
  void *data_end = (void *)(long)ctx->data_end;
  // Type need to match map.
  u32 ip_dest;

  // Hint: +1 is sizeof(struct iphdr)
  if (iph + 1 > data_end)
  {
    bpf_debug("Invalid IPv4 packet\n");
    return XDP_ABORTED;
  }
  ip_dest = iph->daddr;

  bpf_debug("Valid IPv4 packet: raw daddr:0x%x\n", ip_dest);
  switch (iph->protocol)
  {
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
  // Type need to match map.
  u32 ip_dest;

  // Hint: +1 is sizeof(struct iphdr)
  if (iph + 1 > data_end)
  {
    bpf_debug("Invalid IPv4 packet\n");
    return XDP_ABORTED;
  }

  return iph->version == 4;
}

/////////////////////////////////////////////////////////////////////////
///////////////////// ETH FUNCTIONS /////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////
///////////////////// SECTION FUNCTIONS //////////////////////////////////
/////////////////////////////////////////////////////////////////////////
SEC("gtp_process_chain")
int xdp_prog1(struct xdp_md *ctx)
{
  void *data = (void *)(long)ctx->data;
  struct ethhdr *eth = data;

  return eth_handle(ctx, eth);
}

char _license[] SEC("license") = "GPL";