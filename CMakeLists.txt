cmake_minimum_required( VERSION 3.14.0 )
project(
    avr-rectangle
    LANGUAGES C CXX
)

# Set AVR compilers
set(CMAKE_C_COMPILER    /usr/bin/avr-gcc)
set(CMAKE_CXX_COMPILER  /usr/bin/avr-g++)

# Microchip AVR configuration
set(MICROCHIP_AVR_MCU   "atmega32u4")
set(AVR_ARCH            "AVR8")
set(MICROCHIP_AVR_F_CPU "16000000UL")

set(LUFA_ROOT_PATH "lufa/LUFA")

# configure compilation and linking
set(DEFINES
    "-mmcu=${MICROCHIP_AVR_MCU} \
    -DF_CPU=${MICROCHIP_AVR_F_CPU} \
    -DF_USB=16000000 \
    -DUSE_STATIC_OPTIONS \
    -DUSE_FLASH_DESCRIPTORS\
    "
)

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} \
    -O3 \
    -Wall -Wextra \
    -Wcast-align=strict \
    -Wcast-qual \
    -Wduplicated-cond \
    -Wfloat-equal \
    -Wimplicit-fallthrough=5 \
    -Wlogical-op \
    -Wmissing-field-initializers \
    -Wmissing-include-dirs \
    -Wpointer-arith \
    -Wshadow \
    -Wunsafe-loop-optimizations \
    ${DEFINES} \
    "
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} \
    -O3 \
    -Wall -Wextra \
    -Wcast-align=strict \
    -Wcast-qual \
    -Wduplicated-cond \
    -Wextra-semi \
    -Wfloat-equal \
    -Wimplicit-fallthrough=5 \
    -Wlogical-op \
    -Wmissing-field-initializers \
    -Wmissing-include-dirs \
    -Wold-style-cast \
    -Wplacement-new=2 \
    -Wpointer-arith \
    -Wshadow \
    -Wunsafe-loop-optimizations \
    ${DEFINES} \
    "
)

include_directories(/usr/avr/include)
include_directories(${PROJECT_SOURCE_DIR}/lufa/)

set(LUFA_USB_SOURCES
    ${LUFA_ROOT_PATH}/Drivers/USB/Core/${AVR_ARCH}/Device_${AVR_ARCH}.c
    ${LUFA_ROOT_PATH}/Drivers/USB/Core/${AVR_ARCH}/Endpoint_${AVR_ARCH}.c
    ${LUFA_ROOT_PATH}/Drivers/USB/Core/${AVR_ARCH}/EndpointStream_${AVR_ARCH}.c
    ${LUFA_ROOT_PATH}/Drivers/USB/Core/DeviceStandardReq.c
    ${LUFA_ROOT_PATH}/Drivers/USB/Core/${AVR_ARCH}/USBController_${AVR_ARCH}.c
    ${LUFA_ROOT_PATH}/Drivers/USB/Core/${AVR_ARCH}/USBInterrupt_${AVR_ARCH}.c
    ${LUFA_ROOT_PATH}/Drivers/USB/Core/ConfigDescriptors.c
    ${LUFA_ROOT_PATH}/Drivers/USB/Core/Events.c
    ${LUFA_ROOT_PATH}/Drivers/USB/Core/USBTask.c
    ${LUFA_ROOT_PATH}/Drivers/USB/Class/Device/HIDClassDevice.c
)

set(SOURCES
    src/main.cpp
    src/wup/callbacks.cpp
    ${LUFA_USB_SOURCES}
)

add_executable(${PROJECT_NAME} ${SOURCES})
add_custom_target(hex ALL
                  COMMAND avr-objcopy -O ihex -R .eeprom ${PROJECT_NAME} ${PROJECT_NAME}.hex
                  DEPENDS ${PROJECT_NAME})
