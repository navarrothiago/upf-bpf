#ifndef __SESSION_CONTEXT_MAPS_H__
#define __SESSION_CONTEXT_MAPS_H__

#include <linux/bpf.h>
#include <types.h>

struct bpf_map_def SEC("maps") m_far_id_far = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(u32), // teid
	.value_size  = sizeof(u32), // list of pdr
	.max_entries = 10,
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



#endif // __SESSION_CONTEXT_MAPS_H__