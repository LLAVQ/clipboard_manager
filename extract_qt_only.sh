#!/bin/bash

# Script to extract only Qt C++ files for a clean build

echo "Creating clean Qt-only clipboard manager..."

# Create new directory
QT_DIR="qt-clipboard-manager"
mkdir -p "$QT_DIR"

echo "Copying essential Qt files..."

# Copy source files
mkdir -p "$QT_DIR/src"
cp src/*.h src/*.cpp "$QT_DIR/src/"

# Copy build configuration files
cp CMakeLists.txt "$QT_DIR/"
cp ClipboardManager.pro "$QT_DIR/"

# Copy build scripts
cp build.sh "$QT_DIR/"
cp build_qmake.sh "$QT_DIR/"

# Copy resources (create minimal version)
mkdir -p "$QT_DIR/resources"
cp resources/resources.qrc "$QT_DIR/resources/"

# Create minimal README
cat > "$QT_DIR/README.md" << 'EOF'
# Qt Clipboard Manager

A native Mac-style clipboard history manager built with Qt6 and C++.

## Quick Start

```bash
# Install Qt6
brew install qt@6 cmake  # macOS
# OR
sudo apt install qt6-base-dev cmake  # Ubuntu

# Build and run
chmod +x build.sh
./build.sh
./build/ClipboardManager
```

## Features
- Real-time clipboard monitoring
- System tray integration  
- Quick popup + full app interface
- Smart content detection
- Mac-style native design

## Usage
- Left-click tray icon → Quick popup
- Right-click tray icon → Context menu
- Double-click items to copy back to clipboard
EOF

echo "✅ Clean Qt project created in: $QT_DIR/"
echo ""
echo "To build and run:"
echo "  cd $QT_DIR"
echo "  chmod +x build.sh"
echo "  ./build.sh"
echo "  ./build/ClipboardManager"
echo ""
echo "Total files: $(find "$QT_DIR" -type f | wc -l)"
echo "Project size: $(du -sh "$QT_DIR" | cut -f1)"