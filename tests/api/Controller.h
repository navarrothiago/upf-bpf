#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <bpf/ie/apply_action.h>
#include <bpf/ie/fseid.h>
#include <httplib.h>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <types.h>

using json = nlohmann::json;

class Controller
{
public:
  Controller(/* args */);
  virtual ~Controller();
  static int setup(json &jRequest, json &jResponse);
  static int createSesssion(json &jRequest, json &jResponse);
};

#endif // __CONTROLLER_H__
