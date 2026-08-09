cmake_minimum_required(VERSION 3.16)
project(chip8 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)

add_executable(chip8
    src/main.c
)

include_directories(include/)

target_link_libraries(chip8 PRIVATE
    SDL3::SDL3
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(chip8 PRIVATE)
endif()