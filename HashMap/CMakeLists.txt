cmake_minimum_required(VERSION 3.15)
project(hashmap)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O2 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wall")

# 添加 GoogleTest 的子目录
add_subdirectory(./googletest)

enable_testing()

add_executable(
  hashmap_test
  hashmap_test.cpp
)
target_link_libraries(
  hashmap_test
  gtest_main
)

add_executable(
    hashmap_perf
    hashmap_perf.cpp
)

target_link_libraries(
  hashmap_perf
  gtest_main
)

add_executable(
    mytest
    main.cpp
)

include(GoogleTest)
gtest_discover_tests(hashmap_test)
