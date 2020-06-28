include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(xdp_hello_world xdp_hello_world_user.c)
link_directories(${CMAKE_SOURCE_DIR}/extern/libbpf/src)
target_link_libraries(xdp_hello_world 
  ${CMAKE_SOURCE_DIR}/extern/libbpf/src/libbpf.a
  elf
  z
)

add_subdirectory(bpf)