#ifndef __MAPS_H__
#define __MAPS_H__

#include <ie/group_ie/create_pdr.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>
#include <linux/bpf.h>
#include <stdint.h>

#define MAX_LENGTH 10

// Maps TEID to SessionProgram
struct bpf_map_def SEC("maps") m_teid_session = {
  .type        = BPF_MAP_TYPE_PROG_ARRAY,           //!< Must have the key and value with 4 bytes
	.key_size    = sizeof(teid_t_),                   //!< program identifier. 
  .value_size  = sizeof(s32),                       //!< program which represents the session.
  // TODO navarrothiago - check how the management works. The size should be equal
  // to the maximum number of sessions.
	.max_entries = 10000,                             //!< TODO navarrothiago - Is it enought?
};
// Maps UE IPv4 address to SessionProgram
// FIXME navarrothiago - select a primary key. We could use a hash valeu of the IP as a key.
struct bpf_map_def SEC("maps") m_ueip_session = {
	.type        = BPF_MAP_TYPE_PROG_ARRAY,           //!< Must have the key and value with 4 bytes
	.key_size    = sizeof(u32),                       //!< program identifier. 
	.value_size  = sizeof(s32),                       //!< program which represents the session.
  // TODO navarrothiago - check how the management works. The size should be equal
  // to the maximum number of sessions.
  .max_entries = 100000,                             //!< TODO navarrothiago - Is it enought?
};
struct bpf_map_def SEC("maps") m_ue_ip_pdr = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(u32),                        //!< UE IP
    .value_size = sizeof(u32),                      //!< PDR
    .max_entries = 100000,
};
// struct bpf_map_def SEC("maps") m_id_txcnt = {
// 	.type        = BPF_MAP_TYPE_DEVMAP,
// 	.key_size    = sizeof(u32), // id
// 	.value_size  = sizeof(u32), // tx counter
// 	.max_entries = 1,
// };
// //  Downlink maps.
// struct bpf_map_def SEC("maps") m_ue_ip_pdrs_counter = {
// 	.type        = BPF_MAP_TYPE_HASH,
// 	.key_size    = sizeof(teid_t_), // teid
// 	.value_size  = sizeof(u32), // number of allocated PDR in teid
// 	.max_entries = 100000,
// };

#endif // __MAPS_H__
