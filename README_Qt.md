# Clipboard Manager - Qt C++ Version

A native Mac-style clipboard history manager built with Qt6 and C++. Features real clipboard monitoring, system tray integration, and both compact popup and full application interfaces.

## Features

🎨 **Native Mac Design**
- System tray integration with native notifications
- Real-time clipboard monitoring and history
- Glass-morphism UI with backdrop blur effects
- Native Mac keyboard shortcuts and behaviors

📋 **Smart Clipboard Management**
- Automatic clipboard content detection and categorization
- Support for text, images, HTML, URLs, code snippets, and files
- Real-time search and filtering capabilities
- Configurable history size limits

⚡ **Dual Interface Design**
- **System Tray Popup**: Quick access from menu bar with recent items
- **Full Application**: Complete clipboard management with advanced features
- Seamless switching between compact and full interfaces

🔧 **Advanced Features**
- Context menus for item management
- Keyboard shortcuts and navigation
- Automatic duplicate detection and prevention
- Persistent clipboard history (can be extended with database storage)
- Cross-platform compatibility (macOS, Windows, Linux)

## Prerequisites

### System Requirements
- Qt6 (6.2 or later)
- CMake 3.16+
- C++17 compatible compiler
- Platform-specific requirements:
  - **macOS**: Xcode Command Line Tools
  - **Linux**: GCC/Clang, X11 development libraries
  - **Windows**: Visual Studio or MinGW

### Installing Qt6

#### macOS (using Homebrew)
```bash
brew install qt@6
```

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev cmake build-essential
```

#### Windows
Download and install Qt6 from the official Qt website or use package managers like vcpkg.

## Building the Application

### 1. Clone or Extract Source Code
Ensure all source files are in your project directory with the following structure:
```
clipboard-manager/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── MainWindow.{h,cpp}
│   ├── ClipboardManager.{h,cpp}
│   ├── SystemTrayManager.{h,cpp}
│   ├── ClipboardItem.{h,cpp}
│   ├── ClipboardHistoryWidget.{h,cpp}
│   └── TrayPopupWidget.{h,cpp}
├── resources/
│   └── resources.qrc
└── build.sh
```

### 2. Build Using the Script
```bash
chmod +x build.sh
./build.sh
```

### 3. Manual Build (Alternative)
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### 4. Run the Application
```bash
cd build
./ClipboardManager
```

## Usage

### System Tray Integration
- The application starts minimized to the system tray
- **Left-click** the tray icon to open the quick popup
- **Right-click** for context menu with application options
- **Middle-click** to open the full application window

### Quick Popup Interface
- Displays the 10 most recent clipboard items
- Click any item to copy it back to clipboard
- Search through clipboard history in real-time
- "Open App" button launches the full interface
- "Clear" button removes all clipboard history

### Full Application Interface
- Complete clipboard history with unlimited scrolling
- Advanced search and filtering by content type
- Item statistics and management tools
- Context menus for individual item operations
- Preferences and configuration options

### Keyboard Shortcuts
- **Global shortcuts** (when implemented):
  - System-specific clipboard shortcuts work normally
  - Additional shortcuts can be added for quick access

## File Structure

```
src/
├── main.cpp                    # Application entry point
├── MainWindow.{h,cpp}          # Main application window
├── ClipboardManager.{h,cpp}    # Core clipboard monitoring logic
├── SystemTrayManager.{h,cpp}   # System tray integration
├── ClipboardItem.{h,cpp}       # Individual clipboard item model
├── ClipboardHistoryWidget.{h,cpp} # Full history interface widget
└── TrayPopupWidget.{h,cpp}     # Quick popup interface widget
```

## Customization

### Modifying Appearance
- Edit the `applyMacStyle()` methods in each widget class
- Modify stylesheets for custom colors, fonts, and spacing
- Icons can be replaced in the resources directory

### Extending Functionality
- **Database Storage**: Replace in-memory history with SQLite
- **Global Shortcuts**: Integrate platform-specific hotkey libraries
- **Cloud Sync**: Add network synchronization capabilities
- **Plugins**: Extend with custom content processors

### Configuration Options
The application can be extended with:
- Maximum history size configuration
- Custom keyboard shortcuts
- Content filtering rules
- Auto-start preferences

## Platform-Specific Notes

### macOS
- Application integrates with the native menu bar
- Follows macOS Human Interface Guidelines
- Can be built as a proper .app bundle

### Linux
- Works with most desktop environments
- System tray behavior depends on DE support
- May require additional permissions for clipboard access

### Windows
- Integrates with Windows system tray
- Follows Windows design guidelines
- Can be built as a proper Windows installer

## Troubleshooting

### Common Build Issues

**Qt6 not found**:
```bash
# Ensure Qt6 is in your PATH
export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"  # macOS
export CMAKE_PREFIX_PATH="/usr/lib/x86_64-linux-gnu/cmake/Qt6"  # Linux
```

**System tray not available**:
- Ensure your desktop environment supports system tray
- Some Linux environments may need additional packages

**Permission issues**:
- The application needs permission to monitor clipboard
- On some systems, this may require explicit user approval

### Performance Notes
- The application monitors clipboard changes efficiently
- History is kept in memory for fast access
- Large images may impact memory usage

## Development

### Building for Development
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

### Testing
- Manual testing covers all major functionality
- Consider adding automated tests for core clipboard logic
- Test on multiple platforms for compatibility

### Contributing
When extending the application:
1. Follow Qt coding conventions
2. Maintain the Mac-inspired design aesthetic
3. Test on target platforms
4. Update documentation for new features

## License

This project demonstrates Qt application development techniques. Modify and use according to your needs.

---

**Enjoy your native Qt clipboard manager!** 📋✨