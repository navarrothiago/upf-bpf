#ifndef __MAPS_H__
#define __MAPS_H__

#include <ie/group_ie/create_pdr.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>
#include <linux/bpf.h>

#define MAX_LENGTH 10

struct bpf_map_def SEC("maps") m_seid_session = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(seid_t_), // seid
	.value_size  = sizeof(pfcp_session_t_), // session
	.max_entries = 100000,
};

struct bpf_map_def SEC("maps") m_id_txport = {
	.type        = BPF_MAP_TYPE_DEVMAP,
	.key_size    = sizeof(u32), // id
	.value_size  = sizeof(u32), // tx port
	.max_entries = 10,
};

struct bpf_map_def SEC("maps") m_id_txcnt = {
	.type        = BPF_MAP_TYPE_DEVMAP,
	.key_size    = sizeof(u32), // id
	.value_size  = sizeof(u32), // tx counter
	.max_entries = 1,
};

// Uplink maps.
struct bpf_map_def SEC("maps") m_teid_pdrs_counter = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(teid_t_), // teid
	.value_size  = sizeof(u32), // number of allocated PDR in teid
	.max_entries = 100000,
};

struct bpf_map_def SEC("maps") m_teid_pdrs = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(teid_t_), // teid
	.value_size  = sizeof(pfcp_pdr_t_) * MAX_LENGTH, // list of pdr
	.max_entries = 100000,
};

//  Downlink maps.
struct bpf_map_def SEC("maps") m_ue_ip_pdrs_counter = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(teid_t_), // teid
	.value_size  = sizeof(u32), // number of allocated PDR in teid
	.max_entries = 100000,
};

struct bpf_map_def SEC("maps") m_ue_ip_pdrs = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(u32),   // UE IP
    .value_size = sizeof(u32), // PDRs
    .max_entries = 100000,
};

#endif // __MAPS_H__
