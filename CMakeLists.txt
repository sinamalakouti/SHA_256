cmake_minimum_required(VERSION 3.10)
project(SHA_256)

FIND_LIBRARY (PSAPI Psapi)

set(CMAKE_CXX_STANDARD 11)

add_executable(SHA_256 main.cpp)
