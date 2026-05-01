@echo off
setlocal

cd /d "%~dp0"

where clang-format >nul 2>nul
if errorlevel 1 (
    echo Error: clang-format was not found in PATH.
    exit /b 1
)

where cmake >nul 2>nul
if errorlevel 1 (
    echo Error: cmake was not found in PATH.
    exit /b 1
)

where ninja >nul 2>nul
if errorlevel 1 (
    echo Error: ninja was not found in PATH.
    exit /b 1
)

where gcc >nul 2>nul
if errorlevel 1 (
    echo Error: gcc was not found in PATH.
    exit /b 1
)

echo Formatting source files with .clang-format...
powershell -NoProfile -ExecutionPolicy Bypass -Command "$ErrorActionPreference = 'Stop'; Get-ChildItem -Path . -Recurse -File -Include *.c,*.h | Where-Object { $_.FullName -notmatch '\\build\\' } | ForEach-Object { clang-format -i --style=file $_.FullName }"
if errorlevel 1 exit /b 1

if exist "build\CMakeCache.txt" (
    echo Refreshing CMake cache...
    del /q "build\CMakeCache.txt"
    if exist "build\CMakeFiles" rmdir /s /q "build\CMakeFiles"
)

echo Configuring with CMake and Ninja...
cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=gcc
if errorlevel 1 exit /b 1

echo Building hazel...
cmake --build build
if errorlevel 1 exit /b 1

echo Running hazel...
"%~dp0build\hazel.exe"
