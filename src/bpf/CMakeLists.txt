include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DKERNEL_SPACE)
add_library(upf_xdp_bpf upf_xdp_bpf.c)

