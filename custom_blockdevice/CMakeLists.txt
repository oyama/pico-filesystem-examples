cmake_minimum_required(VERSION 3.13...3.27)

include(vendor/pico_sdk_import.cmake)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(noisy C CXX ASM)
pico_sdk_init()

add_executable(noisy main.c blockdevice_noisy.c)
target_link_libraries(noisy PRIVATE pico_stdlib)

pico_enable_stdio_usb(noisy 1)
pico_add_extra_outputs(noisy)

pico_enable_filesystem(noisy FS_INIT fs_init.c)
target_link_libraries(noisy PRIVATE
   pico_filesystem
   pico_filesystem_filesystem_littlefs
)
