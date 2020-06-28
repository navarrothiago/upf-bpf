// Exemple based on https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/samples/bpf/xdp1_kern.c
#include <linux/bpf.h>
#include <bpf_helpers.h>

// Simple XDP BPF program. Everything packet will be dropped. 
SEC("test")
int xdp_prog1(struct xdp_md *ctx){
  return XDP_DROP;
}