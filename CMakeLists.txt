cmake_minimum_required(VERSION 2.8.7)

project(upf-bpf)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/extern/libbpf/src)
  execute_process(COMMAND git submodule update --init --recursive
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_subdirectory(samples)
add_subdirectory(src)