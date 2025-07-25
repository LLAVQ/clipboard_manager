#include "MainWindow.h"
#include <QApplication>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_clipboardManager(nullptr)
    , m_historyWidget(nullptr)
{
    setWindowTitle("Clipboard Manager");
    setMinimumSize(800, 600);
    resize(1000, 700);
    
    setupUI();
    applyMacStyle();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setClipboardManager(ClipboardManager* manager)
{
    m_clipboardManager = manager;
    
    if (m_historyWidget) {
        m_historyWidget->setClipboardManager(manager);
    }
}

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget();
    setCentralWidget(m_centralWidget);
    
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);
    m_mainLayout->setSpacing(16);
    
    // Header
    m_headerLayout = new QHBoxLayout();
    
    m_titleLabel = new QLabel("Clipboard Manager");
    m_titleLabel->setObjectName("titleLabel");
    
    m_preferencesButton = new QPushButton("Preferences");
    m_preferencesButton->setObjectName("headerButton");
    connect(m_preferencesButton, &QPushButton::clicked, this, &MainWindow::showPreferences);
    
    m_hideButton = new QPushButton("Hide to Tray");
    m_hideButton->setObjectName("headerButton");
    connect(m_hideButton, &QPushButton::clicked, this, &MainWindow::hideToTray);
    
    m_headerLayout->addWidget(m_titleLabel);
    m_headerLayout->addStretch();
    m_headerLayout->addWidget(m_preferencesButton);
    m_headerLayout->addWidget(m_hideButton);
    
    // History widget
    m_historyWidget = new ClipboardHistoryWidget();
    
    // Add to main layout
    m_mainLayout->addLayout(m_headerLayout);
    m_mainLayout->addWidget(m_historyWidget, 1);
    
    // Status bar
    statusBar()->showMessage("Ready");
}

void MainWindow::applyMacStyle()
{
    setStyleSheet(R"(
        MainWindow {
            background-color: #f8f8f8;
        }
        
        QLabel#titleLabel {
            font-size: 24px;
            font-weight: 600;
            color: #333;
        }
        
        QPushButton#headerButton {
            background-color: #007AFF;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 13px;
            font-weight: 500;
            min-width: 80px;
        }
        
        QPushButton#headerButton:hover {
            background-color: #0056CC;
        }
        
        QPushButton#headerButton:pressed {
            background-color: #004499;
        }
        
        QStatusBar {
            background-color: #f0f0f0;
            border-top: 1px solid #d0d0d0;
            color: #666;
        }
    )");
    
#ifdef Q_OS_MACOS
    // Enable unified title and toolbar on macOS
    setUnifiedTitleAndToolBarOnMac(true);
#endif
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        hide();
        event->ignore();
        
        // Show notification first time
        static bool firstTime = true;
        if (firstTime) {
            QMessageBox::information(this, "Clipboard Manager",
                                   "The application will keep running in the system tray. "
                                   "To terminate the application, choose Quit from the context menu "
                                   "of the system tray entry.");
            firstTime = false;
        }
    } else {
        event->accept();
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    QMainWindow::changeEvent(event);
    
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized() && QSystemTrayIcon::isSystemTrayAvailable()) {
            hide();
        }
    }
}

void MainWindow::hideToTray()
{
    hide();
}

void MainWindow::showPreferences()
{
    QMessageBox::information(this, "Preferences",
                           "Preferences dialog would be implemented here.\n\n"
                           "Available settings:\n"
                           "• Maximum history size\n"
                           "• Keyboard shortcuts\n"
                           "• Startup behavior\n"
                           "• Content filtering");
}