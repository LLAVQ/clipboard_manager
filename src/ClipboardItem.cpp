#include "ClipboardItem.h"
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QUrl>
#include <QRegularExpression>
#include <QPainter>
#include <QStyle>

ClipboardItem::ClipboardItem(const QMimeData* mimeData)
    : m_timestamp(QDateTime::currentDateTime())
{
    if (mimeData->hasImage()) {
        m_image = qvariant_cast<QPixmap>(mimeData->imageData());
        m_type = Image;
        m_text = QString("Image (%1x%2)").arg(m_image.width()).arg(m_image.height());
    } else if (mimeData->hasHtml()) {
        m_text = mimeData->html();
        m_type = Html;
    } else if (mimeData->hasText()) {
        m_text = mimeData->text();
        determineType();
    } else {
        m_text = "Unknown format";
        m_type = Text;
    }
    
    generatePreview();
    loadIcon();
}

ClipboardItem::ClipboardItem(const QString& text, ItemType type)
    : m_text(text), m_type(type), m_timestamp(QDateTime::currentDateTime())
{
    if (type == Text) {
        determineType();
    }
    generatePreview();
    loadIcon();
}

QString ClipboardItem::typeString() const
{
    switch (m_type) {
        case Text: return "Text";
        case Image: return "Image";
        case Html: return "HTML";
        case Url: return "URL";
        case File: return "File";
        case Code: return "Code";
        default: return "Unknown";
    }
}

QString ClipboardItem::formattedTimestamp() const
{
    const QDateTime now = QDateTime::currentDateTime();
    const qint64 secondsAgo = m_timestamp.secsTo(now);
    
    if (secondsAgo < 60) {
        return "Just now";
    } else if (secondsAgo < 3600) {
        const int minutes = secondsAgo / 60;
        return QString("%1 minute%2 ago").arg(minutes).arg(minutes == 1 ? "" : "s");
    } else if (secondsAgo < 86400) {
        const int hours = secondsAgo / 3600;
        return QString("%1 hour%2 ago").arg(hours).arg(hours == 1 ? "" : "s");
    } else {
        return m_timestamp.toString("MMM dd, hh:mm");
    }
}

void ClipboardItem::copyToClipboard() const
{
    QClipboard* clipboard = QApplication::clipboard();
    if (m_type == Image && !m_image.isNull()) {
        clipboard->setPixmap(m_image);
    } else {
        clipboard->setText(m_text);
    }
}

bool ClipboardItem::operator==(const ClipboardItem& other) const
{
    return m_text == other.m_text && m_type == other.m_type;
}

void ClipboardItem::determineType()
{
    // Check if it's a URL
    QRegularExpression urlRegex(R"(^https?://[^\s]+$)");
    if (urlRegex.match(m_text.trimmed()).hasMatch()) {
        m_type = Url;
        return;
    }
    
    // Check if it's code (contains common programming patterns)
    QRegularExpression codeRegex(R"(\b(function|class|import|export|const|let|var|if|else|for|while|return)\b|[{}();]|\s{4,}|\t)");
    if (codeRegex.match(m_text).hasMatch() || m_text.contains("```")) {
        m_type = Code;
        return;
    }
    
    // Check if it's a file path
    QRegularExpression fileRegex(R"(^[/\\]?([^/\\]+[/\\])*[^/\\]+\.[a-zA-Z0-9]+$)");
    if (fileRegex.match(m_text.trimmed()).hasMatch()) {
        m_type = File;
        return;
    }
    
    // Default to text
    m_type = Text;
}

void ClipboardItem::generatePreview()
{
    if (m_type == Image) {
        m_preview = QString("Image (%1x%2)").arg(m_image.width()).arg(m_image.height());
    } else {
        m_preview = truncateText(m_text.simplified());
    }
}

void ClipboardItem::loadIcon()
{
    QStyle* style = QApplication::style();
    QPixmap baseIcon;
    
    switch (m_type) {
        case Text:
            baseIcon = style->standardIcon(QStyle::SP_FileDialogDetailView).pixmap(16, 16);
            break;
        case Image:
            baseIcon = style->standardIcon(QStyle::SP_FileDialogDetailView).pixmap(16, 16);
            break;
        case Html:
            baseIcon = style->standardIcon(QStyle::SP_ComputerIcon).pixmap(16, 16);
            break;
        case Url:
            baseIcon = style->standardIcon(QStyle::SP_DriveNetIcon).pixmap(16, 16);
            break;
        case File:
            baseIcon = style->standardIcon(QStyle::SP_FileIcon).pixmap(16, 16);
            break;
        case Code:
            baseIcon = style->standardIcon(QStyle::SP_FileDialogDetailView).pixmap(16, 16);
            break;
    }
    
    m_icon = baseIcon;
}

QString ClipboardItem::truncateText(const QString& text, int maxLength) const
{
    if (text.length() <= maxLength) {
        return text;
    }
    return text.left(maxLength) + "...";
}