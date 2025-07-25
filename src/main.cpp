#include <QApplication>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QFont>
#include "MainWindow.h"
#include "SystemTrayManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Clipboard Manager");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("ClipboardManager");
    app.setOrganizationDomain("clipboardmanager.com");
    
    // Check if system tray is available
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("System Tray"),
                             QObject::tr("System tray is not available on this system."));
        return 1;
    }
    
    // Don't quit when last window is closed (for system tray)
    app.setQuitOnLastWindowClosed(false);
    
    // Set application font for better Mac integration
#ifdef Q_OS_MACOS
    QFont systemFont = QFont(".AppleSystemUIFont", 13);
    app.setFont(systemFont);
#endif
    
    // Create main window
    MainWindow mainWindow;
    
    // Create system tray manager
    SystemTrayManager trayManager(&mainWindow);
    
    // Show tray icon
    trayManager.show();
    
    return app.exec();
}