cmake_minimum_required(VERSION 3.13...3.27)

include(vendor/pico_sdk_import.cmake)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(tests C CXX ASM)
pico_sdk_init()

add_executable(logger
    main.c
    usb_msc.c
    usb_descriptors.c
)
target_include_directories(logger PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_options(logger PRIVATE -Os)
target_link_libraries(logger PRIVATE
    hardware_adc
    pico_filesystem
    pico_filesystem_blockdevice_flash
    pico_filesystem_filesystem_fat
    pico_filesystem_filesystem_littlefs
    pico_stdlib
    tinyusb_board
    tinyusb_device
)
pico_enable_stdio_usb(logger 1)
pico_add_extra_outputs(logger)
