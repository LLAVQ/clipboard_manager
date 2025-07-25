#ifndef CLIPBOARDHISTORYWIDGET_H
#define CLIPBOARDHISTORYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include "ClipboardManager.h"

class ClipboardHistoryWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ClipboardHistoryWidget(QWidget* parent = nullptr);
    
    void setClipboardManager(ClipboardManager* manager);
    
private slots:
    void onSearchTextChanged();
    void onFilterChanged();
    void onItemClicked(QListWidgetItem* item);
    void onItemDoubleClicked(QListWidgetItem* item);
    void onClearHistoryClicked();
    void onHistoryChanged();
    void showItemContextMenu(const QPoint& position);
    
private:
    ClipboardManager* m_clipboardManager;
    
    // UI elements
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_searchLayout;
    QHBoxLayout* m_statsLayout;
    
    QLineEdit* m_searchEdit;
    QComboBox* m_filterCombo;
    QListWidget* m_historyList;
    QLabel* m_statsLabel;
    QPushButton* m_clearButton;
    
    void setupUI();
    void applyMacStyle();
    void updateHistoryList();
    void updateHistoryList(const QList<ClipboardItem>& items);
    void updateStats();
    QListWidgetItem* createHistoryItem(const ClipboardItem& clipboardItem, int index);
    QList<ClipboardItem> getFilteredItems() const;
};

#endif // CLIPBOARDHISTORYWIDGET_H