cmake_minimum_required(VERSION 3.14)
project(test2)

set(CMAKE_CXX_STANDARD 11)

add_executable(test2 main.cpp src/Base64Stream.cpp src/Base64Stream.h)