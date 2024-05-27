cmake_minimum_required(VERSION 3.13...3.27)

option(WITHOUT_BLOCKDEVICE_SD "without SD block device in demo and tests")

include(vendor/pico_sdk_import.cmake)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(benchmark C CXX ASM)
pico_sdk_init()

add_executable(benchmark main.c)
target_link_libraries(benchmark PRIVATE
    pico_stdlib
    pico_filesystem
    pico_filesystem_blockdevice_sd
    pico_filesystem_blockdevice_flash
    pico_filesystem_filesystem_fat
    pico_filesystem_filesystem_littlefs
    )
if(WITHOUT_BLOCKDEVICE_SD)
    target_compile_definitions(benchmark PRIVATE WITHOUT_BLOCKDEVICE_SD=1)
endif()
pico_enable_stdio_usb(benchmark 1)
pico_add_extra_outputs(benchmark)

