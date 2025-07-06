#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>

class DocumentReader;

/**
 * Widget for displaying document pages with zoom and navigation capabilities.
 * This widget handles the main document viewing area with support for
 * zooming, panning, and page navigation.
 */
class DocumentViewer : public QScrollArea
{
    Q_OBJECT

public:
    explicit DocumentViewer(QWidget *parent = nullptr);
    ~DocumentViewer();
    
    /**
     * Set the document to display.
     * @param document Pointer to the document reader, or nullptr to clear
     */
    void setDocument(DocumentReader* document);
    
    /**
     * Get the current page index (0-based).
     * @return Current page index
     */
    int currentPage() const;
    
    /**
     * Get the current zoom factor.
     * @return Zoom factor (1.0 = 100%)
     */
    double zoomFactor() const;
    
public slots:
    void goToPage(int pageIndex);
    void nextPage();
    void previousPage();
    void zoomIn();
    void zoomOut();
    void setZoom(double factor);
    void fitToWidth();
    void fitToPage();
    void actualSize();

signals:
    void pageChanged(int pageIndex);
    void zoomChanged(double factor);

protected:
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void updateDisplay();

private:
    void renderCurrentPage();
    void updateScrollBars();
    double calculateFitToWidthZoom() const;
    double calculateFitToPageZoom() const;
    
    DocumentReader* m_document;
    QLabel* m_imageLabel;
    
    int m_currentPage;
    double m_zoomFactor;
    double m_dpi;
    
    // Mouse interaction
    bool m_dragging;
    QPoint m_lastPanPoint;
    
    // Auto-fit modes
    enum class FitMode {
        None,
        Width,
        Page
    };
    FitMode m_fitMode;
};
