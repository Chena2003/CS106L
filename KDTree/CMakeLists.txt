cmake_minimum_required(VERSION 3.14)
project(KDTree)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -Wall")

# 添加 GoogleTest 的子目录
add_subdirectory(./googletest)

enable_testing()

add_executable(kd_tree_test kd_tree_test.cc)
target_link_libraries(kd_tree_test GTest::gtest_main)


include(GoogleTest)
gtest_discover_tests(kd_tree_test)
