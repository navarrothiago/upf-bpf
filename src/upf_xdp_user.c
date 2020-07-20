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

#include <ie/group_ie/create_pdr.h>

static int ifindex;
static __u32 xdp_flags = XDP_FLAGS_UPDATE_IF_NOEXIST;
static __u32 prog_id;

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
static void poll_stats(int map_fd, int interval)
{
  create_pdr_t pValue, prev[UINT8_MAX] = {0};
  int i;

  printf("pool stats\n");
  while (1)
  {
    __u32 key_teid = UINT32_MAX;

    sleep(interval);

    while (bpf_map_get_next_key(map_fd, &key_teid, &key_teid) != -1)
    {
      __u64 sum = 0;

      if (bpf_map_lookup_elem(map_fd, &key_teid, &pValue) != 0)
      {
        perror("lookup error");
        continue;
      }
      printf("pdr pdi_id rules %d\n", pValue.pdr_id.rule_id);
    }
  }
}

int insert_create_pdr(int map_fd, __u32 key_teid)
{
  create_pdr_t value;

  // Rule id is equal do rule id for while.
  value.pdr_id.rule_id = key_teid; 

  printf("Update create_pdr at key %d\n", key_teid);
  if (bpf_map_update_elem(map_fd, &key_teid, &value, BPF_NOEXIST) != 0)
  {
    perror("Update error");
    return 1;
  }
  printf("Updated create_pdr at key %d\n", key_teid);
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
  int prog_fd, map_fd;
  struct bpf_object *obj = NULL;
  struct bpf_map *map = NULL;
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

  map = bpf_map__next(NULL, obj);
  if (!map)
  {
    printf("finding a map in obj file failed\n");
    return 1;
  }
  map_fd = bpf_map__fd(map);

  signal(SIGINT, int_exit);
  signal(SIGTERM, int_exit);
  signal(SIGSEGV, int_exit);

  if (bpf_set_link_xdp_fd(ifindex, prog_fd, xdp_flags) < 0)
  {
    printf("link set xdp fd failed\n");
    return 1;
  }

  err = bpf_obj_get_info_by_fd(prog_fd, &info, &info_len);
  if (err)
  {
    printf("can't get prog info - %s\n", strerror(errno));
    return err;
  }
  prog_id = info.id;

  // The value 100 will must be equal to GPDU tunnelId. 
  // TODO navarrothiago remove hardcoded.
  if (insert_create_pdr(map_fd, 100))
  {
    return 1;
  }
  poll_stats(map_fd, 2);

  while (1)
  {
  };
  return 0;
}