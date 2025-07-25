#!/bin/bash

# Build script for Clipboard Manager

echo "Building Clipboard Manager..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
make -j$(nproc)

echo "Build complete!"
echo "Executable: ./ClipboardManager"