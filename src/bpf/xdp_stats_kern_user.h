/* SPDX-License-Identifier: GPL-2.0 */

/* Used by BPF-prog kernel side BPF-progs and userspace programs,
 * for sharing xdp_stats common struct and DEFINEs.
 */
#ifndef __XDP_STATS_KERN_USER_H
#define __XDP_STATS_KERN_USER_H

#include <linux/bpf.h>

/* This is the data record stored in the map */
struct datarec {
	__u64 rx_packets;
	__u64 rx_bytes;
};

#ifndef XDP_ACTION_MAX
#define XDP_ACTION_MAX (XDP_REDIRECT + 1)
#endif

// clang-format off
static const char *xdp_action_names[XDP_ACTION_MAX] = {
    [XDP_ABORTED] = "XDP_ABORTED",
    [XDP_DROP] = "XDP_DROP",
    [XDP_PASS] = "XDP_PASS",
    [XDP_TX] = "XDP_TX",
    [XDP_REDIRECT] = "XDP_REDIRECT"
};
// clang-format on

static int map_fd_stats;

#endif /* __XDP_STATS_KERN_USER_H */
