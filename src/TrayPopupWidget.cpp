#include "TrayPopupWidget.h"
#include <QApplication>
#include <QListWidgetItem>
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsDropShadowEffect>

TrayPopupWidget::TrayPopupWidget(ClipboardManager* clipboardManager, QWidget* parent)
    : QWidget(parent)
    , m_clipboardManager(clipboardManager)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_ShowWithoutActivating, false);
    setFocusPolicy(Qt::StrongFocus);
    
    setupUI();
    applyMacStyle();
    
    // Connect clipboard manager signals
    connect(m_clipboardManager, &ClipboardManager::historyChanged,
            this, &TrayPopupWidget::updateHistoryList);
    
    // Initial update
    updateHistoryList();
}

void TrayPopupWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    
    // Header
    m_headerLayout = new QHBoxLayout();
    m_headerLayout->setContentsMargins(12, 8, 12, 8);
    
    m_titleLabel = new QLabel("Clipboard");
    m_titleLabel->setObjectName("titleLabel");
    
    m_headerLayout->addWidget(m_titleLabel);
    m_headerLayout->addStretch();
    
    // Search
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Search clipboard...");
    m_searchEdit->setObjectName("searchEdit");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &TrayPopupWidget::onSearchTextChanged);
    
    // History list
    m_historyList = new QListWidget();
    m_historyList->setObjectName("historyList");
    m_historyList->setAlternatingRowColors(false);
    m_historyList->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_historyList, &QListWidget::itemClicked, this, &TrayPopupWidget::onItemClicked);
    connect(m_historyList, &QListWidget::itemDoubleClicked, this, &TrayPopupWidget::onItemDoubleClicked);
    
    // Footer
    m_footerLayout = new QHBoxLayout();
    m_footerLayout->setContentsMargins(8, 4, 8, 8);
    
    m_openAppButton = new QPushButton("Open App");
    m_openAppButton->setObjectName("openAppButton");
    connect(m_openAppButton, &QPushButton::clicked, this, &TrayPopupWidget::onOpenMainWindowClicked);
    
    m_clearButton = new QPushButton("Clear");
    m_clearButton->setObjectName("clearButton");
    connect(m_clearButton, &QPushButton::clicked, this, &TrayPopupWidget::onClearHistoryClicked);
    
    m_footerLayout->addWidget(m_openAppButton);
    m_footerLayout->addStretch();
    m_footerLayout->addWidget(m_clearButton);
    
    // Add to main layout
    m_mainLayout->addLayout(m_headerLayout);
    m_mainLayout->addWidget(m_searchEdit);
    m_mainLayout->addWidget(m_historyList, 1);
    m_mainLayout->addLayout(m_footerLayout);
}

void TrayPopupWidget::applyMacStyle()
{
    setStyleSheet(R"(
        TrayPopupWidget {
            background-color: rgba(248, 248, 248, 245);
            border: 1px solid rgba(0, 0, 0, 0.15);
            border-radius: 8px;
        }
        
        QLabel#titleLabel {
            font-weight: 600;
            font-size: 13px;
            color: #333;
        }
        
        QLineEdit#searchEdit {
            border: 1px solid rgba(0, 0, 0, 0.15);
            border-radius: 6px;
            padding: 6px 10px;
            margin: 8px 12px;
            background-color: white;
            font-size: 12px;
        }
        
        QLineEdit#searchEdit:focus {
            border: 2px solid #007AFF;
            margin: 7px 11px;
        }
        
        QListWidget#historyList {
            border: none;
            background-color: transparent;
            outline: none;
            margin: 0 8px;
        }
        
        QListWidget#historyList::item {
            border: none;
            padding: 8px;
            margin: 1px 0px;
            border-radius: 4px;
            min-height: 20px;
        }
        
        QListWidget#historyList::item:hover {
            background-color: rgba(0, 122, 255, 0.1);
        }
        
        QListWidget#historyList::item:selected {
            background-color: rgba(0, 122, 255, 0.2);
        }
        
        QPushButton#openAppButton {
            background-color: #007AFF;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 4px 12px;
            font-size: 11px;
            font-weight: 500;
        }
        
        QPushButton#openAppButton:hover {
            background-color: #0056CC;
        }
        
        QPushButton#clearButton {
            background-color: transparent;
            color: #666;
            border: none;
            border-radius: 4px;
            padding: 4px 12px;
            font-size: 11px;
        }
        
        QPushButton#clearButton:hover {
            background-color: rgba(255, 0, 0, 0.1);
            color: #FF3B30;
        }
    )");
    
    // Add drop shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 60));
    shadow->setOffset(0, 4);
    setGraphicsEffect(shadow);
}

