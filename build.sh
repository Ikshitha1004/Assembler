#!/bin/bash
set -e

# Directories
BUILD_DIR=build
BIN_DIR=bin
EXECUTABLE=assembler

# Clean previous build
rm -rf $BUILD_DIR
rm -rf $BIN_DIR
mkdir -p $BIN_DIR

# Configure CMake
cmake -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build $BUILD_DIR

#cp $BUILD_DIR/$EXECUTABLE $BIN_DIR/$EXECUTABLE
chmod +x $BIN_DIR/$EXECUTABLE

echo "$EXECUTABLE built successfully: $BIN_DIR/$EXECUTABLE"
