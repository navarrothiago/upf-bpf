#include <stdint.h>
#include <string>
#include <vector>

class Util
{
public:
  static Util& getInstance();
  virtual ~Util();
  std::vector<u_char> stringToMac(std::string const &s);
};
