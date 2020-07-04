#include <linux/bpf.h>
#include <bpf_helpers.h>
#include <types.h>
#include <linux/if_ether.h>
#include <protocols/eth.h>

SEC("gtp_process_chain")
int xdp_prog1(struct xdp_md *ctx)
{
  void *data = (void *)(long)ctx->data;
  struct ethhdr *eth = data;

  return eth_handle(ctx, eth);
}

char _license[] SEC("license") = "GPL";