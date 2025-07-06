#include "documentviewer.h"
#include "../document/documentreader.h"
#include <QVBoxLayout>
#include <QScrollBar>
#include <QApplication>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QDebug>
#include <cmath>

DocumentViewer::DocumentViewer(QWidget *parent)
    : QScrollArea(parent)
    , m_document(nullptr)
    , m_imageLabel(nullptr)
    , m_currentPage(0)
    , m_zoomFactor(1.0)
    , m_dpi(96.0) // Standard screen DPI
    , m_dragging(false)
    , m_fitMode(FitMode::None)
{
    setWidgetResizable(true);
    setAlignment(Qt::AlignCenter);
    setBackgroundRole(QPalette::Dark);
    
    m_imageLabel = new QLabel;
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setScaledContents(false);
    m_imageLabel->setBackgroundRole(QPalette::Base);
    m_imageLabel->setAutoFillBackground(true);
    m_imageLabel->setMinimumSize(100, 100);
    
    setWidget(m_imageLabel);
    
    // Set up mouse tracking for panning
    setMouseTracking(true);
    m_imageLabel->setMouseTracking(true);
}

DocumentViewer::~DocumentViewer() = default;

void DocumentViewer::setDocument(DocumentReader* document)
{
    m_document = document;
    m_currentPage = 0;
    m_zoomFactor = 1.0;
    m_fitMode = FitMode::None;
    
    if (m_document && m_document->isLoaded()) {
        renderCurrentPage();
        emit pageChanged(m_currentPage);
        emit zoomChanged(m_zoomFactor);
    } else {
        m_imageLabel->clear();
        m_imageLabel->setText("No document loaded");
    }
}

int DocumentViewer::currentPage() const
{
    return m_currentPage;
}

double DocumentViewer::zoomFactor() const
{
    return m_zoomFactor;
}

void DocumentViewer::goToPage(int pageIndex)
{
    if (!m_document || !m_document->isLoaded()) {
        return;
    }
    
    if (pageIndex < 0 || pageIndex >= m_document->pageCount()) {
        return;
    }
    
    m_currentPage = pageIndex;
    renderCurrentPage();
    emit pageChanged(m_currentPage);
}

void DocumentViewer::nextPage()
{
    if (!m_document || !m_document->isLoaded()) {
        return;
    }
    
    if (m_currentPage < m_document->pageCount() - 1) {
        goToPage(m_currentPage + 1);
    }
}

void DocumentViewer::previousPage()
{
    if (!m_document || !m_document->isLoaded()) {
        return;
    }
    
    if (m_currentPage > 0) {
        goToPage(m_currentPage - 1);
    }
}

void DocumentViewer::zoomIn()
{
    setZoom(m_zoomFactor * 1.25);
}

void DocumentViewer::zoomOut()
{
    setZoom(m_zoomFactor / 1.25);
}

void DocumentViewer::setZoom(double factor)
{
    if (!m_document || !m_document->isLoaded()) {
        return;
    }
    
    // Limit zoom range
    factor = std::max(0.1, std::min(factor, 10.0));
    
    if (std::abs(factor - m_zoomFactor) < 0.001) {
        return; // No significant change
    }
    
    m_zoomFactor = factor;
    m_fitMode = FitMode::None;
    
    renderCurrentPage();
    emit zoomChanged(m_zoomFactor);
}

void DocumentViewer::fitToWidth()
{
    if (!m_document || !m_document->isLoaded()) {
        return;
    }
    
    m_fitMode = FitMode::Width;
    double zoom = calculateFitToWidthZoom();
    m_zoomFactor = zoom;
    
    renderCurrentPage();
    emit zoomChanged(m_zoomFactor);
}

void DocumentViewer::fitToPage()
{
    if (!m_document || !m_document->isLoaded()) {
        return;
    }
    
    m_fitMode = FitMode::Page;
    double zoom = calculateFitToPageZoom();
    m_zoomFactor = zoom;
    
    renderCurrentPage();
    emit zoomChanged(m_zoomFactor);
}

