cmake_minimum_required(VERSION 3.13...3.27)

include(vendor/pico_sdk_import.cmake)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(pico-filesystem-example C CXX ASM)
pico_sdk_init()

add_executable(pico-filesystem main.c)
target_link_libraries(pico-filesystem PRIVATE pico_stdlib)

pico_enable_stdio_usb(pico-filesystem 1)
pico_enable_filesystem(pico-filesystem)

# pico_enable_filesystem(pico-filesystem FS_INIT my_fs_init.c)
# target_link_libraries(pico-filesystem PRIVATE
#    pico_stdlib
#    pico_filesystem
#    pico_filesystem_blockdevice_sd
#    pico_filesystem_blockdevice_flash
#    pico_filesystem_filesystem_fat
#    pico_filesystem_filesystem_littlefs
#)
