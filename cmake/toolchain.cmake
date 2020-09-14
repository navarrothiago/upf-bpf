set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_BUILD_TYPE Release)

# Clang target triple
SET(triple bpf)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})

# TODO Remove -target bpf
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -target bpf")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall ${CXX_ISYSTEM_DIRS} -fsanitize=address -O1 -fno-omit-frame-pointer -fPIC" )

set(CMAKE_OBJCOPY llvm-objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL llvm-size CACHE INTERNAL "size tool")

