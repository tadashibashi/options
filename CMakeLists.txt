cmake_minimum_required(VERSION 3.16)
project(options_test)

set(CMAKE_CXX_STANDARD 17)
add_executable(options_test test.cpp options.hpp)

