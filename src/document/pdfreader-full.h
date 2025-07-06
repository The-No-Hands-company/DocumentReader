#pragma once

#include "documentreader.h"
#include <poppler-qt6.h>
#include <memory>

/**
 * PDF document reader implementation using Poppler library.
 * This class provides PDF-specific functionality while implementing
 * the DocumentReader interface.
 */
class PDFReader : public DocumentReader
{
public:
    PDFReader();
    ~PDFReader() override;
    
    // DocumentReader interface implementation
    bool load(const QString& filePath) override;
    void close() override;
    bool isLoaded() const override;
    int pageCount() const override;
    QPixmap renderPage(int pageIndex, double dpi = 72.0) const override;
    QSizeF pageSize(int pageIndex) const override;
    
    QString title() const override;
    QString author() const override;
    QString subject() const override;
    QString creator() const override;
    QString producer() const override;
    QString filePath() const override;
    
    bool supportsTextExtraction() const override;
    QString extractText(int pageIndex) const override;
    QList<int> searchText(const QString& searchText, bool caseSensitive = false) const override;
    
    // PDF-specific methods
    bool isEncrypted() const;
    bool unlock(const QString& password);
    QDateTime creationDate() const;
    QDateTime modificationDate() const;
    QString version() const;
    
private:
    std::unique_ptr<Poppler::Document> m_document;
    QString m_filePath;
    mutable QList<std::unique_ptr<Poppler::Page>> m_pageCache;
    
    Poppler::Page* getPage(int pageIndex) const;
    void clearPageCache();
};
