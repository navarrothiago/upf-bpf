/*
 * This file is a modified version from the source.
 * Copyright 2018 The Polycube Authors
 * Copyright 2021 Thiago Navarro
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __CSUM_H__
#define __CSUM_H__

#include <vmlinux.h>
#include <errno.h>
#include <linux/version.h>

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

static void pcn_update_csum(struct iphdr *iph)
{
  u16 *next_iph_u16;
  u32 csum = 0;
  int i;

  next_iph_u16 = (u16 *)iph;

#pragma clang loop unroll(full)
  for(i = 0; i < sizeof(*iph) >> 1; i++)
    csum += *next_iph_u16++;

  iph->check = ~((csum & 0xffff) + (csum >> 16));
}

/* checksum related stuff */
static __sum16 pcn_csum_fold(__wsum csum)
{
  u32 sum = (u32)csum;
  sum = (sum & 0xffff) + (sum >> 16);
  sum = (sum & 0xffff) + (sum >> 16);
  return (__sum16)~sum;
}

static __wsum pcn_csum_add(__wsum csum, __wsum addend)
{
  u32 res = (u32)csum;
  res += (u32)addend;
  return (__wsum)(res + (res < (u32)addend));
}

static __sum16 pcn_csum16_add(__sum16 csum, __be16 addend)
{
  u16 res = (u16)csum;

  res += (u16)addend;
  return (__sum16)(res + (res < (u16)addend));
}

static __wsum pcn_csum_unfold(__sum16 n) { return (__wsum)n; }

static void pcn_csum_replace_by_diff(__sum16 *sum, __wsum diff) { *sum = pcn_csum_fold(pcn_csum_add(diff, ~pcn_csum_unfold(*sum))); }

static int pcn_l3_csum_replace(struct xdp_md *ctx, u32 csum_offset, u32 old_value, u32 new_value, u32 flags)
{
  __sum16 *ptr;

  if(unlikely(flags & ~(BPF_F_HDR_FIELD_MASK)))
    return -EINVAL;

  if(unlikely(csum_offset > 0xffff || csum_offset & 1))
    return -EFAULT;

  void *data2 = (void *)(long)ctx->data;
  void *data_end2 = (void *)(long)ctx->data_end;
  if(data2 + csum_offset + sizeof(*ptr) > data_end2) {
    return -EINVAL;
  }

  ptr = (__sum16 *)((void *)(long)ctx->data + csum_offset);

  switch(flags & BPF_F_HDR_FIELD_MASK) {
  case 0:
    pcn_csum_replace_by_diff(ptr, new_value);
    break;
  case 2:
    *ptr = ~pcn_csum16_add(pcn_csum16_add(~(*ptr), ~old_value), new_value);
    break;
  case 4:
    pcn_csum_replace_by_diff(ptr, pcn_csum_add(new_value, ~old_value));
    break;
  default:
    return -EINVAL;
  }
  return 0;
}

static
__wsum pcn_csum_diff(__be32 *from, u32 from_size, __be32 *to,
                     u32 to_size, __wsum seed) {
// FIXME: sometimes the LINUX_VERSION_CODE is not aligned with the kernel.
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,16,0)
  return bpf_csum_diff(from, from_size, to, to_size, seed);
#else
  if (from_size != 4 || to_size != 4)
    return -EINVAL;

  __wsum result = pcn_csum_add(*to, ~*from);
  result += seed;
	if (seed > result)
		result += 1;

  return result;
#endif
}

/* end checksum related stuff */
#endif // __CSUM_H__
