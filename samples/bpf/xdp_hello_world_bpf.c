// Exemple based on https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/samples/bpf/xdp1_kern.c
#include <linux/bpf.h>
#include <bpf_helpers.h>

// Simple XDP BPF program. Everything packet will be dropped. 
SEC("test")
int xdp_prog1(struct xdp_md *ctx){
  char drop_message[] = "XDP PACKET DROP\n";
  bpf_trace_printk(&drop_message, sizeof(drop_message));
  return XDP_DROP;
}

char _license[] SEC("license") = "GPL";

