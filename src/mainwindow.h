#pragma once

#include <QMainWindow>
#include <QObject>
#include <QLabel>
#include <QProgressBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QDockWidget>
#include <QSettings>
#include <QStringList>
#include <memory>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class DocumentViewer;
class ThumbnailWidget;
class DocumentReader;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openDocument();
    void closeDocument();
    void printDocument();
    void zoomIn();
    void zoomOut();
    void fitToWidth();
    void fitToPage();
    void actualSize();
    void goToPage();
    void nextPage();
    void previousPage();
    void showAbout();
    
    // Recent files
    void openRecentFile();
    void clearRecentFiles();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWidgets();
    void updateActions();
    void updateStatusBar();

    // Central widget
    DocumentViewer* m_documentViewer;
    
    // Dock widgets
    QDockWidget* m_thumbnailDock;
    ThumbnailWidget* m_thumbnailWidget;
    
    // Menus
    QMenu* m_fileMenu;
    QMenu* m_viewMenu;
    QMenu* m_helpMenu;
    
    // Actions
    QAction* m_openAction;
    QAction* m_closeAction;
    QAction* m_printAction;
    QAction* m_exitAction;
    
    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;
    QAction* m_fitToWidthAction;
    QAction* m_fitToPageAction;
    QAction* m_actualSizeAction;
    
    QAction* m_goToPageAction;
    QAction* m_nextPageAction;
    QAction* m_previousPageAction;
    
    QAction* m_aboutAction;
    
    // Recent files
    QMenu* m_recentFilesMenu;
    QAction* m_clearRecentAction;
    QAction* m_recentFileActions[10];  // Support up to 10 recent files
    
    // Toolbars
    QToolBar* m_mainToolBar;
    QToolBar* m_viewToolBar;
    
    // Status bar widgets
    QLabel* m_pageLabel;
    QLabel* m_zoomLabel;
    QProgressBar* m_progressBar;
    
    // Document
    std::unique_ptr<DocumentReader> m_document;
    QString m_currentFile;

    // Recent files
    QStringList m_recentFiles;
    QAction* m_recentFilesActions[5]; // Assuming a maximum of 5 recent files

    // Recent files management
    void updateRecentFilesMenu();
    void addToRecentFiles(const QString& fileName);
    QStringList getRecentFiles() const;
    void setRecentFiles(const QStringList& files);
    
    static const int MaxRecentFiles = 10;
};
