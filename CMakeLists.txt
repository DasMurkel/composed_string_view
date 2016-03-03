cmake_minimum_required (VERSION 2.8)
project (Tutorial)

#include_directories(/home/matze/workspace/libcxx/include)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_COMPILER /usr/bin/clang)

include_directories(/home/matze/workspace/llvm/libcxx/build/include/c++/v1)
include_directories(/home/matze/workspace/utility/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
set(CMAKE_CXX_DEBUG_FLAGS "${CMAKE_CXX_DEBUG_FLAGS} -g")

MESSAGE(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

set(CSV_FILES
    test
)

add_executable(csv ${CSV_FILES})
