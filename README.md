# M0 — MSPM0G3507 空白工程 (Keil MDK + MSPM0 SDK)

基于 TI MSPM0 SDK `2.02.00.05` 的 DriverLib 空白模板，目标芯片 **MSPM0G3507** (Cortex-M0+, LQFP-64 / LP-MSPM0G3507)。

## 目录结构
```
M0/
├── main.c                  用户主程序（在 while(1) 里写你的代码）
├── M0.syscfg               SysConfig 配置文件（最小化：仅 SYSCTL + Board）
├── ti_msp_dl_config.c/.h   外设初始化代码（由 SysConfig 根据 .syscfg 自动生成，勿手改）
├── README.md
├── .vscode/
│   └── tasks.json          VS Code 任务：⚙️ 打开 TI SysConfig（图形界面）
└── keil/
    ├── M0.uvprojx          Keil 工程文件（双击打开）
    ├── M0.uvoptx
    ├── mspm0g3507.sct      链接脚本（分散加载）
    └── startup_mspm0g350x_uvision.s   启动文件
```

## 依赖（本机实际路径，均在 D 盘）
- MSPM0 SDK: `D:\ti\M0_SDK\mspm0_sdk_2_02_00_05`
- SysConfig（带 GUI）: `D:\ti\sysconfig_1.21.1`
- driverlib.a: `...\source\ti\driverlib\lib\keil\m0p\mspm0g1x0x_g3x0x\driverlib.a`

> 说明：本机另有 C 盘的 SDK 2.10.00.04 + SysConfig 1.26.2，但 1.26.2 是**纯命令行版没有 GUI**，
> 所以工程统一用 D 盘成套的 SDK 2.02 + SysConfig 1.21.1（版本匹配，GUI 可直接打开）。

工程用**绝对路径**引用 SDK，可放在 SDK 目录之外（当前在桌面）。
若以后路径/版本变化，需同步修改：
- `keil/M0.uvprojx`：IncludePath、`driverlib.a` 路径、BeforeMake 里调用 SysConfig 的命令行
- `.vscode/tasks.json`：`command` 与 `-s` 后的 product.json 路径

## 使用

### Keil
1. 用 Keil µVision 打开 `keil/M0.uvprojx`。
2. 首次需安装 TI 器件包 `MSPM0G3507`（Pack Installer，Vendor: Texas Instruments）。
3. 编译（F7）。编译前会自动运行 SysConfig，依据 `M0.syscfg` 重新生成 `ti_msp_dl_config.c/.h`。

### VS Code
- 打开 `M0` 文件夹 → `Ctrl+Shift+P` → `Tasks: Run Task` → **⚙️ 打开 TI SysConfig**，
  即可用图形界面编辑 `M0.syscfg`；保存后会重新生成 `ti_msp_dl_config.c/.h` 到工程根目录。

> 提示：`ti_msp_dl_config.c/.h` 是自动生成的，改外设请改 `.syscfg`（或用 GUI），不要直接编辑这两个文件。
