include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DKERNEL_SPACE)

add_custom_target(upf_xdp_bpf_all DEPENDS upf_xdp_bpf session_bpf far_bpf)

function(bpf prefix)
  add_library(${prefix}_bpf ${prefix}_bpf.c)
  add_custom_command(
    TARGET ${prefix}_bpf
    PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/../skel
    COMMAND ${CMAKE_SOURCE_DIR}/bin/bpftool gen skeleton $<TARGET_OBJECTS:${prefix}_bpf> > ${CMAKE_CURRENT_SOURCE_DIR}/../skel/${prefix}_bpf_skel.h
    COMMAND sed -i '9i\#define typeof\(x\) __typeof__\(x\)' ${CMAKE_CURRENT_SOURCE_DIR}/../skel/${prefix}_bpf_skel.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${prefix}_bpf.c
  )
endfunction()

bpf(upf_xdp)
bpf(session)
bpf(far)
