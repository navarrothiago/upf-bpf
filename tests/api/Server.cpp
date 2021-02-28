#include "Controller.h"
#include <UserPlaneComponent.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <utils/LogDefines.h>

using json = nlohmann::json;

int main(int argc, char *argv[])
{
  using namespace httplib;

  Server svr;
  uint32_t port = 1234;

  // Check the number of parameters
  if(argc < 2) {
    // Tell the user how to run the program
    std::cerr << "Usage: " << argv[0] << " <ENDPOINT>" << std::endl;
    return 1;
  }

  LOG_INF("Running server on endpoint {}:{}", argv[1], port);

  svr.Post("/configure", [](const Request& req, Response& res) {
    json jBody = json::parse(req.body);
    res.status = Controller::setup(jBody);
    res.set_content("{\"teste\": \"Hello World!\"s}", "application/json");
  });

  svr.Post("/createSession", [](const Request &req, Response &res) {
    json jBody = json::parse(req.body);
    res.status = Controller::createSesssion(jBody);
    res.set_content("{\"teste\": \"Hello World!\"s}", "application/json");
  });

  svr.listen("localhost", port);
}