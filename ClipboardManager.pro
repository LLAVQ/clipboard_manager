QT += core widgets gui

CONFIG += c++17

TARGET = ClipboardManager
TEMPLATE = app

# Application metadata
VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "ClipboardManager"
QMAKE_TARGET_PRODUCT = "Clipboard Manager"
QMAKE_TARGET_DESCRIPTION = "Mac-style clipboard history manager"
QMAKE_TARGET_COPYRIGHT = "Copyright 2024"

# Source files
SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/ClipboardManager.cpp \
    src/SystemTrayManager.cpp \
    src/ClipboardItem.cpp \
    src/ClipboardHistoryWidget.cpp \
    src/TrayPopupWidget.cpp

# Header files
HEADERS += \
    src/MainWindow.h \
    src/ClipboardManager.h \
    src/SystemTrayManager.h \
    src/ClipboardItem.h \
    src/ClipboardHistoryWidget.h \
    src/TrayPopupWidget.h

# Resources
RESOURCES += resources/resources.qrc

# Include directory
INCLUDEPATH += src

# Platform-specific settings
macx {
    ICON = resources/icons/app.icns
    QMAKE_INFO_PLIST = Info.plist
    
    # Enable high DPI support
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}

win32 {
    RC_ICONS = resources/icons/app.ico
    
    # Windows specific flags
    QMAKE_LFLAGS += /ENTRY:mainCRTStartup
}

unix:!macx {
    # Linux specific settings
    target.path = /usr/local/bin
    INSTALLS += target
}

# Compiler flags
QMAKE_CXXFLAGS += -Wall -Wextra