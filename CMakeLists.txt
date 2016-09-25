cmake_minimum_required(VERSION 3.5)
project(A3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES boardstate.cpp main.cpp basic.cpp evaluate.cpp)
add_executable(A3 ${SOURCE_FILES})