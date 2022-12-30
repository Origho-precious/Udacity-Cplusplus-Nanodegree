cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 17)

project(cmake_tutorial)

add_executable(run src/main.cpp src/vect_add_one.cpp src/increment_and_sum.cpp)