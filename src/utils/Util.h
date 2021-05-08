#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Util
{
public:
  virtual ~Util();
  static std::vector<u_char> stringToMac(std::string const &s);
  static void getFlowDirection(std::string direction);
  static struct in_addr convertIpToInet(std::string ipAddress);
private:
};
