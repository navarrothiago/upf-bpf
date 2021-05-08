#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <bpf/ie/apply_action.h>
#include <bpf/ie/fseid.h>
#include <httplib.h>
#include <map>
#include <nlohmann/json.hpp>
#include <types.h>
#include <string>

using json = nlohmann::json;

class Controller
{
public:
  Controller(/* args */);
  virtual ~Controller();
  static int setup(json jBody);
  static int createSesssion(json jBody);

// private:
//   static std::map<std::string, FlowDirection> sMapFlowDirection; 
};

#endif // __CONTROLLER_H__