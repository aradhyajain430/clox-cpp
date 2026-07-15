cmake_minimum_required(VERSION 3.20)
project(clox CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES "*.cpp")
add_executable(clox ${SOURCES})
