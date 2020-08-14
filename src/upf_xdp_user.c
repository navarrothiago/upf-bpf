// upf_xdp_bpf_c based on
// https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/samples/bpf/xdp1_user.c
#include <assert.h>
#include <bpf/libbpf.h>
#include <bpf/xdp_stats_kern_user.h>
#include <errno.h>
#include <ie/far_id.h>
#include <ie/group_ie/pdi.h>
#include <linux/bpf.h>
#include <linux/if_link.h>
#include <net/if.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>
#include <upf_xdp_bpf_skel.h>


#define VAR(var) #var

static int ifindex_in, ifindex_out;
static bool ifindex_out_xdp_redirect_attached = true;
static __u32 xdp_flags = XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE;
static __u32 prog_id, prog_redirect_id;
static int prog_fd, prog_redirect_fd, map_fd_pdrs, map_fd_pdrs_counter, map_fd_seid_pdrs_counter, map_fd_seid_session, map_fd_id_txport;
static void int_exit(int sig)
{
  __u32 curr_prog_id = 0;

  if(bpf_get_link_xdp_id(ifindex_in, &curr_prog_id, xdp_flags)) {
    printf("bpf_get_link_xdp_id failed\n");
    exit(1);
  }
  if(prog_id == curr_prog_id)
    bpf_set_link_xdp_fd(ifindex_in, -1, xdp_flags);
  else if(!curr_prog_id)
    printf("couldn't find a prog id on iface IN\n");
  else
    printf("program on iface IN changed, not removing\n");

  if(ifindex_out_xdp_redirect_attached) {
    curr_prog_id = 0;
    if(bpf_get_link_xdp_id(ifindex_out, &curr_prog_id, xdp_flags)) {
      printf("bpf_get_link_xdp_id failed\n");
      exit(1);
    }
    if(prog_redirect_id == curr_prog_id)
      bpf_set_link_xdp_fd(ifindex_out, -1, xdp_flags);
    else if(!curr_prog_id)
      printf("couldn't find a prog id on iface OUT\n");
    else
      printf("program on iface OUT changed, not removing\n");
  }
  exit(0);
}

// simple per-protocol drop counter
static void poll_stats(int interval)
{
  pfcp_pdr_t pdr;
  unsigned int nr_cpus = libbpf_num_possible_cpus();
  struct datarec values[XDP_ACTION_MAX][nr_cpus], prev[XDP_ACTION_MAX][nr_cpus];
  __u64 sum[nr_cpus];

  printf("Number of CPU available %d\n", nr_cpus);

  memset(prev, 0, sizeof(prev));
  memset(sum, 0, sizeof(sum));

  printf("pool stats\n");
  while(1) {
    __u32 key_teid = UINT32_MAX;
    memset(sum, 0, sizeof(sum));
    sleep(interval);

    while(bpf_map_get_next_key(map_fd_pdrs, &key_teid, &key_teid) != -1) {
      if(bpf_map_lookup_elem(map_fd_pdrs, &key_teid, &pdr) != 0) {
        perror("lookup error");
        continue;
      }

      printf("pdr pdi_id rules %d\n", pdr.pdr_id.rule_id);

      for(u32 i = 0; i < XDP_ACTION_MAX; i++) {
        if(bpf_map_lookup_elem(map_fd_stats, &i, &values[i]) != 0) {
          perror("lookup error");
          continue;
        }
        for(u32 j = 0; j < nr_cpus; j++) {
          // printf("%s - core %d - values %d - prev %d\n",xdp_action_names[i], j, values[i][j].rx_packets, prev[i][j].rx_packets);
          sum[i] += (values[i][j].rx_packets - prev[i][j].rx_packets);
        }
        if(sum[i])
          printf("%s\t\t%10llu pkt/s\n", xdp_action_names[i], sum[i] / interval);
        memcpy(prev[i], values[i], sizeof(values[i]));
      }
      printf("\n");
    }
  }
}

