include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/toolchain.cmake)
add_subdirectory(bpf)

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

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/skel/upf_xdp_bpf_skel.h
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/skel
  COMMAND ${CMAKE_SOURCE_DIR}/bin/bpftool gen skeleton $<TARGET_OBJECTS:upf_xdp_bpf> > ${CMAKE_CURRENT_SOURCE_DIR}/skel/upf_xdp_bpf_skel.h
  COMMAND sed -i '9i\#define typeof\(x\) __typeof__\(x\)' ${CMAKE_CURRENT_SOURCE_DIR}/skel/upf_xdp_bpf_skel.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/bpf/upf_xdp_bpf.c
)

add_custom_target(gen_skeleton ALL
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/skel/upf_xdp_bpf_skel.h 
)

# Building a target (upf_target) that depends on a custom target (gen_skeleton)
# that depends on a custom command output (see ./bpf/CMakeLists.txt).
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