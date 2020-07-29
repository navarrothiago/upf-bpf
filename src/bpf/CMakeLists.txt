set(SKEL_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/skel")
set(SKELETON_FILES
  "${CMAKE_CURRENT_BINARY_DIR}/upf_xdp_bpf_skel.h"
)
set(OBJ_ROOT_DIR $<TARGET_PROPERTY:upf_xdp_bpf,BINARY_DIR>) 

include(${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(upf_xdp_bpf OBJECT upf_xdp_bpf.c)

# Copy object to a specific directory.
# https://stackoverflow.com/questions/51988350/copy-out-plain-o-files-with-cmake
# https://stackoverflow.com/questions/35696103/cmake-wildcard-for-target-objects
# https://stackoverflow.com/questions/26024235/how-to-call-a-cmake-function-from-add-custom-target-command

message(STATUS "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_target(copy_objs 
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:upf_xdp_bpf> ${OBJ_ROOT_DIR}
)

# https://gist.github.com/socantre/7ee63133a0a3a08f3990
add_custom_target(gen_skeleton
  COMMAND ${CMAKE_COMMAND} -E make_directory skel
  COMMAND bpftool gen skeleton $<TARGET_OBJECTS:upf_xdp_bpf> > ${SKEL_DIRECTORY}/upf_xdp_bpf_skel.h
  DEPENDS copy_objs
)

add_custom_target(print_objs 
  COMMAND ${CMAKE_COMMAND} -E echo $<TARGET_OBJECTS:upf_xdp_bpf> 
)

# TODO Rename .c.o 