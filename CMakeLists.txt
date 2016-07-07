cmake_minimum_required(VERSION 3.5)
project(clang_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp)
add_executable(clang_test ${SOURCE_FILES})