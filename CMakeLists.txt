cmake_minimum_required(VERSION 3.21)
project(M0 C ASM)

set(SDK "C:/ti/mspm0_sdk_2_10_00_04")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB ALGO_SRC  ${CMAKE_CURRENT_SOURCE_DIR}/Algorithm/*.c)
file(GLOB DRIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/Drive/*.c)
file(GLOB APPLY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/apply/*.c)

add_executable(M0
    main.c
    ti_msp_dl_config.c
    ${ALGO_SRC}
    ${DRIVE_SRC}
    ${APPLY_SRC}
    ${SDK}/source/ti/devices/msp/m0p/startup_system_files/gcc/startup_mspm0g350x_gcc.c
)

# 与 Keil 工程一致的宏定义
target_compile_definitions(M0 PRIVATE
    __MSPM0G3507__
    ARM_MATH_CM0PLUS
)

target_compile_options(M0 PRIVATE
    -mcpu=cortex-m0plus -mthumb
    -O2 -g3
    -ffunction-sections -fdata-sections
    -Wall -Wno-unused-variable
    -fsingle-precision-constant
    -fno-strict-aliasing
)

# 与 Keil 工程一致的头文件路径
target_include_directories(M0 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    Algorithm
    Drive
    Task
    apply
    ${SDK}/source
    ${SDK}/source/third_party/CMSIS/Core/Include
    ${SDK}/source/third_party/CMSIS/DSP/Include
    ${SDK}/source/third_party/CMSIS/DSP/PrivateInclude
)

target_link_options(M0 PRIVATE
    -mcpu=cortex-m0plus -mthumb
    -T${SDK}/source/ti/devices/msp/m0p/linker_files/gcc/mspm0g3507.lds
    -Wl,--gc-sections -Wl,-Map=M0.map -Wl,--print-memory-usage
    --specs=nano.specs --specs=nosys.specs
)

# 与 Keil 链接的库一一对应，换成 SDK 自带的 GCC 版本
target_link_libraries(M0 PRIVATE
    ${SDK}/source/ti/driverlib/lib/gcc/m0p/mspm0g1x0x_g3x0x/driverlib.a
    ${SDK}/source/third_party/CMSIS/DSP/lib/gcc/m0p/arm_cortexM0l_math.a
    m
)

set_target_properties(M0 PROPERTIES SUFFIX .elf)

add_custom_command(TARGET M0 POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:M0> M0.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:M0> M0.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:M0>
)
