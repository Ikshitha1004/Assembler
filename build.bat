@REM @echo off
@REM setlocal

@REM set BUILD_DIR=build
@REM set BIN_DIR=bin
@REM set EXECUTABLE=assembler.exe

@REM REM Clean previous build
@REM if exist %BUILD_DIR% rmdir /S /Q %BUILD_DIR%
@REM if exist %BIN_DIR% rmdir /S /Q %BIN_DIR%
@REM mkdir %BIN_DIR%

@REM REM Configure CMake with MinGW generator
@REM cmake -G "MinGW Makefiles" -B %BUILD_DIR% -DCMAKE_BUILD_TYPE=Debug

@REM REM Build
@REM cmake --build %BUILD_DIR%

@REM REM No need to copy now if CMake sets output directory
@REM if exist "%BIN_DIR%\%EXECUTABLE%" (
@REM     echo %EXECUTABLE% built successfully: %BIN_DIR%\%EXECUTABLE%
@REM ) else (
@REM     echo ERROR: %EXECUTABLE% not found in %BIN_DIR%
@REM )

@REM pause
@echo off
setlocal

set BUILD_DIR=build
set BIN_DIR=bin
set EXECUTABLE=assembler.exe

REM Only create build dir if it doesn't exist
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
if not exist %BIN_DIR% mkdir %BIN_DIR%

REM Configure CMake if needed
cmake -G "MinGW Makefiles" -B %BUILD_DIR% -DCMAKE_BUILD_TYPE=Debug

REM Build (incremental)
cmake --build %BUILD_DIR% 

REM Check executable
if exist "%BIN_DIR%\%EXECUTABLE%" (
    echo %EXECUTABLE% built successfully: %BIN_DIR%\%EXECUTABLE%
) else (
    echo ERROR: %EXECUTABLE% not found in %BIN_DIR%
)

pause
