#ifndef __FAR_MAPS_H__
#define __FAR_MAPS_H__

#include <bpf_helpers.h>
#include <linux/bpf.h>
#include <pfcp/pfcp_far.h>
#include <types.h>

// The unique FAR that will be consumed in this program.
struct bpf_map_def SEC("maps") m_far = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(u8),
    .value_size = sizeof(pfcp_far_t_),
    .max_entries = 1,
};
struct bpf_map_def SEC("maps") m_redirect_interfaces = {
	.type        = BPF_MAP_TYPE_DEVMAP,
	.key_size    = sizeof(u32), // id
	.value_size  = sizeof(u32), // tx port
	.max_entries = 10,
};
// Static ARP Table. Used to get the MAC address of the next hop.
// TODO navarrothiago - pinned this maps. It not depend on the session program
struct bpf_map_def SEC("maps") m_arp_table = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(u32), // IPv4 address
	.value_size  = 6, // MAC address
	.max_entries = 2,
};

BPF_ANNOTATE_KV_PAIR(m_far, u8, pfcp_far_t_);
BPF_ANNOTATE_KV_PAIR(m_redirect_interfaces, u32, u32);
// BPF_ANNOTATE_KV_PAIR(m_arp_table, u32, ??);
#endif // __FAR_MAPS_H__
