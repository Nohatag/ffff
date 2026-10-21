@echo off
setlocal enabledelayedexpansion
title South - Build Release x64

echo =======================================================
echo          South Build System - Release x64
echo =======================================================
echo.

:: 1. Locate vswhere.exe
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" set "VSWHERE=%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe"

set "MSBUILD="

:: Try finding MSBuild using vswhere
if exist "%VSWHERE%" (
    for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
        if exist "%%i" set "MSBUILD=%%i"
    )
)

:: Fallback check for common paths if vswhere did not resolve MSBuild
if not defined MSBUILD (
    for %%p in (
        "C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\18\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
    ) do (
        if exist "%%~p" if not defined MSBUILD set "MSBUILD=%%~p"
    )
)

if not defined MSBUILD (
    echo [ERROR] MSBuild.exe was not found.
    echo Please make sure Visual Studio / MSBuild is installed.
    echo.
    pause
    exit /b 1
)

echo [INFO] Found MSBuild: "!MSBUILD!"

:: 2. Locate Project File (South.vcxproj)
set "PROJECT_PATH="
if exist "%~dp0South\South.vcxproj" (
    set "PROJECT_PATH=%~dp0South\South.vcxproj"
) else if exist "%~dp0South.vcxproj" (
    set "PROJECT_PATH=%~dp0South.vcxproj"
) else if exist "%~dp0..\South\South.vcxproj" (
    set "PROJECT_PATH=%~dp0..\South\South.vcxproj"
)

if not defined PROJECT_PATH (
    echo [ERROR] South.vcxproj not found!
    echo.
    pause
    exit /b 1
)

echo [INFO] Target Project: "!PROJECT_PATH!"
echo [INFO] Starting build for Release x64...
echo -------------------------------------------------------

"!MSBUILD!" "!PROJECT_PATH!" /p:Configuration=Release /p:Platform=x64 /m /v:minimal

if !ERRORLEVEL! equ 0 (
    echo.
    echo -------------------------------------------------------
    echo [SUCCESS] Build completed successfully for Release x64!
    echo -------------------------------------------------------
) else (
    echo.
    echo -------------------------------------------------------
    echo [FAILED] Build failed with exit code !ERRORLEVEL!.
    echo -------------------------------------------------------
)

echo.
pause
exit /b !ERRORLEVEL!
