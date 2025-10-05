@echo off
setlocal

set BUILD_DIR=build
set BIN_DIR=bin
set EXECUTABLE=assembler.exe

REM Clean previous build
if exist %BUILD_DIR% rmdir /S /Q %BUILD_DIR%
if exist %BIN_DIR% rmdir /S /Q %BIN_DIR%
mkdir %BIN_DIR%

REM Configure CMake with MinGW generator
cmake -G "MinGW Makefiles" -B %BUILD_DIR% -DCMAKE_BUILD_TYPE=Debug

REM Build
cmake --build %BUILD_DIR%

REM No need to copy now if CMake sets output directory
if exist "%BIN_DIR%\%EXECUTABLE%" (
    echo %EXECUTABLE% built successfully: %BIN_DIR%\%EXECUTABLE%
) else (
    echo ERROR: %EXECUTABLE% not found in %BIN_DIR%
)

pause
