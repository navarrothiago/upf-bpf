#ifndef __PROGRAM_DEFINES_H__
#define __PROGRAM_DEFINES_H__

#include <linux/bpf.h>
#include <types.h>

#define XDP_PROGRAM_UPF_PROGRAM 1
#define XDP_PROGRAM_SESSION_CONTEXT 2

struct bpf_map_def SEC("maps") m_jmp_table = {
	.type        = BPF_MAP_TYPE_PROG_ARRAY,
	.key_size    = sizeof(u32), // program identifier. 
	.value_size  = sizeof(u32), // program.
	.max_entries = 8,
};

struct bpf_map_def SEC("maps") m_far_id_far = {
	.type        = BPF_MAP_TYPE_HASH,
	.key_size    = sizeof(u32), // teid
	.value_size  = sizeof(u32), // list of pdr
	.max_entries = 10,
};

#endif // __PROGRAM_DEFINES_H__