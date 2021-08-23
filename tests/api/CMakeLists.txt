include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../
)

add_executable(api
  Server.cpp
  Controller.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/ForwardingActionRulesImpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/PacketDetectionRulesImpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/RulesUtilitiesImpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../interfaces/SessionBpfImpl.cpp
)

target_link_libraries(api
  PRIVATE httplib::httplib
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE upf_xdp
  asan
  spdlog::spdlog
)

install(
  TARGETS api
  RUNTIME
)
