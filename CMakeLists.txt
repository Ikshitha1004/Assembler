cmake_minimum_required(VERSION 3.15)
project(AssemblerProject LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Output executable to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Add executable
add_executable(assembler ${SOURCES})

# Compiler warnings
if(MSVC)
    target_compile_options(assembler PRIVATE /W4 /EHsc)
else()
    target_compile_options(assembler PRIVATE -Wall -Wextra -g)
endif()
