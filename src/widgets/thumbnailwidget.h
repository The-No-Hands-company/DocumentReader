#pragma once

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QListWidgetItem>

class DocumentReader;

/**
 * Widget for displaying document page thumbnails.
 * Provides a sidebar with clickable thumbnail previews of all pages.
 */
class ThumbnailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ThumbnailWidget(QWidget *parent = nullptr);
    ~ThumbnailWidget();
    
    /**
     * Set the document to display thumbnails for.
     * @param document Pointer to the document reader, or nullptr to clear
     */
    void setDocument(DocumentReader* document);
    
    /**
     * Highlight the specified page in the thumbnail list.
     * @param pageIndex 0-based page index
     */
    void setCurrentPage(int pageIndex);

signals:
    void pageRequested(int pageIndex);

private slots:
    void onItemClicked(QListWidgetItem* item);
    void generateThumbnails();

private:
    void clearThumbnails();
    QPixmap generateThumbnail(int pageIndex);
    
    DocumentReader* m_document;
    QListWidget* m_listWidget;
    QLabel* m_statusLabel;
    
    static constexpr int THUMBNAIL_WIDTH = 120;
    static constexpr int THUMBNAIL_DPI = 36; // Low DPI for fast thumbnail generation
};
