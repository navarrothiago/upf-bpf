/* From https://gist.github.com/javiermon/6272065#file-gateway_netlink-c */
/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>

#include <fstream>  // std::ifstream
#include <string>

#include "common_defs.h"
#include "get_gateway_netlink.hpp"
#include "logger.hpp"

#define BUFFER_SIZE 4096

using namespace std;

//------------------------------------------------------------------------------
bool util::get_iface_l2_addr(const std::string& iface, std::string& mac) {
  std::string mac_address_path =
      fmt::format("/sys/class/net/{}/address", iface);
  std::ifstream mac_address_in(
      mac_address_path.c_str(), ios_base::in | ios_base::binary);
  char wb[32];
  mac_address_in.get(wb, 32);
  mac.assign(wb);
  Logger::pfcp_switch().error(
      "Found IFace %s MAC %s", iface.c_str(), mac.c_str());
  mac.erase(std::remove(mac.begin(), mac.end(), ':'), mac.end());
  return true;
  //  ifr = {};
  //  strncpy ((char *) ifr.ifr_name, ifname, IFNAMSIZ);
  //  if (ioctl(sd, SIOCGIFFLAGS, &ifr) == 0) {
  //    if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
  //      if (ioctl(sd, SIOCGIFHWADDR, &ifr) == 0) {
  //        memcpy(pdn_mac_address, ifr.ifr_hwaddr.sa_data, 6);
  //      }
  //    }
  //  }
}
//------------------------------------------------------------------------------
bool util::get_gateway_and_iface(std::string& gw, std::string& iface) {
  int received_bytes = 0, msg_len = 0, route_attribute_len = 0;
  int sock = -1, msgseq = 0;
  struct nlmsghdr *nlh, *nlmsg;
  struct rtmsg* route_entry;
  // This struct contain route attributes (route type)
  struct rtattr* route_attribute;
  char gateway_address[INET_ADDRSTRLEN], interface[IF_NAMESIZE + 1];
  char msgbuf[BUFFER_SIZE], buffer[BUFFER_SIZE];
  char* ptr = buffer;
  struct timeval tv;
  int rv = RETURNok;

  if ((sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) < 0) {
    perror("socket failed");
    return false;
  }

  memset(msgbuf, 0, sizeof(msgbuf));
  memset(gateway_address, 0, sizeof(gateway_address));
  memset(interface, 0, sizeof(interface));
  memset(buffer, 0, sizeof(buffer));

  /* point the header and the msg structure pointers into the buffer */
  nlmsg = (struct nlmsghdr*) msgbuf;

  /* Fill in the nlmsg header*/
  nlmsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
  nlmsg->nlmsg_type =
      RTM_GETROUTE;  // Get the routes from kernel routing table .
  nlmsg->nlmsg_flags =
      NLM_F_DUMP | NLM_F_REQUEST;  // The message is a request for dump.
  nlmsg->nlmsg_seq = msgseq++;     // Sequence of the message packet.
  nlmsg->nlmsg_pid = getpid();     // PID of process sending the request.

  /* 1 Sec Timeout to avoid stall */
  tv.tv_sec = 1;
  setsockopt(
      sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*) &tv,
      sizeof(struct timeval));
  /* send msg */
  if (send(sock, nlmsg, nlmsg->nlmsg_len, 0) < 0) {
    perror("send failed");
    return false;
  }

  /* receive response */
  do {
    received_bytes = recv(sock, ptr, sizeof(buffer) - msg_len, 0);
    if (received_bytes < 0) {
      perror("Error in recv");
      return false;
    }

    nlh = (struct nlmsghdr*) ptr;

    /* Check if the header is valid */
    if ((NLMSG_OK(nlmsg, received_bytes) == 0) ||
        (nlmsg->nlmsg_type == NLMSG_ERROR)) {
      perror("Error in received packet");
      return false;
    }

    /* If we received all data break */
    if (nlh->nlmsg_type == NLMSG_DONE)
      break;
    else {
      ptr += received_bytes;
      msg_len += received_bytes;
    }

    /* Break if its not a multi part message */
    if ((nlmsg->nlmsg_flags & NLM_F_MULTI) == 0) break;
  } while ((nlmsg->nlmsg_seq != msgseq) || (nlmsg->nlmsg_pid != getpid()));

  /* parse response */
  for (; NLMSG_OK(nlh, received_bytes); nlh = NLMSG_NEXT(nlh, received_bytes)) {
    /* Get the route data */
    route_entry = (struct rtmsg*) NLMSG_DATA(nlh);

    /* We are just interested in main routing table */
    if (route_entry->rtm_table != RT_TABLE_MAIN) continue;

    route_attribute     = (struct rtattr*) RTM_RTA(route_entry);
    route_attribute_len = RTM_PAYLOAD(nlh);

    /* Loop through all attributes */
    for (; RTA_OK(route_attribute, route_attribute_len);
         route_attribute = RTA_NEXT(route_attribute, route_attribute_len)) {
      switch (route_attribute->rta_type) {
        case RTA_OIF:
          if_indextoname(*(int*) RTA_DATA(route_attribute), interface);
          break;
        case RTA_GATEWAY:
          inet_ntop(
              AF_INET, RTA_DATA(route_attribute), gateway_address,
              sizeof(gateway_address));
          break;
        default:
          break;
      }
    }

    if ((*gateway_address) && (*interface)) {
      gw.assign(gateway_address);
      iface.assign(interface);
      break;
    } else {
      rv = false;
    }
  }
  close(sock);
  return true;
}
