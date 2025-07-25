#include "ClipboardHistoryWidget.h"
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>

ClipboardHistoryWidget::ClipboardHistoryWidget(QWidget* parent)
    : QWidget(parent)
    , m_clipboardManager(nullptr)
{
    setupUI();
    applyMacStyle();
}

void ClipboardHistoryWidget::setClipboardManager(ClipboardManager* manager)
{
    if (m_clipboardManager) {
        disconnect(m_clipboardManager, nullptr, this, nullptr);
    }
    
    m_clipboardManager = manager;
    
    if (m_clipboardManager) {
        connect(m_clipboardManager, &ClipboardManager::historyChanged,
                this, &ClipboardHistoryWidget::onHistoryChanged);
        
        updateHistoryList();
        updateStats();
    }
}

void ClipboardHistoryWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(12);
    
    // Search and filter layout
    m_searchLayout = new QHBoxLayout();
    
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Search clipboard history...");
    m_searchEdit->setObjectName("searchEdit");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &ClipboardHistoryWidget::onSearchTextChanged);
    
    m_filterCombo = new QComboBox();
    m_filterCombo->setObjectName("filterCombo");
    m_filterCombo->addItem("All Types", -1);
    m_filterCombo->addItem("Text", ClipboardItem::Text);
    m_filterCombo->addItem("Images", ClipboardItem::Image);
    m_filterCombo->addItem("URLs", ClipboardItem::Url);
    m_filterCombo->addItem("Code", ClipboardItem::Code);
    m_filterCombo->addItem("Files", ClipboardItem::File);
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ClipboardHistoryWidget::onFilterChanged);
    
    m_searchLayout->addWidget(m_searchEdit, 1);
    m_searchLayout->addWidget(m_filterCombo);
    
    // History list
    m_historyList = new QListWidget();
    m_historyList->setObjectName("historyList");
    m_historyList->setAlternatingRowColors(true);
    m_historyList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_historyList->setContextMenuPolicy(Qt::CustomContextMenu);
    
    connect(m_historyList, &QListWidget::itemClicked, this, &ClipboardHistoryWidget::onItemClicked);
    connect(m_historyList, &QListWidget::itemDoubleClicked, this, &ClipboardHistoryWidget::onItemDoubleClicked);
    connect(m_historyList, &QListWidget::customContextMenuRequested,
            this, &ClipboardHistoryWidget::showItemContextMenu);
    
    // Stats and controls layout
    m_statsLayout = new QHBoxLayout();
    
    m_statsLabel = new QLabel("0 items");
    m_statsLabel->setObjectName("statsLabel");
    
    m_clearButton = new QPushButton("Clear History");
    m_clearButton->setObjectName("clearButton");
    connect(m_clearButton, &QPushButton::clicked, this, &ClipboardHistoryWidget::onClearHistoryClicked);
    
    m_statsLayout->addWidget(m_statsLabel);
    m_statsLayout->addStretch();
    m_statsLayout->addWidget(m_clearButton);
    
    // Add to main layout
    m_mainLayout->addLayout(m_searchLayout);
    m_mainLayout->addWidget(m_historyList, 1);
    m_mainLayout->addLayout(m_statsLayout);
}

void ClipboardHistoryWidget::applyMacStyle()
{
    setStyleSheet(R"(
        QLineEdit#searchEdit {
            border: 2px solid #e0e0e0;
            border-radius: 8px;
            padding: 10px 12px;
            font-size: 14px;
            background-color: white;
        }
        
        QLineEdit#searchEdit:focus {
            border: 2px solid #007AFF;
        }
        
        QComboBox#filterCombo {
            border: 2px solid #e0e0e0;
            border-radius: 8px;
            padding: 10px 12px;
            font-size: 14px;
            background-color: white;
            min-width: 120px;
        }
        
        QComboBox#filterCombo:focus {
            border: 2px solid #007AFF;
        }
        
        QComboBox#filterCombo::drop-down {
            border: none;
            width: 20px;
        }
        
        QComboBox#filterCombo::down-arrow {
            width: 12px;
            height: 12px;
        }
        
        QListWidget#historyList {
            border: 2px solid #e0e0e0;
            border-radius: 8px;
            background-color: white;
            outline: none;
            padding: 4px;
        }
        
        QListWidget#historyList::item {
            border: none;
            border-radius: 6px;
            padding: 12px;
            margin: 2px;
            min-height: 40px;
        }
        
        QListWidget#historyList::item:hover {
            background-color: rgba(0, 122, 255, 0.1);
        }
        
        QListWidget#historyList::item:selected {
            background-color: rgba(0, 122, 255, 0.2);
            color: black;
        }
        
        QListWidget#historyList::item:alternate {
            background-color: rgba(0, 0, 0, 0.02);
        }
        
        QLabel#statsLabel {
            color: #666;
            font-size: 13px;
        }
        
        QPushButton#clearButton {
            background-color: #FF3B30;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 13px;
            font-weight: 500;
        }
        
        QPushButton#clearButton:hover {
            background-color: #D70015;
        }
        
        QPushButton#clearButton:pressed {
            background-color: #A60000;
        }
    )");
}

void ClipboardHistoryWidget::onSearchTextChanged()
{
    updateHistoryList();
}

void ClipboardHistoryWidget::onFilterChanged()
{
    updateHistoryList();
}

