include(${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(xdp_hello_world_bpf OBJECT xdp_hello_world_bpf.c)

# Copy object to a specific directory.
# https://stackoverflow.com/questions/51988350/copy-out-plain-o-files-with-cmake
set(OBJS $<TARGET_OBJECTS:xdp_hello_world_bpf>)

message(STATUS "these are my objects: ${OBJS}") # generator expression printed here, not evaluated yet

add_custom_target(print_samples_objs 
  COMMAND ${CMAKE_COMMAND} -E echo $<TARGET_OBJECTS:xdp_hello_world_bpf>)

set(OBJ_SAMPLE_ROOT_DIR $<TARGET_PROPERTY:xdp_hello_world_bpf,BINARY_DIR>) 

add_custom_target(copy_samples_objs 
  # COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJ_SAMPLE_ROOT_DIR}/bpf_obj/
  # COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:xdp_hello_world_bpf> ${OBJ_SAMPLE_ROOT_DIR}/bpf_obj/)
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:xdp_hello_world_bpf> ${OBJ_SAMPLE_ROOT_DIR})

# TODO Rename .c.o 