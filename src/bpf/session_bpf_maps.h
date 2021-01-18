#ifndef __SESSION_CONTEXT_MAPS_H__
#define __SESSION_CONTEXT_MAPS_H__

#include <linux/bpf.h>
#include <types.h>
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
struct bpf_map_def SEC("maps") m_id_txport = {
	.type        = BPF_MAP_TYPE_DEVMAP,
	.key_size    = sizeof(u32), // id
	.value_size  = sizeof(u32), // tx port
	.max_entries = 10,
};
struct bpf_map_def SEC("maps") m_seid = {
	.type        = BPF_MAP_TYPE_ARRAY,
	.key_size    = sizeof(u32), // index
	.value_size  = sizeof(seid_t_), // seid
	.max_entries = 1,
};
// Uplink map.
struct bpf_map_def SEC("maps") m_teid_pdrs_counter = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(teid_t_), // teid
	.value_size  = sizeof(u32), // number of allocated PDR in teid
	.max_entries = 100000,
};
// Uplink map.
struct bpf_map_def SEC("maps") m_teid_pdrs = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(teid_t_), // teid
	.value_size  = sizeof(pfcp_pdr_t_) * MAX_LENGTH, // list of pdr
	.max_entries = 100000,
};



#endif // __SESSION_CONTEXT_MAPS_H__