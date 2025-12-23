cmake_minimum_required(VERSION 3.10)
project(runc)

set(CMAKE_CXX_STANDARD 11)

# Inclui o diretório de headers
include_directories(include)

# Adiciona o executável com todos os arquivos fonte
add_executable(runc 
    src/runc.cpp
    src/uts_namespace.cpp
)