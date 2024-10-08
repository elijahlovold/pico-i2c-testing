
cmake_minimum_required(VERSION 3.13)

# Initialize the SDK
include(pico_sdk_import.cmake)

# Set the project name
project(slave_i2c C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(slave_i2c
    main.c
    )


target_link_libraries(slave_i2c
    pico_i2c_slave
    hardware_i2c
    pico_stdlib
    pico_rand
    )

# enable usb output, disable uart output
pico_enable_stdio_usb(slave_i2c 1)
pico_enable_stdio_uart(slave_i2c 0)


pico_add_extra_outputs(slave_i2c)
