cmake_minimum_required(VERSION 3.23)

project(mspm0g3519_firmware LANGUAGES C)

set(FIRMWARE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/empty.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ti_msp_dl_config.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm/imu_attitude.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm/kalman_filter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm/mahony_filter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm/QuaternionEKF.c
    ${CMAKE_CURRENT_SOURCE_DIR}/apply/Encoder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/apply/GraySensor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/apply/ICM42688.c
    ${CMAKE_CURRENT_SOURCE_DIR}/apply/MotorControl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/apply/Time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive/Beep.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive/Delay.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive/EncoderExti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive/Key.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive/LCD_ST7735.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive/LED.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive/motor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive/pid.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive/spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive/uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Task/task.c
)

# The TI/Keil projects supply startup code and final linker settings. This
# target compiles all application sources for CMake Tools and IntelliSense.
add_library(firmware_objects OBJECT ${FIRMWARE_SOURCES})

target_include_directories(firmware_objects PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm
    ${CMAKE_CURRENT_SOURCE_DIR}/apply
    ${CMAKE_CURRENT_SOURCE_DIR}/Drive
    ${CMAKE_CURRENT_SOURCE_DIR}/Task
    C:/ti/mspm0_sdk_2_10_00_04/source
    C:/ti/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include
    C:/ti/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/DSP/Include
)

target_compile_definitions(firmware_objects PRIVATE
    __MSPM0G3519__
    ARM_MATH_CM0PLUS
)

target_compile_options(firmware_objects PRIVATE
    -mcpu=cortex-m0plus
    -march=thumbv6m
    -mthumb
    -mfloat-abi=soft
    -gdwarf-3
    -Wall
)
