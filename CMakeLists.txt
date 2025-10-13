cmake_minimum_required(VERSION 3.15)
project(AssemblerProject LANGUAGES CXX)

# Output executables go to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# ============ ASSEMBLER TARGET ============

# Collect all source files for assembler (inside src/)
file(GLOB ASSEMBLER_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(assembler ${ASSEMBLER_SOURCES})

# Compiler warnings
if(MSVC)
    target_compile_options(assembler PRIVATE /W4 /EHsc)
else()
    target_compile_options(assembler PRIVATE -Wall -Wextra -g)
endif()

# ============ DISASSEMBLER TARGET ============

# Disassembler main file is outside src/
set(DISASSEMBLER_SOURCES
    ${PROJECT_SOURCE_DIR}/Disassembler.cpp
    ${PROJECT_SOURCE_DIR}/src/Instruction.cpp
    # add more shared .cpp files here if disassembler uses them
)

add_executable(disassembler ${DISASSEMBLER_SOURCES})

if(MSVC)
    target_compile_options(disassembler PRIVATE /W4 /EHsc)
else()
    target_compile_options(disassembler PRIVATE -Wall -Wextra -g)
endif()
