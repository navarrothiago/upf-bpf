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
#include "common_defs.h"
#include "if.hpp"
#include "logger.hpp"

#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>

#define BUFFER_SIZE 4096

//------------------------------------------------------------------------------
int get_gateway_and_iface(std::string* gw, std::string* iface) {
  int received_bytes = 0, msg_len = 0, route_attribute_len = 0;
  int sock = -1, msgseq = 0;
  struct nlmsghdr *nlh, *nlmsg;
  struct rtmsg* route_entry;
  // This struct contain route attributes (route type)
  struct rtattr* route_attribute;
  char gateway_address[INET_ADDRSTRLEN], interface[IF_NAMESIZE];
  char msgbuf[BUFFER_SIZE], buffer[BUFFER_SIZE];
  char* ptr = buffer;
  struct timeval tv;
  int rv = RETURNok;

  if ((sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) < 0) {
    Logger::system().error("socket raw/NETLINK_ROUTE failed");
    return EXIT_FAILURE;
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
    Logger::system().error("send socket raw/NETLINK_ROUTE failed");
    return EXIT_FAILURE;
  }

  /* receive response */
  do {
    received_bytes = recv(sock, ptr, sizeof(buffer) - msg_len, 0);
    if (received_bytes < 0) {
      Logger::system().error("recv socket raw/NETLINK_ROUTE failed");
      return EXIT_FAILURE;
    }

    nlh = (struct nlmsghdr*) ptr;

    /* Check if the header is valid */
    if ((NLMSG_OK(nlmsg, received_bytes) == 0) ||
        (nlmsg->nlmsg_type == NLMSG_ERROR)) {
      Logger::system().error("recv msg raw/NETLINK_ROUTE failed");
      return EXIT_FAILURE;
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
      *gw = std::string(gateway_address);
      if (iface) {
        *iface = std::string(interface);
      }
      break;
    } else {
      rv = RETURNerror;
    }
  }
  close(sock);
  return rv;
}

//------------------------------------------------------------------------------
int get_inet_addr_from_iface(
    const std::string& if_name, struct in_addr& inet_addr) {
  struct ifreq ifr;
  char str[INET_ADDRSTRLEN];

  memset(&ifr, 0, sizeof(ifr));
  int fd                 = socket(AF_INET, SOCK_DGRAM, 0);
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, (const char*) if_name.c_str(), IFNAMSIZ - 1);
  if (ioctl(fd, SIOCGIFADDR, &ifr)) {
    close(fd);
    Logger::system().error(
        "Failed to probe %s inet addr: error %s\n", if_name.c_str(),
        strerror(errno));
    return RETURNerror;
  }
  close(fd);
  struct sockaddr_in* ipaddr = (struct sockaddr_in*) &ifr.ifr_addr;
  // check
  if (inet_ntop(
          AF_INET, (const void*) &ipaddr->sin_addr, str, INET_ADDRSTRLEN) ==
      NULL) {
    return RETURNerror;
  }
  inet_addr.s_addr = ipaddr->sin_addr.s_addr;
  return RETURNok;
}

//------------------------------------------------------------------------------
int get_mtu_from_iface(const std::string& if_name, uint32_t& mtu) {
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  int fd                 = socket(AF_INET, SOCK_DGRAM, 0);
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, (const char*) if_name.c_str(), IFNAMSIZ - 1);
  if (ioctl(fd, SIOCGIFMTU, &ifr)) {
    close(fd);
    Logger::system().error(
        "Failed to probe %s MTU: error %s\n", if_name.c_str(), strerror(errno));
    return RETURNerror;
  }
  close(fd);
  mtu = ifr.ifr_mtu;
  return RETURNok;
}

//------------------------------------------------------------------------------
int get_inet_addr_infos_from_iface(
    const std::string& if_name, struct in_addr& inet_addr,
    struct in_addr& inet_network, unsigned int& mtu) {
  struct ifreq ifr;
  char str[INET_ADDRSTRLEN];

  inet_addr.s_addr    = INADDR_ANY;
  inet_network.s_addr = INADDR_ANY;
  mtu                 = 0;

  memset(&ifr, 0, sizeof(ifr));
  int fd                 = socket(AF_INET, SOCK_DGRAM, 0);
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, (const char*) if_name.c_str(), IFNAMSIZ - 1);
  if (ioctl(fd, SIOCGIFADDR, &ifr)) {
    close(fd);
    Logger::system().error(
        "Failed to probe %s inet addr: error %s\n", if_name.c_str(),
        strerror(errno));
    return RETURNerror;
  }
  struct sockaddr_in* ipaddr = (struct sockaddr_in*) &ifr.ifr_addr;
  // check
  if (inet_ntop(
          AF_INET, (const void*) &ipaddr->sin_addr, str, INET_ADDRSTRLEN) ==
      NULL) {
    close(fd);
    return RETURNerror;
  }
  inet_addr.s_addr = ipaddr->sin_addr.s_addr;

  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, (const char*) if_name.c_str(), IFNAMSIZ - 1);
  if (ioctl(fd, SIOCGIFNETMASK, &ifr)) {
    close(fd);
    Logger::system().error(
        "Failed to probe %s inet netmask: error %s\n", if_name.c_str(),
        strerror(errno));
    return RETURNerror;
  }
  ipaddr = (struct sockaddr_in*) &ifr.ifr_netmask;
  // check
  if (inet_ntop(
          AF_INET, (const void*) &ipaddr->sin_addr, str, INET_ADDRSTRLEN) ==
      NULL) {
    close(fd);
    return RETURNerror;
  }
  inet_network.s_addr = ipaddr->sin_addr.s_addr;

  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_addr.sa_family = AF_INET;
  strncpy(ifr.ifr_name, (const char*) if_name.c_str(), IFNAMSIZ - 1);
  if (ioctl(fd, SIOCGIFMTU, &ifr)) {
    Logger::system().error(
        "Failed to probe %s MTU: error %s\n", if_name.c_str(), strerror(errno));
  } else {
    mtu = ifr.ifr_mtu;
  }
  close(fd);
  return RETURNok;
}
