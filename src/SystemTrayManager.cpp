#include "SystemTrayManager.h"
#include "MainWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QScreen>
#include <QRect>
#include <QStyle>

SystemTrayManager::SystemTrayManager(MainWindow* mainWindow, QObject* parent)
    : QObject(parent)
    , m_mainWindow(mainWindow)
    , m_trayIcon(nullptr)
    , m_trayMenu(nullptr)
    , m_clipboardManager(new ClipboardManager(this))
    , m_trayPopup(nullptr)
{
    createTrayIcon();
    createContextMenu();
    
    // Create tray popup
    m_trayPopup = new TrayPopupWidget(m_clipboardManager);
    
    // Connect clipboard manager signals
    connect(m_clipboardManager, &ClipboardManager::historyChanged,
            this, &SystemTrayManager::onHistoryChanged);
    
    // Connect popup signals
    connect(m_trayPopup, &TrayPopupWidget::openMainWindow,
            this, &SystemTrayManager::showMainWindow);
    
    // Pass clipboard manager to main window
    m_mainWindow->setClipboardManager(m_clipboardManager);
    
    updateTrayIcon();
}

void SystemTrayManager::show()
{
    if (m_trayIcon) {
        m_trayIcon->show();
    }
}

void SystemTrayManager::hide()
{
    if (m_trayIcon) {
        m_trayIcon->hide();
    }
}

void SystemTrayManager::createTrayIcon()
{
    m_trayIcon = new QSystemTrayIcon(this);
    
    // Set initial icon
    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_ComputerIcon);
    m_trayIcon->setIcon(icon);
    m_trayIcon->setToolTip("Clipboard Manager");
    
    connect(m_trayIcon, &QSystemTrayIcon::activated,
            this, &SystemTrayManager::onTrayIconActivated);
}

void SystemTrayManager::createContextMenu()
{
    m_trayMenu = new QMenu();
    
    m_showAction = m_trayMenu->addAction("Show Clipboard Manager");
    connect(m_showAction, &QAction::triggered, this, &SystemTrayManager::showMainWindow);
    
    m_trayMenu->addSeparator();
    
    m_aboutAction = m_trayMenu->addAction("About");
    connect(m_aboutAction, &QAction::triggered, this, &SystemTrayManager::showAbout);
    
    m_quitAction = m_trayMenu->addAction("Quit");
    connect(m_quitAction, &QAction::triggered, this, &SystemTrayManager::quitApplication);
    
    m_trayIcon->setContextMenu(m_trayMenu);
}

void SystemTrayManager::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            toggleTrayPopup();
            break;
        case QSystemTrayIcon::MiddleClick:
            showMainWindow();
            break;
        default:
            break;
    }
}

void SystemTrayManager::toggleTrayPopup()
{
    if (m_trayPopup->isVisible()) {
        m_trayPopup->hide();
    } else {
        positionTrayPopup();
        m_trayPopup->show();
        m_trayPopup->raise();
        m_trayPopup->activateWindow();
    }
}

void SystemTrayManager::positionTrayPopup()
{
    if (!m_trayIcon || !m_trayPopup) {
        return;
    }
    
    QRect trayGeometry = m_trayIcon->geometry();
    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    
    int popupWidth = 320;
    int popupHeight = 400;
    
    // Position popup below system tray icon (macOS style)
    int x = trayGeometry.center().x() - popupWidth / 2;
    int y = trayGeometry.bottom() + 5;
    
    // Ensure popup stays within screen bounds
    if (x + popupWidth > screenGeometry.right()) {
        x = screenGeometry.right() - popupWidth - 10;
    }
    if (x < screenGeometry.left()) {
        x = screenGeometry.left() + 10;
    }
    
    // On macOS, menu bar is at top, so position below
#ifdef Q_OS_MACOS
    y = screenGeometry.top() + 25; // Account for menu bar height
#endif
    
    m_trayPopup->setGeometry(x, y, popupWidth, popupHeight);
}

void SystemTrayManager::showMainWindow()
{
    m_mainWindow->show();
    m_mainWindow->raise();
    m_mainWindow->activateWindow();
    
    // Hide tray popup if visible
    if (m_trayPopup && m_trayPopup->isVisible()) {
        m_trayPopup->hide();
    }
}

void SystemTrayManager::showAbout()
{
    QMessageBox::about(nullptr, "About Clipboard Manager",
                      "Clipboard Manager v1.0.0\n\n"
                      "A beautiful Mac-style clipboard history manager.\n\n"
                      "Features:\n"
                      "• System tray integration\n"
                      "• Clipboard history tracking\n"
                      "• Search and filtering\n"
                      "• Multiple content types support\n\n"
                      "Built with Qt and C++");
}

void SystemTrayManager::quitApplication()
{
    QApplication::quit();
}

void SystemTrayManager::onHistoryChanged()
{
    updateTrayIcon();
    
    // Update tray popup if visible
    if (m_trayPopup && m_trayPopup->isVisible()) {
        m_trayPopup->refreshHistory();
    }
}

void SystemTrayManager::updateTrayIcon()
{
    if (!m_trayIcon) {
        return;
    }
    
    int itemCount = m_clipboardManager->itemCount();
    QString tooltip = QString("Clipboard Manager (%1 items)").arg(itemCount);
    m_trayIcon->setToolTip(tooltip);
    
    // Could create a custom icon with badge showing item count
    // For now, keep the standard icon
}