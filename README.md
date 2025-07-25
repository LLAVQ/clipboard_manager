# Clipboard Manager

A beautiful, native Mac-style clipboard history manager with system tray integration. Available in both React/TypeScript (web) and Qt/C++ (native desktop) versions.

## Qt C++ Version (Native Desktop)

### Required Files Only

To build and run the Qt C++ application, you only need these files:

#### Essential Files:
```
clipboard-manager/
├── src/                           # Core application source code
│   ├── main.cpp                   # Application entry point
│   ├── ClipboardManager.{h,cpp}   # Clipboard monitoring logic
│   ├── ClipboardItem.{h,cpp}      # Clipboard item data model
│   ├── SystemTrayManager.{h,cpp}  # System tray integration
│   ├── MainWindow.{h,cpp}         # Main application window
│   ├── ClipboardHistoryWidget.{h,cpp} # History management UI
│   └── TrayPopupWidget.{h,cpp}    # Quick popup interface
├── resources/
│   └── resources.qrc              # Qt resource file
├── CMakeLists.txt                 # CMake build configuration
├── ClipboardManager.pro           # qmake build configuration (alternative)
├── build.sh                       # CMake build script
└── build_qmake.sh                 # qmake build script (alternative)
```

#### Files You DON'T Need for Qt Version:
- `App.tsx`, `main.tsx` (React files)
- `components/` folder (except if you want to keep it for reference)
- `package.json`, `vite.config.ts` (Node.js/Vite files)
- `styles/`, `public/` (Web assets)
- `index.html` (Web entry point)

### Quick Start (Qt C++ Version)

#### 1. Prerequisites
```bash
# macOS
brew install qt@6 cmake

# Ubuntu/Debian
sudo apt install qt6-base-dev qt6-tools-dev cmake build-essential

# Arch Linux
sudo pacman -S qt6-base qt6-tools cmake
```

#### 2. Build & Run
```bash
# Option A: Using CMake (recommended)
chmod +x build.sh
./build.sh
cd build && ./ClipboardManager

# Option B: Using qmake
chmod +x build_qmake.sh
./build_qmake.sh
cd build-qmake && ./ClipboardManager
```

### Features

🎨 **Native Mac Design**
- System tray integration with native menu bar appearance
- Real-time clipboard monitoring and history tracking
- Glass-morphism UI with backdrop blur effects
- Native keyboard shortcuts and system integration

📋 **Smart Clipboard Management**
- Automatic content detection (text, images, URLs, code, files)
- Real-time search and filtering
- Duplicate detection and prevention
- Configurable history size limits

⚡ **Dual Interface**
- **Quick Popup**: Click tray icon for instant access to recent items
- **Full Application**: Complete management with advanced features
- Seamless switching between compact and full interfaces

### Usage

#### System Tray
- **Left-click** tray icon → Quick popup with recent items
- **Right-click** tray icon → Context menu (Show App, About, Quit)
- **Middle-click** tray icon → Open full application window

#### Quick Popup
- Shows 10 most recent clipboard items
- Click any item to copy it back to clipboard
- Search through history in real-time
- "Open App" → Launch full interface
- "Clear" → Remove all history

#### Full Application
- Complete clipboard history with unlimited scrolling
- Advanced search and filtering by content type
- Item statistics and management tools
- Context menus for individual operations
- Preferences and settings

---

## React/TypeScript Version (Web)

The React version provides a web-based clipboard manager with similar functionality.

### Required Files for React Version:
```
clipboard-manager/
├── src/ (Qt files - not needed for React)
├── components/                    # React UI components
├── styles/                        # CSS styling
├── public/                        # Static assets
├── App.tsx                        # Main React component
├── main.tsx                       # React entry point
├── index.html                     # HTML template
├── package.json                   # Dependencies
├── vite.config.ts                 # Build configuration
└── tsconfig.json                  # TypeScript configuration
```

### Quick Start (React Version)
```bash
npm install
npm run dev
```

### Features
- Web-based clipboard manager interface
- Search and filtering capabilities
- Modern React with TypeScript
- Tailwind CSS styling
- Component-based architecture

---

## Choosing Your Version

**Choose Qt C++ if you want:**
- Native desktop application
- Real clipboard monitoring
- System tray integration
- Better performance
- No browser dependency

**Choose React/TypeScript if you want:**
- Web-based interface
- Easy customization
- Cross-platform web deployment
- Modern web technologies

---

## Development

### Qt C++ Development
```bash
# Debug build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

### React Development
```bash
npm run dev      # Development server
npm run build    # Production build
npm run preview  # Preview production build
```

---

## Platform Support

### Qt C++ Version
- ✅ macOS (10.15+)
- ✅ Linux (Ubuntu 20.04+, most distributions)
- ✅ Windows (10+)

### React Version
- ✅ All modern browsers
- ✅ Cross-platform web deployment

---

## Troubleshooting

### Qt Build Issues
**"Qt6 not found":**
```bash
# macOS
export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"

# Linux
export CMAKE_PREFIX_PATH="/usr/lib/x86_64-linux-gnu/cmake/Qt6"
```

**"System tray not available":**
- Ensure your desktop environment supports system tray
- On some Linux DEs, install additional tray support packages

### React Issues
**"Dependencies not found":**
```bash
rm -rf node_modules package-lock.json
npm install
```

---

## License

This project is provided as-is for educational and practical use. Modify according to your needs.

---

**Enjoy your clipboard manager!** 📋✨