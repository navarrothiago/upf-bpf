# TODO navarrothiago - Check version
cmake_minimum_required(VERSION 3.0.0)

project(UpfBpf VERSION 1.0.0)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "CMAKE={${CMAKE_SYSTEM_PROCESSOR}}")

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/extern/libbpf/src
    AND ${CMAKE_CURRENT_SOURCE_DIR}/extern/spdlog
    AND ${CMAKE_CURRENT_SOURCE_DIR}/extern/cpp-httplib
    AND ${CMAKE_CURRENT_SOURCE_DIR}/extern/json)
  execute_process(COMMAND git submodule update --init --recursive
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(NOT TARGET spdlog)
  # Stand-alone build
  find_package(spdlog REQUIRED)
endif()

if(NOT TARGET httplib)
  # Stand-alone build
  find_package(httplib REQUIRED)
endif()

if(NOT TARGET nlohmann_json)
  # Stand-alone build
  find_package(nlohmann_json 3.9.1 REQUIRED)
endif()

# Set global property (all targets are impacted)
# set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

# Introduce variables:
# * CMAKE_INSTALL_LIBDIR
# * CMAKE_INSTALL_BINDIR
# * CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs)

# Layout. This works for all platforms:
#   * <prefix>/lib*/cmake/<PROJECT-NAME>
#   * <prefix>/lib*/
#   * <prefix>/include/
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Use:
#   * PROJECT_VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

set(INCLUDE_INSTALL_DIR include/)
set(LIB_INSTALL_DIR lib/ )
# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * TARGETS_EXPORT_NAME
#   * PROJECT_NAME
configure_package_config_file(
    "cmake/${PROJECT_NAME}Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

# Config
#   * <prefix>/lib/cmake/UpfBpf/UpfBpfConfig.cmake
#   * <prefix>/lib/cmake/UpfBpf/UpfBpfConfigVersion.cmake
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Config
#   * <prefix>/lib/cmake/UpfBpf/UpfBpfTargets.cmake
install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

install(FILES Makefile DESTINATION "." )

add_subdirectory(samples)
add_subdirectory(src)
