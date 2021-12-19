#ifndef BPF_UTILS_H
#define BPF_UTILS_H

// Dictionary
// htons() - host to network short
// htonl() - host to network long
// ntohs() - network to host short
// ntohl() - network to host long
// If not defined -> "failed to find BTF for extern"

#ifndef htons
#define htons(x) __constant_htons((x))
#endif

#ifndef htonl
#define htonl(x) __constant_htonl((x))
#endif

#ifndef ntohs
#define ntohs(x) __constant_ntohs((x))
#endif

#ifndef ntohl
#define ntohl(x) __constant_ntohl((x))
#endif

static void swap_src_dst_mac(void *data)
{
  bpf_debug("Swapping MAC address...");
	unsigned short *p = data;
	unsigned short dst[3];

	dst[0] = p[0];
	dst[1] = p[1];
	dst[2] = p[2];
	p[0] = p[3];
	p[1] = p[4];
	p[2] = p[5];
	p[3] = dst[0];
	p[4] = dst[1];
	p[5] = dst[2];
  bpf_debug("Swapping MAC address...DONE!");
}


#endif