void ClipboardHistoryWidget::onItemClicked(QListWidgetItem* item)
{
    if (!item || !m_clipboardManager) return;
    
    int index = item->data(Qt::UserRole).toInt();
    const QList<ClipboardItem>& history = m_clipboardManager->history();
    
    if (index >= 0 && index < history.size()) {
        // Just select, don't copy yet (wait for double-click or Enter)
    }
}

void ClipboardHistoryWidget::onItemDoubleClicked(QListWidgetItem* item)
{
    if (!item || !m_clipboardManager) return;
    
    int index = item->data(Qt::UserRole).toInt();
    const QList<ClipboardItem>& history = m_clipboardManager->history();
    
    if (index >= 0 && index < history.size()) {
        history[index].copyToClipboard();
        
        // Show feedback
        m_historyList->setToolTip("Copied to clipboard!");
        QTimer::singleShot(1000, [this]() {
            m_historyList->setToolTip("");
        });
    }
}

void ClipboardHistoryWidget::onClearHistoryClicked()
{
    if (!m_clipboardManager) return;
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Clear History",
        "Are you sure you want to clear all clipboard history?\nThis action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        m_clipboardManager->clearHistory();
    }
}

void ClipboardHistoryWidget::onHistoryChanged()
{
    updateHistoryList();
    updateStats();
}

void ClipboardHistoryWidget::showItemContextMenu(const QPoint& position)
{
    QListWidgetItem* item = m_historyList->itemAt(position);
    if (!item || !m_clipboardManager) return;
    
    int index = item->data(Qt::UserRole).toInt();
    const QList<ClipboardItem>& history = m_clipboardManager->history();
    
    if (index < 0 || index >= history.size()) return;
    
    QMenu contextMenu(this);
    
    QAction* copyAction = contextMenu.addAction("Copy to Clipboard");
    copyAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton));
    
    contextMenu.addSeparator();
    
    QAction* removeAction = contextMenu.addAction("Remove");
    removeAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_TrashIcon));
    
    QAction* selectedAction = contextMenu.exec(m_historyList->mapToGlobal(position));
    
    if (selectedAction == copyAction) {
        history[index].copyToClipboard();
    } else if (selectedAction == removeAction) {
        m_clipboardManager->removeItem(index);
    }
}

void ClipboardHistoryWidget::updateHistoryList()
{
    if (!m_clipboardManager) return;
    
    const QList<ClipboardItem> items = getFilteredItems();
    updateHistoryList(items);
}

void ClipboardHistoryWidget::updateHistoryList(const QList<ClipboardItem>& items)
{
    m_historyList->clear();
    
    for (int i = 0; i < items.size(); ++i) {
        const ClipboardItem& item = items[i];
        QListWidgetItem* listItem = createHistoryItem(item, i);
        m_historyList->addItem(listItem);
    }
    
    if (items.isEmpty()) {
        QListWidgetItem* emptyItem = new QListWidgetItem("No clipboard items match your search");
        emptyItem->setFlags(Qt::NoItemFlags);
        emptyItem->setTextAlignment(Qt::AlignCenter);
        m_historyList->addItem(emptyItem);
    }
}

void ClipboardHistoryWidget::updateStats()
{
    if (!m_clipboardManager) {
        m_statsLabel->setText("0 items");
        return;
    }
    
    const int totalItems = m_clipboardManager->itemCount();
    const int filteredItems = getFilteredItems().size();
    
    if (m_searchEdit->text().isEmpty() && m_filterCombo->currentData().toInt() == -1) {
        m_statsLabel->setText(QString("%1 item%2").arg(totalItems).arg(totalItems == 1 ? "" : "s"));
    } else {
        m_statsLabel->setText(QString("%1 of %2 item%3").arg(filteredItems).arg(totalItems).arg(totalItems == 1 ? "" : "s"));
    }
}

QListWidgetItem* ClipboardHistoryWidget::createHistoryItem(const ClipboardItem& clipboardItem, int index)
{
    QListWidgetItem* item = new QListWidgetItem();
    
    // Set main text
    QString mainText = clipboardItem.preview();
    
    // Add type and timestamp info
    QString subtitle = QString("%1 • %2")
                      .arg(clipboardItem.typeString())
                      .arg(clipboardItem.formattedTimestamp());
    
    QString displayText = QString("%1\n%2").arg(mainText).arg(subtitle);
    
    item->setText(displayText);
    item->setIcon(clipboardItem.icon());
    item->setData(Qt::UserRole, index); // Store index for reference
    item->setToolTip(QString("Double-click to copy\nOriginal: %1").arg(clipboardItem.text()));
    
    return item;
}

QList<ClipboardItem> ClipboardHistoryWidget::getFilteredItems() const
{
    if (!m_clipboardManager) {
        return QList<ClipboardItem>();
    }
    
    // Start with search results
    const QString searchText = m_searchEdit->text();
    QList<ClipboardItem> items = m_clipboardManager->search(searchText);
    
    // Apply type filter
    const int filterType = m_filterCombo->currentData().toInt();
    if (filterType != -1) {
        QList<ClipboardItem> filteredItems;
        for (const ClipboardItem& item : items) {
            if (item.type() == static_cast<ClipboardItem::ItemType>(filterType)) {
                filteredItems.append(item);
            }
        }
        items = filteredItems;
    }
    
    return items;
}