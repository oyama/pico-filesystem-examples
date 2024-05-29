cmake_minimum_required(VERSION 3.13...3.27)

include(vendor/pico_sdk_import.cmake)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(ram_disk C CXX ASM)
pico_sdk_init()

add_executable(ram_disk main.c)
target_link_libraries(ram_disk PRIVATE pico_stdlib)

pico_enable_stdio_usb(ram_disk 1)
pico_add_extra_outputs(ram_disk)
pico_enable_filesystem(ram_disk FS_INIT mount_ram_disk.c)

# If FS_INIT is specified, the library to be used must be added
target_link_libraries(ram_disk PRIVATE
    pico_filesystem
    pico_filesystem_blockdevice_heap
    pico_filesystem_filesystem_littlefs
)