void DocumentViewer::actualSize()
{
    setZoom(1.0);
}

void DocumentViewer::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        // Zoom with Ctrl+Wheel
        if (event->angleDelta().y() > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
        event->accept();
    } else {
        // Normal scrolling
        QScrollArea::wheelEvent(event);
    }
}

void DocumentViewer::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    } else {
        QScrollArea::mousePressEvent(event);
    }
}

void DocumentViewer::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging) {
        QPoint delta = event->pos() - m_lastPanPoint;
        m_lastPanPoint = event->pos();
        
        // Pan the view
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        
        event->accept();
    } else {
        QScrollArea::mouseMoveEvent(event);
    }
}

void DocumentViewer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    } else {
        QScrollArea::mouseReleaseEvent(event);
    }
}

void DocumentViewer::resizeEvent(QResizeEvent* event)
{
    QScrollArea::resizeEvent(event);
    
    // Reapply fit modes when window is resized
    if (m_fitMode == FitMode::Width) {
        fitToWidth();
    } else if (m_fitMode == FitMode::Page) {
        fitToPage();
    }
}

void DocumentViewer::updateDisplay()
{
    renderCurrentPage();
}

void DocumentViewer::renderCurrentPage()
{
    if (!m_document || !m_document->isLoaded()) {
        m_imageLabel->clear();
        m_imageLabel->setText("No document loaded");
        return;
    }
    
    if (m_currentPage < 0 || m_currentPage >= m_document->pageCount()) {
        m_imageLabel->clear();
        m_imageLabel->setText("Invalid page");
        return;
    }
    
    // Calculate DPI based on zoom factor
    double renderDpi = m_dpi * m_zoomFactor;
    
    // Render the page
    QPixmap pixmap = m_document->renderPage(m_currentPage, renderDpi);
    
    if (pixmap.isNull()) {
        m_imageLabel->clear();
        m_imageLabel->setText("Failed to render page");
        return;
    }
    
    m_imageLabel->setPixmap(pixmap);
    m_imageLabel->resize(pixmap.size());
}

double DocumentViewer::calculateFitToWidthZoom() const
{
    if (!m_document || !m_document->isLoaded()) {
        return 1.0;
    }
    
    QSizeF pageSize = m_document->pageSize(m_currentPage);
    if (pageSize.isEmpty()) {
        return 1.0;
    }
    
    // Get available width (subtract scrollbar width and margins)
    int availableWidth = viewport()->width() - 20; // Small margin
    
    // Calculate zoom to fit width
    double pageWidthInPixels = pageSize.width() * m_dpi / 72.0; // Convert points to pixels
    double zoom = static_cast<double>(availableWidth) / pageWidthInPixels;
    
    return std::max(0.1, std::min(zoom, 10.0));
}

double DocumentViewer::calculateFitToPageZoom() const
{
    if (!m_document || !m_document->isLoaded()) {
        return 1.0;
    }
    
    QSizeF pageSize = m_document->pageSize(m_currentPage);
    if (pageSize.isEmpty()) {
        return 1.0;
    }
    
    // Get available space (subtract scrollbar dimensions and margins)
    int availableWidth = viewport()->width() - 20;
    int availableHeight = viewport()->height() - 20;
    
    // Calculate zoom to fit both dimensions
    double pageWidthInPixels = pageSize.width() * m_dpi / 72.0;
    double pageHeightInPixels = pageSize.height() * m_dpi / 72.0;
    
    double zoomWidth = static_cast<double>(availableWidth) / pageWidthInPixels;
    double zoomHeight = static_cast<double>(availableHeight) / pageHeightInPixels;
    
    // Use the smaller zoom to ensure the entire page fits
    double zoom = std::min(zoomWidth, zoomHeight);
    
    return std::max(0.1, std::min(zoom, 10.0));
}
