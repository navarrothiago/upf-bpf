// Example based on https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/samples/bpf/xdp1_user.c
#include <linux/bpf.h>
#include <linux/if_link.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <linux/bpf.h>
#include "libbpf.h"

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

int main(int argc, char **argv)
{
  struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
  struct bpf_prog_load_attr prog_load_attr = {
      .prog_type = BPF_PROG_TYPE_XDP,
  };
  struct bpf_prog_info info = {};
  __u32 info_len = sizeof(info);
  int prog_fd;
  struct bpf_object *obj;
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

  signal(SIGINT, int_exit);
  signal(SIGTERM, int_exit);

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

  while (1) ;
  return 0;
}