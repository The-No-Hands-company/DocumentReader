#include "mainwindow.h"
#include "widgets/documentviewer.h"
#include "widgets/thumbnailwidget.h"
#include "document/documentfactory.h"
#include "document/documentreader.h"

#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QProgressBar>
#include <QLabel>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_documentViewer(nullptr)
    , m_thumbnailDock(nullptr)
    , m_thumbnailWidget(nullptr)
    , m_pageLabel(nullptr)
    , m_zoomLabel(nullptr)
    , m_progressBar(nullptr)
{
    setWindowTitle("Document Reader");
    setMinimumSize(800, 600);
    resize(1200, 800);
    
    // Create main components
    m_documentViewer = new DocumentViewer(this);
    setCentralWidget(m_documentViewer);
    
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWidgets();
    
    updateActions();
    
    // Connect document viewer signals
    connect(m_documentViewer, &DocumentViewer::pageChanged, this, &MainWindow::updateStatusBar);
    connect(m_documentViewer, &DocumentViewer::zoomChanged, this, &MainWindow::updateStatusBar);
}

MainWindow::~MainWindow() = default;

void MainWindow::createActions()
{
    // File actions
    m_openAction = new QAction("&Open...", this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip("Open a document");
    m_openAction->setIcon(QIcon(":/icons/open.svg"));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openDocument);
    
    m_closeAction = new QAction("&Close", this);
    m_closeAction->setShortcut(QKeySequence::Close);
    m_closeAction->setStatusTip("Close the current document");
    m_closeAction->setIcon(QIcon(":/icons/close.svg"));
    connect(m_closeAction, &QAction::triggered, this, &MainWindow::closeDocument);
    
    m_printAction = new QAction("&Print...", this);
    m_printAction->setShortcut(QKeySequence::Print);
    m_printAction->setStatusTip("Print the current document");
    m_printAction->setIcon(QIcon(":/icons/print.svg"));
    connect(m_printAction, &QAction::triggered, this, &MainWindow::printDocument);
    
    m_exitAction = new QAction("E&xit", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip("Exit the application");
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    
    // View actions
    m_zoomInAction = new QAction("Zoom &In", this);
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    m_zoomInAction->setStatusTip("Zoom in");
    m_zoomInAction->setIcon(QIcon(":/icons/zoom_in.svg"));
    connect(m_zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    
    m_zoomOutAction = new QAction("Zoom &Out", this);
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    m_zoomOutAction->setStatusTip("Zoom out");
    m_zoomOutAction->setIcon(QIcon(":/icons/zoom_out.svg"));
    connect(m_zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    
    m_fitToWidthAction = new QAction("Fit to &Width", this);
    m_fitToWidthAction->setStatusTip("Fit document to window width");
    m_fitToWidthAction->setIcon(QIcon(":/icons/fit_width.svg"));
    connect(m_fitToWidthAction, &QAction::triggered, this, &MainWindow::fitToWidth);
    
    m_fitToPageAction = new QAction("Fit to &Page", this);
    m_fitToPageAction->setStatusTip("Fit entire page in window");
    m_fitToPageAction->setIcon(QIcon(":/icons/fit_page.svg"));
    connect(m_fitToPageAction, &QAction::triggered, this, &MainWindow::fitToPage);
    
    m_actualSizeAction = new QAction("&Actual Size", this);
    m_actualSizeAction->setStatusTip("Show document at actual size");
    connect(m_actualSizeAction, &QAction::triggered, this, &MainWindow::actualSize);
    
    // Navigation actions
    m_goToPageAction = new QAction("&Go to Page...", this);
    m_goToPageAction->setShortcut(Qt::CTRL | Qt::Key_G);
    m_goToPageAction->setStatusTip("Go to specific page");
    connect(m_goToPageAction, &QAction::triggered, this, &MainWindow::goToPage);
    
    m_nextPageAction = new QAction("&Next Page", this);
    m_nextPageAction->setShortcut(Qt::Key_PageDown);
    m_nextPageAction->setStatusTip("Go to next page");
    m_nextPageAction->setIcon(QIcon(":/icons/next_page.svg"));
    connect(m_nextPageAction, &QAction::triggered, this, &MainWindow::nextPage);
    
    m_previousPageAction = new QAction("&Previous Page", this);
    m_previousPageAction->setShortcut(Qt::Key_PageUp);
    m_previousPageAction->setStatusTip("Go to previous page");
    m_previousPageAction->setIcon(QIcon(":/icons/previous_page.svg"));
    connect(m_previousPageAction, &QAction::triggered, this, &MainWindow::previousPage);
    
    // Help actions
    m_aboutAction = new QAction("&About", this);
    m_aboutAction->setStatusTip("Show information about the application");
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu("&File");
    m_fileMenu->addAction(m_openAction);
    m_fileMenu->addAction(m_closeAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_printAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);
    
    m_viewMenu = menuBar()->addMenu("&View");
    m_viewMenu->addAction(m_zoomInAction);
    m_viewMenu->addAction(m_zoomOutAction);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_fitToWidthAction);
    m_viewMenu->addAction(m_fitToPageAction);
    m_viewMenu->addAction(m_actualSizeAction);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_goToPageAction);
    m_viewMenu->addAction(m_nextPageAction);
    m_viewMenu->addAction(m_previousPageAction);
    
    m_helpMenu = menuBar()->addMenu("&Help");
    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBars()
{
    m_mainToolBar = addToolBar("Main");
    m_mainToolBar->addAction(m_openAction);
    m_mainToolBar->addAction(m_closeAction);
    m_mainToolBar->addSeparator();
    m_mainToolBar->addAction(m_printAction);
    
    m_viewToolBar = addToolBar("View");
    m_viewToolBar->addAction(m_previousPageAction);
    m_viewToolBar->addAction(m_nextPageAction);
    m_viewToolBar->addSeparator();
    m_viewToolBar->addAction(m_zoomOutAction);
    m_viewToolBar->addAction(m_zoomInAction);
    m_viewToolBar->addSeparator();
    m_viewToolBar->addAction(m_fitToWidthAction);
    m_viewToolBar->addAction(m_fitToPageAction);
}

void MainWindow::createStatusBar()
{
    m_pageLabel = new QLabel("No document");
    m_zoomLabel = new QLabel("100%");
    m_progressBar = new QProgressBar;
    m_progressBar->setVisible(false);
    
    statusBar()->addWidget(m_pageLabel);
    statusBar()->addPermanentWidget(m_zoomLabel);
    statusBar()->addPermanentWidget(m_progressBar);
}

void MainWindow::createDockWidgets()
{
    m_thumbnailDock = new QDockWidget("Thumbnails", this);
    m_thumbnailWidget = new ThumbnailWidget(this);
    m_thumbnailDock->setWidget(m_thumbnailWidget);
    addDockWidget(Qt::LeftDockWidgetArea, m_thumbnailDock);
    
    // Connect thumbnail widget
    connect(m_thumbnailWidget, &ThumbnailWidget::pageRequested, 
            m_documentViewer, &DocumentViewer::goToPage);
}

void MainWindow::openDocument()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open Document", 
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        DocumentFactory::fileDialogFilter());
    
    if (fileName.isEmpty())
        return;
    
    m_progressBar->setVisible(true);
    m_progressBar->setRange(0, 0); // Indeterminate progress
    
    try {
        m_document = DocumentFactory::createReader(fileName);
        if (m_document && m_document->load(fileName)) {
            m_currentFile = fileName;
            m_documentViewer->setDocument(m_document.get());
            m_thumbnailWidget->setDocument(m_document.get());
            setWindowTitle(QString("Document Reader - %1").arg(QFileInfo(fileName).fileName()));
            updateActions();
            updateStatusBar();
        } else {
            QMessageBox::warning(this, "Error", "Failed to load document");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load document: %1").arg(e.what()));
    }
    
    m_progressBar->setVisible(false);
}

void MainWindow::closeDocument()
{
    m_document.reset();
    m_documentViewer->setDocument(nullptr);
    m_thumbnailWidget->setDocument(nullptr);
    m_currentFile.clear();
    setWindowTitle("Document Reader");
    updateActions();
    updateStatusBar();
}

void MainWindow::printDocument()
{
    if (m_document) {
        // TODO: Implement printing
        QMessageBox::information(this, "Print", "Printing functionality will be implemented soon");
    }
}

void MainWindow::zoomIn()
{
    m_documentViewer->zoomIn();
}

void MainWindow::zoomOut()
{
    m_documentViewer->zoomOut();
}

void MainWindow::fitToWidth()
{
    m_documentViewer->fitToWidth();
}

void MainWindow::fitToPage()
{
    m_documentViewer->fitToPage();
}

void MainWindow::actualSize()
{
    m_documentViewer->actualSize();
}

void MainWindow::goToPage()
{
    if (!m_document)
        return;
    
    bool ok;
    int page = QInputDialog::getInt(this, "Go to Page", "Page number:", 
                                   m_documentViewer->currentPage() + 1, 1, 
                                   m_document->pageCount(), 1, &ok);
    if (ok) {
        m_documentViewer->goToPage(page - 1); // Convert to 0-based
    }
}

void MainWindow::nextPage()
{
    m_documentViewer->nextPage();
}

void MainWindow::previousPage()
{
    m_documentViewer->previousPage();
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "About Document Reader",
        "Document Reader v1.0.0\n\n"
        "A modern document viewer built with Qt and C++23.\n"
        "Currently supports PDF documents with extensible architecture "
        "for future document formats.");
}

void MainWindow::updateActions()
{
    bool hasDocument = m_document != nullptr;
    
    m_closeAction->setEnabled(hasDocument);
    m_printAction->setEnabled(hasDocument);
    m_zoomInAction->setEnabled(hasDocument);
    m_zoomOutAction->setEnabled(hasDocument);
    m_fitToWidthAction->setEnabled(hasDocument);
    m_fitToPageAction->setEnabled(hasDocument);
    m_actualSizeAction->setEnabled(hasDocument);
    m_goToPageAction->setEnabled(hasDocument);
    m_nextPageAction->setEnabled(hasDocument);
    m_previousPageAction->setEnabled(hasDocument);
}

void MainWindow::updateStatusBar()
{
    if (m_document) {
        int currentPage = m_documentViewer->currentPage() + 1; // Convert to 1-based
        int totalPages = m_document->pageCount();
        m_pageLabel->setText(QString("Page %1 of %2").arg(currentPage).arg(totalPages));
        
        int zoom = static_cast<int>(m_documentViewer->zoomFactor() * 100);
        m_zoomLabel->setText(QString("%1%").arg(zoom));
    } else {
        m_pageLabel->setText("No document");
        m_zoomLabel->setText("100%");
    }
}
