@echo off
set SYSCFG_PATH=C:\ti\sysconfig_1.28.0\sysconfig_cli.bat
set SDK_ROOT=C:\ti\mspm0_sdk_2_10_00_04

set PROJ_DIR=%~1
set PROJ_DIR=%PROJ_DIR:'=%

set SYSCFG_FILE=%~2
set SYSCFG_FILE=%SYSCFG_FILE:'=%

set SYSCFG_DIR=%PROJ_DIR%
set iter=0
:syscfg_search_loop
if exist "%SYSCFG_DIR%\*.syscfg" (
    if "%SYSCFG_DIR:~-1%"=="\" set SYSCFG_DIR=%SYSCFG_DIR:~0,-1%
    goto syscfg_search_exit
) else if %iter% geq 5 (
    echo "Couldn't find syscfg file"
    exit /b 1
) else (
    set /a iter=%iter%+1
    set SYSCFG_DIR=%SYSCFG_DIR%..\
    goto syscfg_search_loop
)
:syscfg_search_exit

"%SYSCFG_PATH%" -o "%SYSCFG_DIR%" -s "%SDK_ROOT%\.metadata\product.json" --compiler keil "%SYSCFG_DIR%\%SYSCFG_FILE%"
