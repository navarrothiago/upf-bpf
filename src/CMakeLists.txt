set(SKEL_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/skel")
set(SKELETON_FILES
  "${CMAKE_CURRENT_BINARY_DIR}/upf_xdp_bpf_skel.h"
)
set(OBJ_ROOT_DIR $<TARGET_PROPERTY:upf_xdp_bpf,BINARY_DIR>) 

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/bpf)
include_directories(${CMAKE_SOURCE_DIR}/build/src/skel)
include_directories(interfaces)
include_directories(/usr/include/x86_64-linux-gnu)


add_subdirectory(bpf)

add_custom_target(copy_objs 
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:upf_xdp_bpf> ${OBJ_ROOT_DIR}
)

# https://gist.github.com/socantre/7ee63133a0a3a08f3990
add_custom_target(gen_skeleton
  COMMAND ${CMAKE_COMMAND} -E make_directory skel
  COMMAND ${CMAKE_SOURCE_DIR}/bin/bpftool gen skeleton $<TARGET_OBJECTS:upf_xdp_bpf> > ${SKEL_DIRECTORY}/upf_xdp_bpf_skel.h
  DEPENDS copy_objs
)

add_custom_target(print_objs 
  COMMAND ${CMAKE_COMMAND} -E echo $<TARGET_OBJECTS:upf_xdp_bpf> 
)

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
