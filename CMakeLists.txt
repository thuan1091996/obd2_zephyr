# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# NOTE: Add module paths variable
set(QPC ${CMAKE_CURRENT_SOURCE_DIR}/qpc)                                    # QPC framework

# Append to Zephyr extra modules
set(ZEPHYR_EXTRA_MODULES ${QPC} )
#####################################################
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr_obd)

target_sources(app PRIVATE src/main.c)

# Custom files and folders
target_sources(app PRIVATE
src/ble/bluetoothle.c
    src/framework/framework-app.c
    src/ble/bluetoothle.c
    src/app/app_ble.c
)
zephyr_library_include_directories(src/framework)
zephyr_library_include_directories(src/app)
zephyr_library_include_directories(src/ble)



