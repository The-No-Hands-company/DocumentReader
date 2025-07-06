#pragma once

#include <QString>
#include <QPixmap>
#include <QSizeF>
#include <memory>

/**
 * Abstract base class for document readers.
 * This class defines the interface that all document readers must implement.
 * It's designed to be extensible for various document formats.
 */
class DocumentReader
{
public:
    virtual ~DocumentReader() = default;
    
    /**
     * Load a document from the specified file path.
     * @param filePath Path to the document file
     * @return true if successfully loaded, false otherwise
     */
    virtual bool load(const QString& filePath) = 0;
    
    /**
     * Close the currently loaded document.
     */
    virtual void close() = 0;
    
    /**
     * Check if a document is currently loaded.
     * @return true if document is loaded, false otherwise
     */
    virtual bool isLoaded() const = 0;
    
    /**
     * Get the number of pages in the document.
     * @return Number of pages, or 0 if no document is loaded
     */
    virtual int pageCount() const = 0;
    
    /**
     * Render a specific page as a QPixmap.
     * @param pageIndex 0-based page index
     * @param dpi Resolution for rendering (default: 72)
     * @return Rendered page as QPixmap, or null pixmap if page doesn't exist
     */
    virtual QPixmap renderPage(int pageIndex, double dpi = 72.0) const = 0;
    
    /**
     * Get the size of a specific page in points.
     * @param pageIndex 0-based page index
     * @return Page size in points, or empty size if page doesn't exist
     */
    virtual QSizeF pageSize(int pageIndex) const = 0;
    
    /**
     * Get the document's metadata.
     * @return Document metadata (title, author, etc.)
     */
    virtual QString title() const = 0;
    virtual QString author() const = 0;
    virtual QString subject() const = 0;
    virtual QString creator() const = 0;
    virtual QString producer() const = 0;
    
    /**
     * Get the file path of the currently loaded document.
     * @return File path, or empty string if no document is loaded
     */
    virtual QString filePath() const = 0;
    
    /**
     * Check if the document supports text extraction.
     * @return true if text extraction is supported
     */
    virtual bool supportsTextExtraction() const = 0;
    
    /**
     * Extract text from a specific page.
     * @param pageIndex 0-based page index
     * @return Extracted text, or empty string if not supported or page doesn't exist
     */
    virtual QString extractText(int pageIndex) const = 0;
    
    /**
     * Search for text in the document.
     * @param searchText Text to search for
     * @param caseSensitive Whether search should be case sensitive
     * @return List of page indices where text was found
     */
    virtual QList<int> searchText(const QString& searchText, bool caseSensitive = false) const = 0;
};
