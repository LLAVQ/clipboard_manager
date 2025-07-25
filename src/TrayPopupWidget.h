#ifndef TRAYPOPUPWIDGET_H
#define TRAYPOPUPWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ClipboardManager.h"

class TrayPopupWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TrayPopupWidget(ClipboardManager* clipboardManager, QWidget* parent = nullptr);
    
    void refreshHistory();
    
signals:
    void openMainWindow();
    
protected:
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    
private slots:
    void onSearchTextChanged();
    void onItemClicked(QListWidgetItem* item);
    void onItemDoubleClicked(QListWidgetItem* item);
    void onClearHistoryClicked();
    void onOpenMainWindowClicked();
    
private:
    ClipboardManager* m_clipboardManager;
    
    // UI elements
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_headerLayout;
    QHBoxLayout* m_footerLayout;
    
    QLabel* m_titleLabel;
    QLineEdit* m_searchEdit;
    QListWidget* m_historyList;
    QPushButton* m_openAppButton;
    QPushButton* m_clearButton;
    
    void setupUI();
    void applyMacStyle();
    void updateHistoryList();
    void updateHistoryList(const QList<ClipboardItem>& items);
    QListWidgetItem* createHistoryItem(const ClipboardItem& clipboardItem);
};

#endif // TRAYPOPUPWIDGET_H