/* Copyright (c) 2017 Covalent IO, Inc. http://covalent.io
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 */
// FROM - https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/plain/samples/bpf/xdp_redirect_map_kern.c

#define KBUILD_MODNAME "foo"
#include <types.h>
#include <linux/bpf.h>
#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_vlan.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <bpf_helpers.h>

struct bpf_map_def SEC("maps") tx_port = {
	.type        = BPF_MAP_TYPE_DEVMAP,
	.key_size    = sizeof(u32),
	.value_size  = sizeof(u32),
	.max_entries = 100,
};

/* Count RX packets, as XDP bpf_prog doesn't get direct TX-success
 * feedback.  Redirect TX errors can be caught via a tracepoint.
 */
struct bpf_map_def SEC("maps") rxcnt = {
	.type        = BPF_MAP_TYPE_PERCPU_ARRAY,
	.key_size    = sizeof(u32),
	.value_size  = sizeof(u32),
	.max_entries = 1,
};

static void swap_src_dst_mac(void *data)
{
	u32 *p = data;
	u32 dst[3];

	dst[0] = p[0];
	dst[1] = p[1];
	dst[2] = p[2];
	p[0] = p[3];
	p[1] = p[4];
	p[2] = p[5];
	p[3] = dst[0];
	p[4] = dst[1];
	p[5] = dst[2];
}

SEC("xdp_redirect_map")
u32 xdp_redirect_map_prog(struct xdp_md *ctx)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data = (void *)(long)ctx->data;
	struct ethhdr *eth = data;
	u32 rc = XDP_DROP;
	u32 vport, port = 0, m = 0;
	u32 *value;
	u32 key = 0;
	u64 nh_off;

	nh_off = sizeof(*eth);
	if (data + nh_off > data_end)
		return rc;

	/* constant virtual port */
	vport = 0;

	/* count packet in global counter */
	value = bpf_map_lookup_elem(&rxcnt, &key);
	if (value)
		*value += 1;

	// swap_src_dst_mac(data);

	/* send packet out physical port */
	return bpf_redirect_map(&tx_port, vport, 0);
}

/* Redirect require an XDP bpf_prog loaded on the TX device */
SEC("xdp_redirect_dummy")
u32 xdp_redirect_dummy_prog(struct xdp_md *ctx)
{
	return XDP_PASS;
}

s8 _license[] SEC("license") = "GPL";
