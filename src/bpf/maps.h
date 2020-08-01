#ifndef __MAPS_H__
#define __MAPS_H__

#include <ie/group_ie/create_pdr.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>
#include <linux/bpf.h> 

#define MAX_LENGTH 10

struct bpf_map_def SEC("maps") m_teid_pdrs_counter = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(teid_t), // teid 
	.value_size  = sizeof(u32), // number of allocated PDR in teid 
	.max_entries = 100000,
};

struct bpf_map_def SEC("maps") m_teid_pdrs = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(teid_t), // teid 
	.value_size  = sizeof(pfcp_pdr_t) * MAX_LENGTH, // list of pdr 
	.max_entries = 100000,
};

struct bpf_map_def SEC("maps") m_seid_session = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(seid_t), // seid 
	.value_size  = sizeof(pfcp_session_t), // session 
	.max_entries = 100000,
};


#endif // __MAPS_H__