# Set the minimum CMake version
cmake_minimum_required(VERSION 3.20)

# Include the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Configure project
project(gcn-usb-adapter C CXX ASM)

# CMake target
set(TARGET gcn-usb-adapter)

# Initialize the Pico SDK
pico_sdk_init()

# Repo root (two levels up from ports/rp2xxx/)
set(GCCA_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)

# Joybus data-line GPIO for each channel (0-3); override with -DGCCA_JOYBUS_GPIO_CH0=.. etc.
set(GCCA_JOYBUS_GPIO_CH0 1 CACHE STRING "Joybus data GPIO for channel 0")
set(GCCA_JOYBUS_GPIO_CH1 2 CACHE STRING "Joybus data GPIO for channel 1")
set(GCCA_JOYBUS_GPIO_CH2 3 CACHE STRING "Joybus data GPIO for channel 2")
set(GCCA_JOYBUS_GPIO_CH3 4 CACHE STRING "Joybus data GPIO for channel 3")

# Make libjoybus available with the rp2xxx backend
set(JOYBUS_BACKEND rp2xxx)
add_subdirectory(${GCCA_ROOT}/lib/libjoybus ${CMAKE_BINARY_DIR}/_deps/libjoybus-build)

# Create the executable: app core + rp2xxx entry point
add_executable(${TARGET}
  ${GCCA_ROOT}/src/gcca.c
  ${GCCA_ROOT}/src/usb_descriptors.c
  main.c
)

# Include paths: app core headers and the port dir (tusb_config.h)
target_include_directories(${TARGET} PRIVATE ${GCCA_ROOT}/src .)

# Depend on Pico SDK, TinyUSB, and libjoybus
target_link_libraries(${TARGET} pico_stdlib tinyusb_device tinyusb_board joybus)

# Pass the configurable Joybus pins through to the firmware
target_compile_definitions(${TARGET} PRIVATE
  GCCA_JOYBUS_GPIO_CH0=${GCCA_JOYBUS_GPIO_CH0}
  GCCA_JOYBUS_GPIO_CH1=${GCCA_JOYBUS_GPIO_CH1}
  GCCA_JOYBUS_GPIO_CH2=${GCCA_JOYBUS_GPIO_CH2}
  GCCA_JOYBUS_GPIO_CH3=${GCCA_JOYBUS_GPIO_CH3})

# Generate UF2 file
pico_add_extra_outputs(${TARGET})
