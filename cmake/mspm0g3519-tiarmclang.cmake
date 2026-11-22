set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

find_program(CMAKE_C_COMPILER
    NAMES tiarmclang arm-none-eabi-gcc
    HINTS "C:/ti/ti_cgt_arm_llvm_4.0.2.LTS/bin"
    REQUIRED
)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
