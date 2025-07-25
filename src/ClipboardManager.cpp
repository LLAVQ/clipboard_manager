#include "ClipboardManager.h"
#include <QApplication>
#include <QMimeData>

ClipboardManager::ClipboardManager(QObject* parent)
    : QObject(parent)
    , m_clipboard(QApplication::clipboard())
    , m_maxHistorySize(100)
    , m_updateTimer(new QTimer(this))
{
    // Connect clipboard signals
    connect(m_clipboard, &QClipboard::dataChanged, this, &ClipboardManager::onClipboardChanged);
    
    // Timer to prevent rapid updates
    m_updateTimer->setSingleShot(true);
    m_updateTimer->setInterval(100); // 100ms delay
    connect(m_updateTimer, &QTimer::timeout, this, &ClipboardManager::onClipboardChanged);
    
    // Initialize with current clipboard content
    onClipboardChanged();
}

void ClipboardManager::clearHistory()
{
    m_history.clear();
    emit historyChanged();
}

void ClipboardManager::removeItem(int index)
{
    if (index >= 0 && index < m_history.size()) {
        m_history.removeAt(index);
        emit historyChanged();
    }
}

void ClipboardManager::setMaxHistorySize(int size)
{
    m_maxHistorySize = qMax(1, size);
    
    // Trim history if needed
    while (m_history.size() > m_maxHistorySize) {
        m_history.removeLast();
    }
    
    if (m_history.size() < size) {
        emit historyChanged();
    }
}

QList<ClipboardItem> ClipboardManager::search(const QString& query) const
{
    QList<ClipboardItem> results;
    
    if (query.isEmpty()) {
        return m_history;
    }
    
    const QString lowerQuery = query.toLower();
    
    for (const auto& item : m_history) {
        if (item.text().toLower().contains(lowerQuery) ||
            item.preview().toLower().contains(lowerQuery) ||
            item.typeString().toLower().contains(lowerQuery)) {
            results.append(item);
        }
    }
    
    return results;
}

void ClipboardManager::onClipboardChanged()
{
    const QMimeData* mimeData = m_clipboard->mimeData();
    if (!mimeData) {
        return;
    }
    
    // Prevent processing our own clipboard changes
    if (sender() == m_clipboard && m_updateTimer->isActive()) {
        return;
    }
    
    ClipboardItem newItem(mimeData);
    
    // Skip empty or duplicate items
    if (newItem.text().isEmpty() || isDuplicate(newItem)) {
        return;
    }
    
    addItem(newItem);
}

void ClipboardManager::addItem(const ClipboardItem& item)
{
    // Remove existing duplicate if found
    for (int i = 0; i < m_history.size(); ++i) {
        if (m_history[i] == item) {
            m_history.removeAt(i);
            break;
        }
    }
    
    // Add to beginning of history
    m_history.prepend(item);
    
    // Trim history if it exceeds max size
    while (m_history.size() > m_maxHistorySize) {
        m_history.removeLast();
    }
    
    emit newItemAdded(item);
    emit historyChanged();
}

bool ClipboardManager::isDuplicate(const ClipboardItem& item) const
{
    if (m_history.isEmpty()) {
        return false;
    }
    
    // Check if the most recent item is the same
    return m_history.first() == item;
}