@echo off
title Builder - Shark Remake

set "vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%vswhere%" (
    for /f "usebackq tokens=*" %%i in (`"%vswhere%" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
        set "InstallDir=%%i"
    )
)

if exist "%InstallDir%\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBuild=%InstallDir%\MSBuild\Current\Bin\MSBuild.exe"
) else (
    echo MSBuild not found! Please ensure Visual Studio is installed.
    exit /b 1
)

echo Building Project...
"%MSBuild%" "South.sln" /p:Configuration=Release /p:Platform=x64
if %errorlevel% neq 0 (
    echo Build failed with error code %errorlevel%
    exit /b %errorlevel%
)
echo Build succeeded!
