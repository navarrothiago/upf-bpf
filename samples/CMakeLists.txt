set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
include_directories(${CMAKE_SOURCE_DIR}/include)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/toolchain.cmake)

# Create xdp_hello_world target.
add_executable(xdp_hello_world xdp_hello_world_user.c)
link_directories(${CMAKE_SOURCE_DIR}/extern/libbpf/src)
target_link_libraries(xdp_hello_world
  ${CMAKE_SOURCE_DIR}/extern/libbpf/src/libbpf.a
  elf
  z
)
add_dependencies(xdp_hello_world xdp_hello_world_bpf)

# Create xdp_redirect_map target.
add_executable(xdp_redirect_map xdp_redirect_map_user.c)
target_link_libraries(xdp_redirect_map
  ${CMAKE_SOURCE_DIR}/extern/libbpf/src/libbpf.a
  elf
  z
)
add_dependencies(xdp_redirect_map xdp_redirect_map_kern)

# Create sockex3 target.
add_executable(sockex3 sockex3_user.c)
target_link_libraries(sockex3
  ${CMAKE_SOURCE_DIR}/extern/libbpf/src/libbpf.a
  elf
  z
)
target_compile_definitions(sockex3 PUBLIC HAVE_ATTR_TEST=0)
add_dependencies(sockex3 sockex3_kern)

# Add bpf subdirectory
add_subdirectory(bpf)
