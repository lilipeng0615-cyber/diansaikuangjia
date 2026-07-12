@echo off
set "SYSCONFIG_DIR=C:\ti\sysconfig_1.28.0"
set "SYSCONFIG_GUI=%SYSCONFIG_DIR%\sysconfig_gui.bat"
set "SDK_ROOT=C:\ti\mspm0_sdk_2_10_00_04"
set "PROJECT_DIR=%~dp0"

if not exist "%SYSCONFIG_GUI%" (
    echo TI SysConfig 1.28 was not found: "%SYSCONFIG_GUI%"
    exit /b 1
)

rem Match the target metadata embedded in empty.syscfg. Do not save a default project.
call "%SYSCONFIG_GUI%" --product "%SDK_ROOT%\.metadata\product.json" --device "MSPM0G351X" --part "Default" --package "LQFP-80(PN)" --script "%PROJECT_DIR%empty.syscfg"
