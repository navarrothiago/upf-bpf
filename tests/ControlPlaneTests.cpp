#include "interfaces/ForwardingActionRulesImpl.h"
#include "interfaces/PacketDetectionRulesImpl.h"
#include "interfaces/RulesUtilitiesImpl.h"
#include "interfaces/SessionBpfImpl.h"
#include <SessionManager.h>
#include <UserPlaneComponent.h>
#include <UPFProgram.h>
#include <SessionProgramManager.h>
#include <SessionProgram.h>
#include <arpa/inet.h> // inet_aton
#include <assert.h>    // assert
#include <bpf/libbpf.h>
#include <bpf/xdp_stats_kern_user.h>
#include <ie/far_id.h>
#include <ie/group_ie/pdi.h>
#include <ie/source_interface.h>
#include <ie/destination_interface.h>
#include <pfcp/pfcp_pdr.h>
#include <pfcp/pfcp_session.h>
#include <stdio.h>
#include <unistd.h> //sleep
#include <wrappers/BPFMaps.h>
#include "InformationElementFactory.hpp"

static std::shared_ptr<SessionManager> spSessionManager;

enum FlowDirection{
  UPLINK,
  DOWNLINK
};

// simple per-protocol drop counter
static void poll_stats(int interval, teid_t_ teid, struct in_addr ueIpAddress, seid_t_ seid, FlowDirection direction)
{
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);
  if(!pSessionProgram){
    throw std::runtime_error("Session not found!");
  }

  // Only used to check if there is a PDR and print.
  auto pPDRMap = direction == UPLINK ? pSessionProgram->getUplinkPDRsMap() : pSessionProgram->getDownlinkPDRsMap();
  auto pCounterMap = pSessionProgram->getCounterMap();

  pfcp_pdr_t_ pdr;
  const int nr_cpus = libbpf_num_possible_cpus();
  const uint32_t row = XDP_ACTION_MAX;
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

    // Loop again if there isnt a PDR created.
    if(direction == UPLINK && pPDRMap->lookup(teid, &pdr) != 0
      || direction == DOWNLINK && pPDRMap->lookup(ueIpAddress.s_addr, &pdr) != 0) {
      perror("lookup error m_teid_pdrs");
      continue;
    }

    printf("pdr pdi_id rules %d\n", pdr.pdr_id.rule_id);

    for(u32 i = 0; i < XDP_ACTION_MAX; i++) {
      if(pCounterMap->lookup(i, values[i]) != 0) {
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

int main(int argc, char **argv)
{
  std::shared_ptr<RulesUtilities> mpRulesFactory;
  mpRulesFactory = std::make_shared<RulesUtilitiesImpl>();
  UserPlaneComponent::getInstance().setup(mpRulesFactory);
  spSessionManager = UserPlaneComponent::getInstance().getSessionManager();
  struct in_addr src_addr;
  struct in_addr ue_ip;
  struct in_addr dst_addr;

  // Fill the source address;
  // TODO navarrothiago - Avoid hardcoded.
  if(inet_aton("10.10.10.10", &src_addr) == 0) {
    fprintf(stderr, "Invalid address\n");
    return 1;
  }
  if(inet_aton("10.1.3.27", &dst_addr) == 0) {
    fprintf(stderr, "Invalid address\n");
    return 1;
  }
  // if(inet_aton("10.1.3.27", &ue_ip) == 0) {
  //   fprintf(stderr, "Invalid address\n");
  //   return 1;
  // }

  LOG_DBG("src_addr:{} dst_addr:{} ue_ip:{}", src_addr.s_addr, dst_addr.s_addr, ue_ip.s_addr);

  // Initialize context.
  seid_t_ seid = 1;
  u16 pdrIdUL = 10;
  u16 pdrIdDL = 20;
  u32 farIdUL = 100;
  u32 farIdDL = 200;
  u32 teid = 100;
  apply_action_t_ actions;
  actions.forw = true;
  u16 dstPort = 1234;

  // Create session, PDR and FAR
  auto pSession = createSession(seid);

  // Uplink.
  auto pPdrUL = createPDR(pdrIdUL, farIdUL, teid, INTERFACE_VALUE_ACCESS, src_addr, OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4);
  auto pFarUL = createFAR(farIdUL, actions, INTERFACE_VALUE_CORE, OUTER_HEADER_CREATION_UDP_IPV4, dst_addr, dstPort);

  // Downlink.
  auto pPdrDL = createPDR(pdrIdDL, farIdDL, teid, INTERFACE_VALUE_CORE, dst_addr, OUTER_HEADER_REMOVAL_UDP_IPV4);
  auto pFarDL = createFAR(farIdDL, actions, INTERFACE_VALUE_ACCESS, OUTER_HEADER_CREATION_GTPU_UDP_IPV4, dst_addr, dstPort);

  // Request to BPF program.
  LOG_INF("Case: create session");
  spSessionManager->createSession(pSession);
  LOG_INF("Case: add UL PDR");
  spSessionManager->addPDR(pSession->getSeid(), pPdrUL);
  LOG_INF("Case: add UL FAR");
  spSessionManager->addFAR(pSession->getSeid(), pFarUL);
  LOG_INF("Case: add DL PDR");
  spSessionManager->addPDR(pSession->getSeid(), pPdrDL);
  LOG_INF("Case: add DL FAR");
  spSessionManager->addFAR(pSession->getSeid(), pFarDL);

  poll_stats(2, teid, dst_addr, seid, DOWNLINK);

  while(1) {
  };
  return 0;
}
