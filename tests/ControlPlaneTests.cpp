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

// simple per-protocol drop counter
static void poll_stats(int interval, teid_t_ teid, seid_t_ seid)
{
  auto pSessionProgram = SessionProgramManager::getInstance().findSessionProgram(seid);
  if(!pSessionProgram){
    throw std::runtime_error("Session not found!");
  }
  auto pUplinkMap = pSessionProgram->getUplinkPDRsMap();
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

    if(pUplinkMap->lookup(teid, &pdr) != 0) {
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

  // Fill the source address;
  // TODO navarrothiago - Avoid hardcoded.
  if(inet_aton("10.10.10.10", &src_addr) == 0) {
    fprintf(stderr, "Invalid address\n");
    return 1;
  }
  
  // Initialize context.
  seid_t_ seid = 1;
  u16 pdrId = 10; 
  u32 farId = 100;
  u32 teid = 100;
  apply_action_t_ actions;
  actions.forw = true;

  // Create session, PDR and FAR
  auto pSession = createSession(seid);
  auto pPdr = createPDR(pdrId, farId, teid, INTERFACE_VALUE_ACCESS, src_addr);
  auto pFar = createFAR(farId, actions, INTERFACE_VALUE_CORE);

  // Request to BPF program.
  LOG_INF("Case: create session");
  spSessionManager->createSession(pSession);
  LOG_INF("Case: add PDR");
  spSessionManager->addPDR(pSession->getSeid(), pPdr);
  LOG_INF("Case: add FAR");
  spSessionManager->addFAR(pSession->getSeid(), pFar);

  poll_stats(2, teid, seid);

  while(1) {
  };
  return 0;
}
