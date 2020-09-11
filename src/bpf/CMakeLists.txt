include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DKERNEL_SPACE)

add_library(upf_xdp_bpf OBJECT upf_xdp_bpf.c)

# Copy object to a specific directory.
# https://stackoverflow.com/questions/51988350/copy-out-plain-o-files-with-cmake
# https://stackoverflow.com/questions/35696103/cmake-wildcard-for-target-objects
# https://stackoverflow.com/questions/26024235/how-to-call-a-cmake-function-from-add-custom-target-command

message(STATUS "${CMAKE_CURRENT_BINARY_DIR}")



# TODO Rename .c.o
