#ifndef SYSTEMTRAYMANAGER_H
#define SYSTEMTRAYMANAGER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include "ClipboardManager.h"
#include "TrayPopupWidget.h"

class MainWindow;

class SystemTrayManager : public QObject
{
    Q_OBJECT
    
public:
    explicit SystemTrayManager(MainWindow* mainWindow, QObject* parent = nullptr);
    
    void show();
    void hide();
    
private slots:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMainWindow();
    void showAbout();
    void quitApplication();
    void onHistoryChanged();
    void toggleTrayPopup();
    
private:
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_trayMenu;
    QAction* m_showAction;
    QAction* m_aboutAction;
    QAction* m_quitAction;
    
    MainWindow* m_mainWindow;
    ClipboardManager* m_clipboardManager;
    TrayPopupWidget* m_trayPopup;
    
    void createTrayIcon();
    void createContextMenu();
    void updateTrayIcon();
    void positionTrayPopup();
};

#endif // SYSTEMTRAYMANAGER_H