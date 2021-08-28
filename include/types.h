#if !defined(TYPES_H)
#define TYPES_H

#include <linux/types.h>

typedef __u64 u64;
typedef __s64 s64;

typedef __u32 u32;
typedef __s32 s32;

typedef __u16 u16;
typedef __s16 s16;

typedef __u8 u8;
typedef __s8 s8;

enum FlowDirection { DOWNLINK = 0, UPLINK = 1 };

#define BPF_ANNOTATE_KV_PAIR(name, type_key, type_val)		\
	struct ____btf_map_##name {				\
		type_key key;					\
		type_val value;					\
	};							\
	struct ____btf_map_##name				\
	__attribute__ ((section(".maps." #name), used))		\
		____btf_map_##name = { }

#endif // TYPES_H
