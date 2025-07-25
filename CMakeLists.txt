cmake_minimum_required(VERSION 3.16)

project(ClipboardManager VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Enable Qt's MOC, UIC and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Application icon and resources
set(RESOURCE_FILES
    resources/resources.qrc
)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/ClipboardManager.cpp
    src/SystemTrayManager.cpp
    src/ClipboardItem.cpp
    src/ClipboardHistoryWidget.cpp
    src/TrayPopupWidget.cpp
)

# Header files
set(HEADERS
    src/MainWindow.h
    src/ClipboardManager.h
    src/SystemTrayManager.h
    src/ClipboardItem.h
    src/ClipboardHistoryWidget.h
    src/TrayPopupWidget.h
)

# UI files
set(UI_FILES
    ui/MainWindow.ui
    ui/ClipboardHistoryWidget.ui
)

# Create executable
add_executable(ClipboardManager
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

# Link Qt libraries
target_link_libraries(ClipboardManager
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

# Set target properties
set_target_properties(ClipboardManager PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# macOS specific settings
if(APPLE)
    set_target_properties(ClipboardManager PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        MACOSX_BUNDLE_BUNDLE_NAME "Clipboard Manager"
        MACOSX_BUNDLE_DISPLAY_NAME "Clipboard Manager"
        MACOSX_BUNDLE_IDENTIFIER "com.clipboardmanager.app"
        MACOSX_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    )
endif()

# Windows specific settings
if(WIN32)
    set_target_properties(ClipboardManager PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Include directories
target_include_directories(ClipboardManager PRIVATE src)