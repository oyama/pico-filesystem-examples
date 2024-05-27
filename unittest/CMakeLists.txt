cmake_minimum_required(VERSION 3.13...3.27)

option(WITHOUT_BLOCKDEVICE_SD "without SD block device in tests")

include(vendor/pico_sdk_import.cmake)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

project(tests C CXX ASM)
pico_sdk_init()

add_executable(tests
    main.c
    test_blockdevice.c
    test_filesystem.c
    test_vfs.c
    test_stdio.c
    test_copy_between_different_filesystems.c
)
target_link_libraries(tests PRIVATE
    pico_stdlib
    pico_filesystem
    pico_filesystem_blockdevice_sd
    pico_filesystem_blockdevice_flash
    pico_filesystem_filesystem_fat
    pico_filesystem_filesystem_littlefs
)
target_compile_options(tests PRIVATE -Werror -Wall -Wextra -Wnull-dereference)
if(WITHOUT_BLOCKDEVICE_SD)
    target_compile_definitions(tests PRIVATE WITHOUT_BLOCKDEVICE_SD=1)
endif()

pico_enable_stdio_usb(tests 1)
pico_add_extra_outputs(tests)