int insert_elements(u32 key_teid)
{
  pfcp_pdr_t pdr;
  pfcp_far_t far;
  pfcp_session_t session;
  u32 counter = 0;
  u32 pdrs_fars_counter = 0;
  u32 seid = 1;
  struct in_addr src_addr;

  // Fill the source address;
  // TODO navarrothiago - Avoid hardcoded.
  if(inet_aton("10.10.10.10", &src_addr) == 0) {
    fprintf(stderr, "Invalid address\n");
    return 1;
  }

  // Fill the PDR entry
  pdr.teid = key_teid;
  pdr.local_seid = seid + 1;
  pdr.pdr_id.rule_id = key_teid;
  pdr.outer_header_removal.outer_header_removal_description = OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4;
  pdr.pdi.source_interface.interface_value = INTERFACE_VALUE_ACCESS;
  pdr.pdi.fteit.teid = key_teid;
  pdr.pdi.ue_ip_address.ipv4_address = src_addr.s_addr;
  counter += 1;

  // Fill the FAR entry with FAR ID 1, INTERFACE_VALUE_CORE, action forward.
  far.apply_action.forw = true;
  far.forwarding_parameters.outer_header_creation.outer_header_creation_description = OUTER_HEADER_CREATION_UDP_IPV4;
  far.forwarding_parameters.destination_interface.interface_value = INTERFACE_VALUE_ACCESS;
  far.far_id.far_id = 1;

  // Fill the session entry with PDR and FAR.
  session.seid = pdr.local_seid;
  session.pdrs[pdrs_fars_counter] = pdr;
  session.fars[pdrs_fars_counter] = far;
  pdrs_fars_counter += 1;
  session.pdrs_counter = pdrs_fars_counter;
  session.fars_counter = pdrs_fars_counter;

  printf("Update create_pdr at key %d, counter\n", key_teid, counter);

  // clang-format off
  // Update BPF maps
  if(bpf_map_update_elem(map_fd_pdrs, &pdr.teid, &pdr, BPF_NOEXIST) != 0
      || bpf_map_update_elem(map_fd_pdrs_counter, &key_teid, &counter, BPF_NOEXIST) != 0
      || bpf_map_update_elem(map_fd_seid_session, &session.seid, &session, BPF_NOEXIST) != 0) {
    perror("Update error");
    return 1;
  }
  // clang-format on
  printf("Updated create_pdr at teid %d, counter %d\n", key_teid, counter);
  return 0;
}

int main(int argc, char **argv)
{
  struct upf_xdp_bpf_c *skel;
  struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
  struct bpf_prog_info info = {};
  __u32 info_len = sizeof(info);
  u32 key_ifmap = 0;
  int err;

  if(setrlimit(RLIMIT_MEMLOCK, &r)) {
    perror("setrlimit(RLIMIT_MEMLOCK)");
    return 1;
  }

  skel = upf_xdp_bpf_c__open();
  if(!skel)
    goto cleanup;

  // TODO navarrothiago - remove hardcoded.
  ifindex_in = if_nametoindex("wlp0s20f3");
  ifindex_out = if_nametoindex("veth0");
  if(!ifindex_in || !ifindex_out) {
    perror("if_nametoindex");
    return 1;
  }

  // Load BPF programs idetified in skeleton.
  err = upf_xdp_bpf_c__load(skel);
  if(err)
    goto cleanup;

  // Attach is not support by XDP programs.
  // This call doesnt do anything.
  err = upf_xdp_bpf_c__attach(skel);
  if(err)
    goto cleanup;

  // Get FD from skeleton object.
  prog_fd = bpf_program__fd(skel->progs.upf_chain);
  prog_redirect_fd = bpf_program__fd(skel->progs.xdp_redirect_gtpu);
  map_fd_pdrs = bpf_map__fd(skel->maps.m_teid_pdrs);
  map_fd_pdrs_counter = bpf_map__fd(skel->maps.m_teid_pdrs_counter);
  map_fd_seid_session = bpf_map__fd(skel->maps.m_seid_session);
  map_fd_id_txport = bpf_map__fd(skel->maps.m_id_txport);
  map_fd_stats = bpf_map__fd(skel->maps.mc_stats);

  signal(SIGINT, int_exit);
  signal(SIGTERM, int_exit);
  signal(SIGSEGV, int_exit);

  if(bpf_set_link_xdp_fd(ifindex_in, prog_fd, xdp_flags) < 0) {
    printf("link set xdp fd IN failed\n");
    int_exit(SIGTERM);
  }

  // Get info from XDP program.
  err = bpf_obj_get_info_by_fd(prog_fd, &info, &info_len);
  if(err) {
    printf("can't get prog info IN - %s\n", strerror(errno));
    int_exit(SIGTERM);
  }
  prog_id = info.id;

  if(bpf_set_link_xdp_fd(ifindex_out, prog_redirect_fd, xdp_flags | XDP_FLAGS_UPDATE_IF_NOEXIST) < 0) {
    printf("link set xdp fd OUT failed\n");
    int_exit(SIGTERM);
  }

  // Clean memory. Without this, the error "can't get prog info OUT - Bad address" on bpf_obj_get_info_by_fd is shown.
  memset(&info, 0, sizeof(info));

  // Get info from Reditect XDP program.
  err = bpf_obj_get_info_by_fd(prog_redirect_fd, &info, &info_len);
  if(err) {
    printf("can't get prog info OUT - %s\n", strerror(errno));
    int_exit(SIGTERM);
  }
  prog_redirect_id = info.id;

  // The pdr 100 will must be equal to GPDU tunnelId.
  // TODO navarrothiago - remove hardcoded.
  if(insert_elements(100)) {
    int_exit(SIGTERM);
  }

  // Populate virtual to physical port map.
  if(bpf_map_update_elem(map_fd_id_txport, &key_ifmap, &ifindex_out, 0)) {
    perror("bpf_update_elem txport map");
    goto cleanup;
  }
  poll_stats(2);

  while(1) {
  };
  return 0;

cleanup:
  upf_xdp_bpf_c__destroy(skel);
  return err;
}
