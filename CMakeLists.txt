# TODO navarrothiago - Check version
cmake_minimum_required(VERSION 3.10)

project(upf-bpf)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "CMAKE={${CMAKE_SYSTEM_PROCESSOR}}")

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/extern/libbpf/src AND ${CMAKE_CURRENT_SOURCE_DIR}/extern/spdlog)
  execute_process(COMMAND git submodule update --init --recursive
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

# Set global property (all targets are impacted)
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

add_subdirectory(samples)
add_subdirectory(src)


option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

