#ifndef __SESSION_CONTEXT_MAPS_H__
#define __SESSION_CONTEXT_MAPS_H__

#include <vmlinux.h>

#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_far.h>
#include <ie/fteid.h>
#include <ie/fseid.h>

#define MAX_LENGTH 10
#define SESSION_PDRS_MAX_SIZE 10
#define SESSION_FARS_MAX_SIZE 10

struct bpf_map_def SEC("maps") m_pdrs = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(u16), // pdr_id
	.value_size  = sizeof(pfcp_pdr_t_), // pdr
	.max_entries = SESSION_PDRS_MAX_SIZE,
};
struct bpf_map_def SEC("maps") m_fars = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(u32), // teid
	.value_size  = sizeof(pfcp_far_t_), // list of pdr
	.max_entries = SESSION_FARS_MAX_SIZE,
};
struct bpf_map_def SEC("maps") m_redirect_interfaces = {
	.type        = BPF_MAP_TYPE_DEVMAP,
	.key_size    = sizeof(u32), // id
	.value_size  = sizeof(u32), // tx port
	.max_entries = 10,
};
// Uplink map.
struct bpf_map_def SEC("maps") m_teid_pdrs_counter = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(teid_t_), // teid
	.value_size  = sizeof(u32), // number of allocated PDR in teid
	.max_entries = 100000,
};
// Uplink map.
// TODO navarrothiago - Store multiple PDR.
struct bpf_map_def SEC("maps") m_teid_pdr = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(teid_t_), // teid
	.value_size  = sizeof(pfcp_pdr_t_), // assuming only one PDR
	.max_entries = 100000,
};
// Downlink map.
// TODO navarrothiago - Store multiple PDR.
struct bpf_map_def SEC("maps") m_ueip_pdr = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(u32), // UE IP address
	.value_size  = sizeof(pfcp_pdr_t_), // assuming only one PDR
	.max_entries = 100000,
};

// Static ARP Table. Used to get the MAC address of the next hop.
// TODO navarrothiago - pinned this maps. It not depend on the session program
struct bpf_map_def SEC("maps") m_arp_table = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(u32), // IPv4 address
	.value_size  = 6, // MAC address
	.max_entries = 2,
};
#endif // __SESSION_CONTEXT_MAPS_H__
