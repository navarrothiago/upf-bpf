/* SPDX-License-Identifier: GPL-2.0 */

/* Used *ONLY* by BPF-prog running kernel side. */
#ifndef __XDP_STATS_KERN_H
#define __XDP_STATS_KERN_H

#include <bpf_helpers.h>
#include <utils/logger.h>

/* Data record type 'struct datarec' is defined in common/xdp_stats_kern_user.h,
 * programs using this header must first include that file.
 */
#ifndef __XDP_STATS_KERN_USER_H
#warning "You forgot to #include "xdp_stats_kern_user.h"
#include "xdp_stats_kern_user.h"
#endif

/* Keeps stats per (enum) xdp_action */
struct bpf_map_def SEC("maps") mc_stats = {
    .type = BPF_MAP_TYPE_PERCPU_ARRAY,
    .key_size = sizeof(__u32),
    .value_size = sizeof(struct datarec),
    .max_entries = XDP_ACTION_MAX,
};

/**
 * @brief Record actions in map.
 *
 * @param ctx Ther user accessible metadata for XDP packet hook.
 * @param action The action to be recorded.
 * @return __u32 The XDP action.
 */
static __u32 xdp_stats_record_action(struct xdp_md *ctx, __u32 action)
{
  if(action >= XDP_ACTION_MAX) {
    bpf_debug("Error: Invalid action\n");
    return XDP_ABORTED;
  }

  /* Lookup in kernel BPF-side return pointer to actual data record */
  struct datarec *rec = bpf_map_lookup_elem(&mc_stats, &action);
  if(!rec) {
    bpf_debug("Error: Invalid datarec\n");
    return XDP_ABORTED;
  }

  /* BPF_MAP_TYPE_PERCPU_ARRAY returns a data record specific to current
   * CPU and XDP hooks runs under Softirq, which makes it safe to update
   * without atomic operations.
   */
  rec->rx_packets++;
  rec->rx_bytes += (ctx->data_end - ctx->data);

  return action;
}

#endif /* __XDP_STATS_KERN_H */
