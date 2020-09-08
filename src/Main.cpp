#include <bpf/libbpf.h>
#include <bpf/xdp_stats_kern_user.h>
#include <arpa/inet.h> // inet_aton
#include <assert.h>    // assert
#include <ie/far_id.h>
#include <ie/group_ie/pdi.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>
#include <stdio.h>
#include <unistd.h> //sleep
#include <UPFProgramManager.h>
#include <wrappers/BPFMaps.h>

// simple per-protocol drop counter
static void poll_stats(int interval, teid_t_ key_teid)
{
  auto pMaps = UPFProgramManager::getInstance().getMaps();
  pfcp_pdr_t_ pdr;
  const size_t_ nr_cpus = libbpf_num_possible_cpus();
  const size_t_ row = XDP_ACTION_MAX;
  struct datarec values[XDP_ACTION_MAX][nr_cpus], prev[XDP_ACTION_MAX][nr_cpus];
  __u64 sum[nr_cpus];

  printf("Number of CPU available %d\n", nr_cpus);

  memset(prev, 0, sizeof(prev));
  memset(sum, 0, sizeof(sum));

  printf("pool stats\n");
  while(1) {
    // teid_t_ key_teid = UINT32_MAX;
    memset(sum, 0, sizeof(sum));
    sleep(interval);


    if(pMaps->getMap("m_teid_pdrs").lookup(key_teid, &pdr) != 0) {
      perror("lookup error m_teid_pdrs");
      continue;
    }

    printf("pdr pdi_id rules %d\n", pdr.pdr_id.rule_id);

    for(u32 i = 0; i < XDP_ACTION_MAX; i++) {
      if(pMaps->getMap("mc_stats").lookup(i, values[i]) != 0) {
        perror("lookup error mc_stats");
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

int insert_elements(u32 key_teid)
{
  pfcp_pdr_t_ pdr;
  pfcp_far_t_ far;
  pfcp_session_t_ session;
  u32 counter = 0;
  u32 pdrs_fars_counter = 0;
  u32 seid = 1;
  struct in_addr src_addr;

  auto pMaps = UPFProgramManager::getInstance().getMaps();

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
  if(pMaps->getMap("m_teid_pdrs").update(pdr.teid, pdr, BPF_NOEXIST) != 0
      || pMaps->getMap("m_teid_pdrs_counter").update(pdr.teid, counter, BPF_NOEXIST) != 0
      || pMaps->getMap("m_seid_session").update(session.seid, session, BPF_NOEXIST) != 0) {
    perror("Update error");
    return 1;
  }
  // clang-format on
  printf("Updated create_pdr at teid %d, counter %d\n", key_teid, counter);
  return 0;
}

int main(int argc, char **argv)
{
  UPFProgramManager::getInstance().setup();
  if(insert_elements(100)) {
    exit(1);
  }

  poll_stats(2, 100);

  while(1) {
  };
  return 0;
}
