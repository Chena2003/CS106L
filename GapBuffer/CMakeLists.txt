
cmake_minimum_required(VERSION 3.15)
project(gap_buffer)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wall")

# 添加 GoogleTest 的子目录
add_subdirectory(./googletest)

enable_testing()

add_executable(
  gap_buffer_test
  gap_buffer_test.cc
)
target_link_libraries(
  gap_buffer_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(gap_buffer_test)