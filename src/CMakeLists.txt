include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/bpf)
include_directories(${CMAKE_SOURCE_DIR}/build/src/bpf/skel)

add_subdirectory(bpf)

add_executable(upf_xdp_user upf_xdp_user.c)
add_dependencies(upf_xdp_user upf_xdp_bpf)
add_dependencies(upf_xdp_user gen_skeleton)

link_directories(${CMAKE_SOURCE_DIR}/extern/libbpf/src)
target_link_libraries(upf_xdp_user 
  ${CMAKE_SOURCE_DIR}/extern/libbpf/src/libbpf.a
  elf
  z
)
