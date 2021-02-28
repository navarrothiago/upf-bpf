#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <types.h>
#include <bpf/ie/fseid.h>
#include <bpf/ie/apply_action.h>

using json = nlohmann::json;

class Controller
{
public:
  Controller(/* args */);
  virtual ~Controller();
  static int setup(json jBody);
  static int createSesssion(json jBody);
};

#endif // __CONTROLLER_H__