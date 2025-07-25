#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <QObject>
#include <QClipboard>
#include <QTimer>
#include <QList>
#include "ClipboardItem.h"

class ClipboardManager : public QObject
{
    Q_OBJECT
    
public:
    explicit ClipboardManager(QObject* parent = nullptr);
    
    // History management
    const QList<ClipboardItem>& history() const { return m_history; }
    void clearHistory();
    void removeItem(int index);
    int maxHistorySize() const { return m_maxHistorySize; }
    void setMaxHistorySize(int size);
    
    // Search
    QList<ClipboardItem> search(const QString& query) const;
    
    // Statistics
    int itemCount() const { return m_history.size(); }
    
signals:
    void historyChanged();
    void newItemAdded(const ClipboardItem& item);
    
private slots:
    void onClipboardChanged();
    
private:
    QClipboard* m_clipboard;
    QList<ClipboardItem> m_history;
    QTimer* m_updateTimer;
    int m_maxHistorySize;
    QString m_lastClipboardText;
    
    void addItem(const ClipboardItem& item);
    bool isDuplicate(const ClipboardItem& item) const;
};

#endif // CLIPBOARDMANAGER_H