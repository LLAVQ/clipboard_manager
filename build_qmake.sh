#!/bin/bash

# Alternative build script using qmake instead of CMake

echo "Building Clipboard Manager with qmake..."

# Create build directory
mkdir -p build-qmake
cd build-qmake

# Generate Makefile with qmake
qmake ../ClipboardManager.pro CONFIG+=release

# Build the project
make -j$(nproc)

echo "Build complete!"
echo "Executable: ./ClipboardManager"

# On macOS, the app bundle will be created
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "macOS App Bundle: ./ClipboardManager.app"
fi