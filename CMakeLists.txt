cmake_minimum_required(VERSION 3.10)
project(runc)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(runc 
    src/runc.cpp
    src/uts_namespace.cpp
    src/pid_namespace.cpp
)