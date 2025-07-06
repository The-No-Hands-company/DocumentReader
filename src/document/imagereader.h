#pragma once

#include "documentreader.h"
#include <QPixmap>

/**
 * Image document reader implementation for common image formats.
 * Supports JPEG, PNG, BMP, GIF, and other Qt-supported formats.
 */
class ImageReader : public DocumentReader
{
public:
    ImageReader();
    ~ImageReader() override = default;
    
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
    
private:
    QString m_filePath;
    QPixmap m_image;
};
