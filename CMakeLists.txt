cmake_minimum_required( VERSION 3.14.0 )
project(
    avr-rectangle
    LANGUAGES C CXX
)

# Set AVR compilers
set(CMAKE_C_COMPILER /usr/bin/avr-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/avr-g++)

# Microchip AVR configuration
set(MICROCHIP_AVR_MCU   "atmega32u4")
set(MICROCHIP_AVR_F_CPU "16000000UL")

# configure compilation and linking
set(DEFINES
    "-mmcu=${MICROCHIP_AVR_MCU} \
    -DF_CPU=${MICROCHIP_AVR_F_CPU} \
    -DF_USB=8000000 \
    -DUSE_STATIC_OPTIONS \
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
set(CMAKE_CXX_STANDARD 20)
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

set(SOURCES
    src/main.cpp
    src/wup/wup.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
add_custom_target(hex ALL
                  COMMAND avr-objcopy -O ihex -R .eeprom ${PROJECT_NAME} ${PROJECT_NAME}.hex
                  DEPENDS ${PROJECT_NAME})
