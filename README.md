# M0 MSPM0G3507 智能车工程

这是一个基于 TI MSPM0G3507 的智能车工程，使用 Keil MDK ARMCLANG 编译，外设初始化代码由 SysConfig 生成。

## 当前配置

- 主控芯片: MSPM0G3507
- 编译器: Keil MDK ARMCLANG V6.21
- MSPM0 SDK: `C:\ti\mspm0_sdk_2_10_00_04`
- SysConfig: `C:\ti\sysconfig_1.28.0`
- CMSIS-DSP: 使用 SDK 自带 `arm_math.h` 和 `arm_cortexM0l_math.a`

## 目录结构

```text
M0/
|-- main.c
|-- M0.syscfg
|-- ti_msp_dl_config.c
|-- ti_msp_dl_config.h
|-- Drive/
|   |-- LED / Beep / Delay / UART / SPI
|   |-- motor / PID / EncoderExti
|   `-- headfile.h / databyte.h
|-- apply/
|   |-- ICM42688
|   |-- MotorControl
|   |-- Encoder
|   `-- GraySensor
|-- Algorithm/
|   |-- mahony_filter
|   |-- imu_attitude
|   |-- kalman_filter
|   `-- QuaternionEKF
`-- keil/
    |-- M0.uvprojx
    |-- mspm0g3507.sct
    `-- startup_mspm0g350x_uvision.s
```

## 编译方法

1. 用 Keil 打开 `keil\M0.uvprojx`。
2. 选择目标 `M0`。
3. 按 `F7` 编译。

也可以在命令行编译:

```powershell
& "D:\Keil5\UV4\UV4.exe" -b "C:\Users\PC\Desktop\M0\keil\M0.uvprojx" -t "M0"
```

## SysConfig 说明

如果需要重新配置 GPIO、SPI、UART、时钟等外设，请修改 `M0.syscfg`，然后重新生成:

- `ti_msp_dl_config.c`
- `ti_msp_dl_config.h`

当前 Keil 工程已经切到 MSPM0 SDK `2.10.00.04`。这样重新生成代码后，`DL_SYSCTL_enableSYSPLL()` 这类新 SDK 接口可以正常找到，不会再因为 Keil 还指向旧 SDK `2.02.00.05` 而报未声明错误。

## 注意事项

- `ti_msp_dl_config.c` 和 `ti_msp_dl_config.h` 是 SysConfig 生成文件，通常不要手动改。
- `kalman_filter` 和 `QuaternionEKF` 依赖 CMSIS-DSP，所以不要删除 `ARM_MATH_CM0PLUS`、CMSIS-DSP 头文件路径和 DSP 数学库。
- Keil 编译输出在 `keil\Objects` 等目录下，已经通过 `.gitignore` 忽略，不需要上传到仓库。
