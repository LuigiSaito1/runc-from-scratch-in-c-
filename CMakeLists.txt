cmake_minimum_required(VERSION 3.10)
project(runc)

set(CMAKE_C_STANDARD 11)

add_executable(runc src/runc.cpp) 