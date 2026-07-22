cmake_minimum_required(VERSION 3.20)
project(clox CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(clox ${SOURCES})

target_compile_options(clox PRIVATE -Wall -Wextra)

option(CLOX_SANITIZE "Build with ASan + UBSan" OFF)
if(CLOX_SANITIZE)
    target_compile_options(clox PRIVATE -fsanitize=address,undefined -g -fno-omit-frame-pointer)
    target_link_options(clox PRIVATE -fsanitize=address,undefined)
endif()