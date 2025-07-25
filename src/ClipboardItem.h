#ifndef CLIPBOARDITEM_H
#define CLIPBOARDITEM_H

#include <QString>
#include <QDateTime>
#include <QMimeData>
#include <QPixmap>

class ClipboardItem
{
public:
    enum ItemType {
        Text,
        Image,
        Html,
        Url,
        File,
        Code
    };
    
    ClipboardItem(const QMimeData* mimeData);
    ClipboardItem(const QString& text, ItemType type = Text);
    
    // Getters
    QString text() const { return m_text; }
    QString preview() const { return m_preview; }
    ItemType type() const { return m_type; }
    QDateTime timestamp() const { return m_timestamp; }
    QPixmap icon() const { return m_icon; }
    bool hasImage() const { return !m_image.isNull(); }
    QPixmap image() const { return m_image; }
    
    // Utility methods
    QString typeString() const;
    QString formattedTimestamp() const;
    void copyToClipboard() const;
    
    // Comparison
    bool operator==(const ClipboardItem& other) const;
    
private:
    QString m_text;
    QString m_preview;
    ItemType m_type;
    QDateTime m_timestamp;
    QPixmap m_icon;
    QPixmap m_image;
    
    void determineType();
    void generatePreview();
    void loadIcon();
    QString truncateText(const QString& text, int maxLength = 100) const;
};

#endif // CLIPBOARDITEM_H