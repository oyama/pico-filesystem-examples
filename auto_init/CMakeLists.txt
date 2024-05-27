cmake_minimum_required(VERSION 3.13...3.27)

include(vendor/pico_sdk_import.cmake)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(auto_init C CXX ASM)
pico_sdk_init()

add_executable(auto_init main.c)
target_link_libraries(auto_init PRIVATE pico_stdlib)
pico_enable_stdio_usb(auto_init 1)
pico_add_extra_outputs(auto_init)

# If `AUTO_INIT` is enabled, `fs_init()` is executed in `pre_main()`.
pico_enable_filesystem(auto_init AUTO_INIT YES)

# Additional sources of initialisation functions can be added to customise the file system configuration.
# pico_enable_filesystem(auto_init AUTO_INIT YES FS_INIT my_fs_init.c)
# target_link_libraries(auto_init PRIVATE
#     pico_stdlib
#     pico_filesystem
#     pico_filesystem_blockdevice_flash
#     pico_filesystem_blockdevice_sd
#     pico_filesystem_filesystem_littlefs
#     pico_filesystem_filesystem_fat
#     )
