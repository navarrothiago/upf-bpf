#include "Util.h"
#include <algorithm>
#include <cstdint>
#include <stdlib.h>
#include "LogDefines.h"
#include <sstream>
#include <iostream>

Util::~Util() { LOG_FUNC(); }

// C - https://stackoverflow.com/a/7326381/2203249
// C++ - https://stackoverflow.com/a/34949247/2203249
std::vector<u_char> Util::stringToMac(std::string const &s)
{
  std::vector<u_char> macAddress(6);
  u_char *a = macAddress.data();
  std::stringstream ss;
  int last = -1;
  int rc = sscanf(s.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%n", a + 0, a + 1, a + 2, a + 3, a + 4, a + 5, &last);
  for (auto i: macAddress){
    std::cout << "mac address: " << std::hex << i << std::endl;
  }
  LOG_DBG("mac address: {}", ss.str());
  if(rc != 6 || s.size() != last)
    throw std::runtime_error("invalid mac address format " + s);
  return macAddress;
}

struct in_addr Util::convertIpToInet(std::string ipAddress){
  struct in_addr inetIpAddress;
  if(inet_aton(ipAddress.c_str(), &inetIpAddress) == 0) {
    fprintf(stderr, "Invalid address\n");
    throw std::runtime_error("Invalid address");
  }
  return inetIpAddress;
}

