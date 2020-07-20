#ifndef __MAPS_H__
#define __MAPS_H__

#include <ie/group_ie/create_pdr.h>
#include <linux/bpf.h> 

struct bpf_map_def SEC("maps") seid_pdr_map = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(teid_t), // teid 
	.value_size  = sizeof(create_pdr_t), 
	.max_entries = 100000,
};

#endif // __MAPS_H__