cmake_minimum_required(VERSION 3.13...3.27)

include(vendor/pico_sdk_import.cmake)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(hello_world C CXX ASM)
pico_sdk_init()

add_executable(hello_world main.c)
target_link_libraries(hello_world PRIVATE pico_stdlib)

pico_enable_stdio_usb(hello_world 1)
pico_add_extra_outputs(hello_world)
pico_enable_filesystem(hello_world)
