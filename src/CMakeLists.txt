include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(upf_xdp_user upf_xdp_user.c)
link_directories(${CMAKE_SOURCE_DIR}/extern/libbpf/src)
target_link_libraries(upf_xdp_user 
  ${CMAKE_SOURCE_DIR}/extern/libbpf/src/libbpf.a
  elf
  z
)

add_subdirectory(bpf)