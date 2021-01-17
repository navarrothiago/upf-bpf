include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/toolchain.cmake)
add_subdirectory(bpf)

# add_definitions(-DDEBUG_LIBBPF)

add_library(upf_xdp STATIC
  UserPlaneComponent.cpp
  SignalHandler.cpp
  UPFProgram.cpp
  SessionProgram.cpp
  SessionProgramManager.cpp
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
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/observer>
)

add_dependencies(upf_xdp upf_xdp_bpf_all)

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