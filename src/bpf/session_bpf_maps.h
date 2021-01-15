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

#endif // __SESSION_CONTEXT_MAPS_H__