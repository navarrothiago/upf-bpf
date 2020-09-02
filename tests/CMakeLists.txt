include_directories(interfaces)
add_subdirectory(interfaces)

# The extra path here is needed to correct the build path because we are calling it from a subdirectory.
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

# Keeping IDEs that support folders clean
set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/bpf)
include_directories(${CMAKE_SOURCE_DIR}/build/src/bpf/skel)

add_executable(UPFProgramTests
  SessionManagerTests.cpp
  interfaces/ForwardingActionRulesImpl.cpp
  interfaces/PacketDetectionRulesImpl.cpp
  interfaces/RulesUtilitiesImpl.cpp
)
add_test(UPFProgramTests UPFProgramTests)
add_dependencies(UPFProgramTests upf_xdp)

add_custom_target(copy_bpf_program
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_OBJECTS:upf_xdp_bpf> ${CMAKE_CURRENT_BINARY_DIR}/bpf
)

link_directories(${CMAKE_SOURCE_DIR}/extern/libbpf/src)
target_link_libraries(UPFProgramTests
  ${CMAKE_SOURCE_DIR}/extern/libbpf/src/libbpf.a
  spdlog::spdlog
  elf
  z
  gtest
  gtest_main
  upf_xdp
)
