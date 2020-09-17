set(SKEL_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/skel")
set(SKELETON_FILES
  "${SKEL_DIRECTORY}/upf_xdp_bpf_skel.h"
)
set(OBJ_ROOT_DIR $<TARGET_PROPERTY:upf_xdp_bpf,BINARY_DIR>)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/toolchain.cmake)

add_subdirectory(bpf)

add_custom_target(copy_objs
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:upf_xdp_bpf> ${OBJ_ROOT_DIR}
)

# https://gist.github.com/socantre/7ee63133a0a3a08f3990
add_custom_target(gen_skeleton
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/skel
  COMMAND ${CMAKE_SOURCE_DIR}/bin/bpftool gen skeleton $<TARGET_OBJECTS:upf_xdp_bpf> > ${CMAKE_CURRENT_SOURCE_DIR}/skel/upf_xdp_bpf_skel.h
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

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../extern/libbpf/src)
target_link_libraries(upf_xdp
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../extern/libbpf/src/libbpf.a
  PUBLIC spdlog::spdlog
  PUBLIC elf
  PUBLIC z
)

target_include_directories(upf_xdp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/bpf>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/skel>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/interfaces>
)

# target_include_directories(upf_xdp SYSTEM PUBLIC /usr/include/x86_64-linux-gnu)

add_dependencies(upf_xdp upf_xdp_bpf)
add_dependencies(upf_xdp gen_skeleton)


# Targets:
#   * <prefix>/lib/upf_bpf.a
#   * header location after install: <prefix>/include/*.h
#   * headers can be included by C++ code `#include <*/*.h>`
install(
    TARGETS upf_xdp
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Headers:
#   * ./*.h -> <prefix>/include/*.h
install(
    DIRECTORY "./"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.h"
)

