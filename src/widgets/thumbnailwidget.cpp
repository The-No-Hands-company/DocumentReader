#include "thumbnailwidget.h"
#include "../document/documentreader.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QListWidgetItem>
#include <QPixmap>
#include <QApplication>
#include <QProgressDialog>
#include <QTimer>

ThumbnailWidget::ThumbnailWidget(QWidget *parent)
    : QWidget(parent)
    , m_document(nullptr)
    , m_listWidget(nullptr)
    , m_statusLabel(nullptr)
{
    setMinimumWidth(150);
    setMaximumWidth(200);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // Status label
    m_statusLabel = new QLabel("No document", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("QLabel { color: #888; font-style: italic; }");
    layout->addWidget(m_statusLabel);
    
    // Thumbnail list
    m_listWidget = new QListWidget(this);
    m_listWidget->setViewMode(QListWidget::IconMode);
    m_listWidget->setIconSize(QSize(THUMBNAIL_WIDTH, THUMBNAIL_WIDTH * 1.4)); // Approximate page ratio
    m_listWidget->setResizeMode(QListWidget::Adjust);
    m_listWidget->setUniformItemSizes(true);
    m_listWidget->setSpacing(5);
    m_listWidget->setStyleSheet(
        "QListWidget {"
        "    background-color: transparent;"
        "    border: none;"
        "}"
        "QListWidget::item {"
        "    background-color: #3a3a3a;"
        "    border: 2px solid transparent;"
        "    border-radius: 4px;"
        "    margin: 2px;"
        "}"
        "QListWidget::item:selected {"
        "    background-color: #4a4a4a;"
        "    border-color: #42A5F5;"
        "}"
        "QListWidget::item:hover {"
        "    background-color: #4a4a4a;"
        "    border-color: #666;"
        "}"
    );
    
    layout->addWidget(m_listWidget);
    
    // Connect signals
    connect(m_listWidget, &QListWidget::itemClicked, this, &ThumbnailWidget::onItemClicked);
}

ThumbnailWidget::~ThumbnailWidget() = default;

void ThumbnailWidget::setDocument(DocumentReader* document)
{
    m_document = document;
    clearThumbnails();
    
    if (m_document && m_document->isLoaded()) {
        m_statusLabel->setText(QString("Pages: %1").arg(m_document->pageCount()));
        
        // Generate thumbnails asynchronously
        QTimer::singleShot(100, this, &ThumbnailWidget::generateThumbnails);
    } else {
        m_statusLabel->setText("No document");
    }
}

void ThumbnailWidget::setCurrentPage(int pageIndex)
{
    if (pageIndex >= 0 && pageIndex < m_listWidget->count()) {
        m_listWidget->setCurrentRow(pageIndex);
    }
}

void ThumbnailWidget::onItemClicked(QListWidgetItem* item)
{
    if (!item) {
        return;
    }
    
    int pageIndex = item->data(Qt::UserRole).toInt();
    emit pageRequested(pageIndex);
}

void ThumbnailWidget::generateThumbnails()
{
    if (!m_document || !m_document->isLoaded()) {
        return;
    }
    
    int pageCount = m_document->pageCount();
    
    // Show progress for large documents
    QProgressDialog* progress = nullptr;
    if (pageCount > 20) {
        progress = new QProgressDialog("Generating thumbnails...", "Cancel", 0, pageCount, this);
        progress->setWindowModality(Qt::WindowModal);
        progress->show();
    }
    
    for (int i = 0; i < pageCount; ++i) {
        if (progress && progress->wasCanceled()) {
            break;
        }
        
        if (progress) {
            progress->setValue(i);
            QApplication::processEvents();
        }
        
        QPixmap thumbnail = generateThumbnail(i);
        
        QListWidgetItem* item = new QListWidgetItem;
        item->setIcon(QIcon(thumbnail));
        item->setText(QString::number(i + 1)); // 1-based page number for display
        item->setData(Qt::UserRole, i); // Store 0-based page index
        item->setToolTip(QString("Page %1").arg(i + 1));
        
        m_listWidget->addItem(item);
    }
    
    if (progress) {
        progress->setValue(pageCount);
        progress->deleteLater();
    }
}

void ThumbnailWidget::clearThumbnails()
{
    m_listWidget->clear();
}

QPixmap ThumbnailWidget::generateThumbnail(int pageIndex)
{
    if (!m_document || !m_document->isLoaded()) {
        return QPixmap();
    }
    
    // Render page at low DPI for thumbnail
    QPixmap pagePixmap = m_document->renderPage(pageIndex, THUMBNAIL_DPI);
    
    if (pagePixmap.isNull()) {
        // Create a placeholder thumbnail
        QPixmap placeholder(THUMBNAIL_WIDTH, THUMBNAIL_WIDTH * 1.4);
        placeholder.fill(Qt::darkGray);
        return placeholder;
    }
    
    // Scale to thumbnail size while maintaining aspect ratio
    QPixmap thumbnail = pagePixmap.scaled(
        THUMBNAIL_WIDTH, 
        THUMBNAIL_WIDTH * 1.4, 
        Qt::KeepAspectRatio, 
        Qt::SmoothTransformation
    );
    
    return thumbnail;
}
