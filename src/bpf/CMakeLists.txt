include(${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(upf_xdp_bpf OBJECT upf_xdp_bpf.c)

# Copy object to a specific directory.
# https://stackoverflow.com/questions/51988350/copy-out-plain-o-files-with-cmake
set(OBJS $<TARGET_OBJECTS:upf_xdp_bpf>)

message(STATUS "these are my objects: ${OBJS}") # generator expression printed here, not evaluated yet

add_custom_target(print_upf_xdp_bpf_objs 
  COMMAND ${CMAKE_COMMAND} -E echo $<TARGET_OBJECTS:upf_xdp_bpf>)

set(OBJ_ROOT_DIR $<TARGET_PROPERTY:upf_xdp_bpf,BINARY_DIR>) 

add_custom_target(copy_upf_xdp_bpf_objs 
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:upf_xdp_bpf> ${OBJ_ROOT_DIR})

# TODO Rename .c.o 