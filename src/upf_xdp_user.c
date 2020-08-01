// upf_xdp_bpf_c based on
// https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/samples/bpf/xdp1_user.c
#include <assert.h>
#include <bpf/libbpf.h>
#include <errno.h>
#include <ie/far_id.h>
#include <ie/group_ie/pdi.h>
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

static int ifindex;
static __u32 xdp_flags = XDP_FLAGS_UPDATE_IF_NOEXIST;
static __u32 prog_id;
static int prog_fd, map_fd_pdrs, map_fd_pdrs_counter, map_fd_seid_pdrs_counter, map_fd_seid_session;

static void int_exit(int sig)
{
  __u32 curr_prog_id = 0;

  if(bpf_get_link_xdp_id(ifindex, &curr_prog_id, xdp_flags)) {
    printf("bpf_get_link_xdp_id failed\n");
    exit(1);
  }
  if(prog_id == curr_prog_id)
    bpf_set_link_xdp_fd(ifindex, -1, xdp_flags);
  else if(!curr_prog_id)
    printf("couldn't find a prog id on a given interface\n");
  else
    printf("program on interface changed, not removing\n");
  exit(0);
}

// simple per-protocol drop counter
static void poll_stats(int interval)
{
  pfcp_pdr_t pdr, prev[UINT8_MAX] = {0};

  printf("pool stats\n");
  while(1) {
    __u32 key_teid = UINT32_MAX;
    sleep(interval);

    while(bpf_map_get_next_key(map_fd_pdrs, &key_teid, &key_teid) != -1) {
      if(bpf_map_lookup_elem(map_fd_pdrs, &key_teid, &pdr) != 0) {
        perror("lookup error");
        continue;
      }
      printf("pdr pdi_id rules %d\n", pdr.pdr_id.rule_id);
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
  int err;

  if(setrlimit(RLIMIT_MEMLOCK, &r)) {
    perror("setrlimit(RLIMIT_MEMLOCK)");
    return 1;
  }

  skel = upf_xdp_bpf_c__open();
  if(!skel) goto cleanup;

  // TODO navarrothiago - remove hardcoded.
  ifindex = if_nametoindex("enp0s20f0u1");
  if(!ifindex) {
    perror("if_nametoindex");
    return 1;
  }

  err = upf_xdp_bpf_c__load(skel);
  if(err) goto cleanup;

  // Attach is not support by XDP programs.
  // This call doesnt do anything.
  err = upf_xdp_bpf_c__attach(skel);
  if(err) goto cleanup;

  prog_fd = bpf_program__fd(skel->progs.upf_chain);
  map_fd_pdrs = bpf_map__fd(skel->maps.m_teid_pdrs);
  map_fd_pdrs_counter = bpf_map__fd(skel->maps.m_teid_pdrs_counter);
  map_fd_seid_session = bpf_map__fd(skel->maps.m_seid_session);

  signal(SIGINT, int_exit);
  signal(SIGTERM, int_exit);
  signal(SIGSEGV, int_exit);

  if(bpf_set_link_xdp_fd(ifindex, prog_fd, xdp_flags) < 0) {
    printf("link set xdp fd failed\n");
    int_exit(SIGTERM);
  }

  err = bpf_obj_get_info_by_fd(prog_fd, &info, &info_len);
  if(err) {
    printf("can't get prog info - %s\n", strerror(errno));
    int_exit(SIGTERM);
  }
  prog_id = info.id;

  // The pdr 100 will must be equal to GPDU tunnelId.
  // TODO navarrothiago - remove hardcoded.
  if(insert_elements(100)) {
    int_exit(SIGTERM);
  }
  poll_stats(2);

  while(1) {
  };
  return 0;

cleanup:
  upf_xdp_bpf_c__destroy(skel);
  return err;
}