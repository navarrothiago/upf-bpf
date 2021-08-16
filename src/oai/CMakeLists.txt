add_subdirectory(common)

add_library(oai STATIC
  pfcp/pfcp_far.cpp
  pfcp/pfcp_pdr.cpp
  pfcp/pfcp_session.cpp
)

target_include_directories(oai PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/common
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/common/utils
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/pfcp
)

install(
    TARGETS oai
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
