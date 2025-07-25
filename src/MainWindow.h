#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "ClipboardManager.h"
#include "ClipboardHistoryWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    
    void setClipboardManager(ClipboardManager* manager);
    
protected:
    void closeEvent(QCloseEvent* event) override;
    void changeEvent(QEvent* event) override;
    
private slots:
    void hideToTray();
    void showPreferences();
    
private:
    ClipboardManager* m_clipboardManager;
    ClipboardHistoryWidget* m_historyWidget;
    
    // UI elements
    QWidget* m_centralWidget;
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_headerLayout;
    QLabel* m_titleLabel;
    QPushButton* m_hideButton;
    QPushButton* m_preferencesButton;
    
    void setupUI();
    void applyMacStyle();
};

#endif // MAINWINDOW_H