#include "imagereader.h"
#include <QFileInfo>
#include <QImageReader>

ImageReader::ImageReader()
{
}

bool ImageReader::load(const QString& filePath)
{
    close();
    
    m_image.load(filePath);
    if (m_image.isNull()) {
        return false;
    }
    
    m_filePath = filePath;
    return true;
}

void ImageReader::close()
{
    m_filePath.clear();
    m_image = QPixmap();
}

bool ImageReader::isLoaded() const
{
    return !m_image.isNull();
}

int ImageReader::pageCount() const
{
    return isLoaded() ? 1 : 0;
}

QPixmap ImageReader::renderPage(int pageIndex, double dpi) const
{
    if (!isLoaded() || pageIndex != 0) {
        return QPixmap();
    }
    
    if (dpi == 72.0) {
        return m_image;
    }
    
    // Scale based on DPI
    double scaleFactor = dpi / 72.0;
    return m_image.scaled(
        static_cast<int>(m_image.width() * scaleFactor),
        static_cast<int>(m_image.height() * scaleFactor),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );
}

QSizeF ImageReader::pageSize(int pageIndex) const
{
    if (!isLoaded() || pageIndex != 0) {
        return QSizeF();
    }
    
    return m_image.size();
}

QString ImageReader::title() const
{
    if (!isLoaded()) {
        return QString();
    }
    
    return QFileInfo(m_filePath).baseName();
}

QString ImageReader::author() const
{
    return QString(); // Images don't typically have author metadata
}

QString ImageReader::subject() const
{
    return QString();
}

QString ImageReader::creator() const
{
    return "Image Viewer";
}

QString ImageReader::producer() const
{
    return "DocumentReader";
}

QString ImageReader::filePath() const
{
    return m_filePath;
}

bool ImageReader::supportsTextExtraction() const
{
    return false; // Images don't contain extractable text
}

QString ImageReader::extractText(int pageIndex) const
{
    Q_UNUSED(pageIndex)
    return QString();
}

QList<int> ImageReader::searchText(const QString& searchText, bool caseSensitive) const
{
    Q_UNUSED(searchText)
    Q_UNUSED(caseSensitive)
    return QList<int>();
}
