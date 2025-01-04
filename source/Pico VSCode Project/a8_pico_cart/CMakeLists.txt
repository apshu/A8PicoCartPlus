# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(a8_pico_cart)

# 16megs of flash on purple pico clones
set(PICO_BOARD weact_studio_rp2040_16mb CACHE STRING "Board type")

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(a8_pico_cart)

# Reconfigure default I2C settings according to A8PicoCartPlus board 
target_compile_definitions(a8_pico_cart PRIVATE
    PICO_DEFAULT_I2C=0
    PICO_DEFAULT_I2C_SDA_PIN=28 
    PICO_DEFAULT_I2C_SCL_PIN=29
)

#pico_set_linker_script(a8_pico_cart ${CMAKE_CURRENT_SOURCE_DIR}/memmap_custom.ld)

target_sources(a8_pico_cart PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/atari_cart.c
    ${CMAKE_CURRENT_LIST_DIR}/msc_disk.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
    ${CMAKE_CURRENT_LIST_DIR}/fatfs_disk.c
    ${CMAKE_CURRENT_LIST_DIR}/flash_fs.c
    ${CMAKE_CURRENT_LIST_DIR}/fatfs/ff.c
    ${CMAKE_CURRENT_LIST_DIR}/fatfs/ffunicode.c
    ${CMAKE_CURRENT_LIST_DIR}/fatfs/diskio.c
    ${CMAKE_CURRENT_LIST_DIR}/ssd1306_i2c.c
    ${CMAKE_CURRENT_LIST_DIR}/eeram_i2c.c
) 

target_include_directories(a8_pico_cart PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/fatfs
)

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support
target_link_libraries(a8_pico_cart PUBLIC pico_stdlib hardware_flash tinyusb_device hardware_i2c)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(a8_pico_cart)