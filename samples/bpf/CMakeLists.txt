set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -target bpf")
include_directories(${CMAKE_SOURCE_DIR}/include)
# add_definitions(-DKERNEL_SPACE)

add_library(xdp_hello_world_bpf OBJECT xdp_hello_world_bpf.c)
add_library(xdp_redirect_map_kern OBJECT xdp_redirect_map_kern.c)
add_library(sockex3_kern OBJECT sockex3_kern.c)

# Copy object to a specific directory.
# https://stackoverflow.com/questions/51988350/copy-out-plain-o-files-with-cmake
set(OBJS $<TARGET_OBJECTS:xdp_hello_world_bpf>)
set(OBJ_REDIRECT $<TARGET_OBJECTS:xdp_redirect_map_kern>)
set(OBJ_TAIL $<TARGET_OBJECTS:sockex3_kern>)

# generator expression printed here, not evaluated yet
message(STATUS "these are my objects: ${OBJS}")
message(STATUS "these are my objects: ${OBJ_REDIRECT}")
message(STATUS "these are my objects: ${OBJ_TAIL}")

add_custom_target(print_samples_objs
  COMMAND ${CMAKE_COMMAND} -E echo $<TARGET_OBJECTS:xdp_hello_world_bpf>
  COMMAND ${CMAKE_COMMAND} -E echo $<TARGET_OBJECTS:xdp_redirect_map_kern>
  COMMAND ${CMAKE_COMMAND} -E echo $<TARGET_OBJECTS:sockex3_kern>
)

set(OBJ_SAMPLE_ROOT_DIR )

add_custom_target(copy_samples_objs
  # COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJ_SAMPLE_ROOT_DIR}/bpf_obj/
  # COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:xdp_hello_world_bpf> ${OBJ_SAMPLE_ROOT_DIR}/bpf_obj/)
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:xdp_hello_world_bpf> $<TARGET_PROPERTY:xdp_hello_world_bpf,BINARY_DIR>
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:xdp_redirect_map_kern>  $<TARGET_PROPERTY:xdp_redirect_map_kern,BINARY_DIR>
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:sockex3_kern> $<TARGET_PROPERTY:sockex3_kern,BINARY_DIR>
)

# TODO Rename .c.o
