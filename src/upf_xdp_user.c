// Example based on https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/samples/bpf/xdp1_user.c
#include <linux/if_link.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <libbpf.h>
#include <assert.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>

#define VAR(var) #var

static int ifindex;
static __u32 xdp_flags = XDP_FLAGS_UPDATE_IF_NOEXIST;
static __u32 prog_id;
static int prog_fd, map_fd_pdrs, map_fd_pdrs_counter, map_fd_seid_pdrs_counter, map_fd_seid_session;

static void int_exit(int sig)
{
  __u32 curr_prog_id = 0;

  if (bpf_get_link_xdp_id(ifindex, &curr_prog_id, xdp_flags))
  {
    printf("bpf_get_link_xdp_id failed\n");
    exit(1);
  }
  if (prog_id == curr_prog_id)
    bpf_set_link_xdp_fd(ifindex, -1, xdp_flags);
  else if (!curr_prog_id)
    printf("couldn't find a prog id on a given interface\n");
  else
    printf("program on interface changed, not removing\n");
  exit(0);
}

/* simple per-protocol drop counter
 */
static void poll_stats(int interval)
{
  pfcp_pdr_t pdr, prev[UINT8_MAX] = {0};

  printf("pool stats\n");
  while (1)
  {
    __u32 key_teid = UINT32_MAX;
    sleep(interval);

    while (bpf_map_get_next_key(map_fd_pdrs, &key_teid, &key_teid) != -1)
    {
      if (bpf_map_lookup_elem(map_fd_pdrs, &key_teid, &pdr) != 0)
      {
        perror("lookup error");
        continue;
      }
      printf("pdr pdi_id rules %d\n", pdr.pdr.pdr_id.rule_id);
    }
  }
}

int insert_elements(u32 key_teid)
{
  pfcp_pdr_t value;
  pfcp_session_t session;
  u32 counter = 0;
  u32 seid_pdrs_counter = 0;
  u32 seid = 1;

  // Fill the PDR entry
  value.teid = key_teid;
  value.local_seid = seid + 1;
  value.pdr.pdr_id.rule_id = key_teid; 
  counter += 1;

  // Fill the session entry
  session.seid = value.local_seid;
  session.pdrs[seid_pdrs_counter] = value;
  seid_pdrs_counter += 1;
  session.iter = seid_pdrs_counter;
  
  printf("Update create_pdr at key %d, counter\n", key_teid, counter);
  if (bpf_map_update_elem(map_fd_pdrs, &value.teid, &value, BPF_NOEXIST) != 0 
      || bpf_map_update_elem(map_fd_pdrs_counter, &key_teid, &counter, BPF_NOEXIST) != 0 
      || bpf_map_update_elem(map_fd_seid_session, &session.seid, &session, BPF_NOEXIST) != 0 
      || bpf_map_update_elem(map_fd_seid_pdrs_counter, &value.local_seid, &seid_pdrs_counter, BPF_NOEXIST) != 0 
      )
  {
    perror("Update error");
    int_exit(0);
  }
  printf("Updated create_pdr at teid %d, counter %d\n", key_teid, counter);
  return 0;
}

int main(int argc, char **argv)
{
  struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
  struct bpf_prog_load_attr prog_load_attr = {
      .prog_type = BPF_PROG_TYPE_XDP,
  };
  struct bpf_prog_info info = {};
  __u32 info_len = sizeof(info);
  // TODO create map string to int (map_fd)
  struct bpf_object *obj = NULL;
  struct bpf_map *m_teid_pdrs_counter = NULL;
  struct bpf_map *m_teid_pdrs = NULL;
  struct bpf_map *m_seid_pdrs_counter  = NULL;
  struct bpf_map *m_seid_session = NULL;
  
  int err;

  if (setrlimit(RLIMIT_MEMLOCK, &r))
  {
    perror("setrlimit(RLIMIT_MEMLOCK)");
    return 1;
  }

  // TODO navarrothiago Remove hardcode path.
  prog_load_attr.file = "bpf/upf_xdp_bpf.c.o";

  ifindex = if_nametoindex("enp0s20f0u1");

  if (!ifindex)
  {
    perror("if_nametoindex");
    return 1;
  }

  if (bpf_prog_load_xattr(&prog_load_attr, &obj, &prog_fd))
    return 1;

  m_teid_pdrs = bpf_object__find_map_by_name(obj, VAR(m_teid_pdrs));
  m_teid_pdrs_counter = bpf_object__find_map_by_name(obj, VAR(m_teid_pdrs_counter));
  m_seid_pdrs_counter = bpf_object__find_map_by_name(obj, VAR(m_seid_pdrs_counter));
  m_seid_session = bpf_object__find_map_by_name(obj, VAR(m_seid_session));

  if (!m_teid_pdrs || !m_teid_pdrs_counter || !m_seid_session || !m_seid_pdrs_counter)
  {
    printf("finding a map in obj file failed\n");
    int_exit(SIGTERM);
  }
  map_fd_pdrs = bpf_map__fd(m_teid_pdrs);
  map_fd_pdrs_counter = bpf_map__fd(m_teid_pdrs_counter);
  map_fd_seid_pdrs_counter = bpf_map__fd(m_seid_pdrs_counter);
  map_fd_seid_session = bpf_map__fd(m_seid_session);

  signal(SIGINT, int_exit);
  signal(SIGTERM, int_exit);
  signal(SIGSEGV, int_exit);

  if (bpf_set_link_xdp_fd(ifindex, prog_fd, xdp_flags) < 0)
  {
    printf("link set xdp fd failed\n");
    int_exit(SIGTERM);
  }

  err = bpf_obj_get_info_by_fd(prog_fd, &info, &info_len);
  if (err)
  {
    printf("can't get prog info - %s\n", strerror(errno));
    int_exit(SIGTERM);
  }
  prog_id = info.id;

  // The value 100 will must be equal to GPDU tunnelId. 
  // TODO navarrothiago remove hardcoded.
  if (insert_elements(100))
  {
    int_exit(SIGTERM);
  }
  poll_stats(2);

  while (1)
  {
  };
  return 0;
}