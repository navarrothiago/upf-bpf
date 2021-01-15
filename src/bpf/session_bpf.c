#include <linux/bpf.h>
#include <bpf_helpers.h>
#include <session_bpf_maps.h>
#include <types.h>

// Simple XDP BPF program. Everything packet will be dropped.
SEC("xdp_entry_point")
int entry_point(struct xdp_md *ctx){
  char drop_message[] = "XDP SESSION CONTEXT %d\n";
  u32 *value;
  u32 key = 0;

  value = bpf_map_lookup_elem(&m_far_id_far, &key);

  if(!value) {
    return XDP_PASS;
  }

  bpf_trace_printk(&drop_message, sizeof(drop_message), *value);
  
  return XDP_DROP;
}

char _license[] SEC("license") = "GPL";