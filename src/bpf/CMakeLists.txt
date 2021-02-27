include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DKERNEL_SPACE)

add_custom_target(upf_xdp_bpf_all DEPENDS upf_xdp_bpf session_bpf)

add_library(upf_xdp_bpf upf_xdp_bpf.c)
add_library(session_bpf session_bpf.c)

add_custom_command(
  TARGET upf_xdp_bpf
  PRE_LINK
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/../skel
  COMMAND ${CMAKE_SOURCE_DIR}/bin/bpftool gen skeleton $<TARGET_OBJECTS:upf_xdp_bpf> > ${CMAKE_CURRENT_SOURCE_DIR}/../skel/upf_xdp_bpf_skel.h
  COMMAND sed -i '9i\#define typeof\(x\) __typeof__\(x\)' ${CMAKE_CURRENT_SOURCE_DIR}/../skel/upf_xdp_bpf_skel.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/upf_xdp_bpf.c
)

add_custom_command(
  TARGET session_bpf
  PRE_LINK
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/../skel
  COMMAND ${CMAKE_SOURCE_DIR}/bin/bpftool gen skeleton $<TARGET_OBJECTS:session_bpf> > ${CMAKE_CURRENT_SOURCE_DIR}/../skel/session_bpf_skel.h
  COMMAND sed -i '9i\#define typeof\(x\) __typeof__\(x\)' ${CMAKE_CURRENT_SOURCE_DIR}/../skel/session_bpf_skel.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/session_bpf.c
)
