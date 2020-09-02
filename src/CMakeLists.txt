include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/bpf)
include_directories(${CMAKE_SOURCE_DIR}/build/src/bpf/skel)
include_directories(interfaces)


add_subdirectory(bpf)

add_library(upf_xdp STATIC
  UPFProgramManager.cpp
  SessionManager.cpp
  wrappers/BPFMaps.cpp
  wrappers/BPFMap.cpp
  utils/Logger.cpp
)

add_dependencies(upf_xdp upf_xdp_bpf)
add_dependencies(upf_xdp gen_skeleton)

link_directories(${CMAKE_SOURCE_DIR}/extern/libbpf/src)

target_link_libraries(upf_xdp
  ${CMAKE_SOURCE_DIR}/extern/libbpf/src/libbpf.a
  spdlog::spdlog
  elf
  z
)
