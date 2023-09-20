# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# NOTE: Add module paths variable
set(QPC ${CMAKE_CURRENT_SOURCE_DIR}/qpc)                                    # QPC framework
set(APP_UART ${CMAKE_CURRENT_SOURCE_DIR}/ncs-uart-handler/modules/app_uart) # Async UART lib

# Append to Zephyr extra modules
#####################################################
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr_obd)

target_sources(app PRIVATE src/main.c)

# Custom files and folders
target_sources(app PRIVATE
src/ble/bluetoothle.c
    
    src/ble/bluetoothle.c
    src/ble/ble_gatt.c
    src/driver/hal.c
    src/driver/hal_adc.c
    src/driver/hal_gpio.c
    src/driver/hal_i2c.c
    src/driver/hal_pwm.c
    src/driver/hal_uart.c
)

zephyr_library_include_directories(src/ble)
zephyr_library_include_directories(src/driver)