void TrayPopupWidget::refreshHistory()
{
    updateHistoryList();
}

void TrayPopupWidget::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    
    // Focus the search edit when shown
    m_searchEdit->setFocus();
    m_searchEdit->clear();
    
    // Refresh history
    updateHistoryList();
}

void TrayPopupWidget::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);
    
    // Clear search when hidden
    m_searchEdit->clear();
}

void TrayPopupWidget::focusOutEvent(QFocusEvent* event)
{
    QWidget::focusOutEvent(event);
    
    // Hide popup when focus is lost (with a small delay)
    QTimer::singleShot(100, this, [this]() {
        if (!hasFocus() && !m_searchEdit->hasFocus() && !m_historyList->hasFocus()) {
            hide();
        }
    });
}

void TrayPopupWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        hide();
        return;
    }
    
    QWidget::keyPressEvent(event);
}

void TrayPopupWidget::onSearchTextChanged()
{
    const QString searchText = m_searchEdit->text();
    const QList<ClipboardItem> results = m_clipboardManager->search(searchText);
    updateHistoryList(results);
}

void TrayPopupWidget::onItemClicked(QListWidgetItem* item)
{
    if (!item) return;
    
    int index = item->data(Qt::UserRole).toInt();
    const QList<ClipboardItem>& history = m_clipboardManager->history();
    
    if (index >= 0 && index < history.size()) {
        history[index].copyToClipboard();
        hide();
    }
}

void TrayPopupWidget::onItemDoubleClicked(QListWidgetItem* item)
{
    onItemClicked(item);
}

void TrayPopupWidget::onClearHistoryClicked()
{
    m_clipboardManager->clearHistory();
    hide();
}

void TrayPopupWidget::onOpenMainWindowClicked()
{
    emit openMainWindow();
    hide();
}

void TrayPopupWidget::updateHistoryList()
{
    updateHistoryList(m_clipboardManager->history());
}

void TrayPopupWidget::updateHistoryList(const QList<ClipboardItem>& items)
{
    m_historyList->clear();
    
    for (int i = 0; i < items.size() && i < 10; ++i) { // Show max 10 items in popup
        const ClipboardItem& item = items[i];
        QListWidgetItem* listItem = createHistoryItem(item);
        listItem->setData(Qt::UserRole, i); // Store original index
        m_historyList->addItem(listItem);
    }
    
    if (items.isEmpty()) {
        QListWidgetItem* emptyItem = new QListWidgetItem("No clipboard items");
        emptyItem->setFlags(Qt::NoItemFlags);
        emptyItem->setTextAlignment(Qt::AlignCenter);
        m_historyList->addItem(emptyItem);
    }
}

QListWidgetItem* TrayPopupWidget::createHistoryItem(const ClipboardItem& clipboardItem)
{
    QListWidgetItem* item = new QListWidgetItem();
    
    // Set text with type and preview
    QString displayText = QString("%1 • %2")
                         .arg(clipboardItem.typeString())
                         .arg(clipboardItem.preview());
    
    item->setText(displayText);
    item->setIcon(clipboardItem.icon());
    item->setToolTip(QString("%1\n%2").arg(clipboardItem.formattedTimestamp()).arg(clipboardItem.text()));
    
    return item;
}