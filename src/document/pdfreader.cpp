#include "pdfreader.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <poppler-qt6.h>

PDFReader::PDFReader()
    : m_document(nullptr)
{
}

PDFReader::~PDFReader()
{
    close();
}

bool PDFReader::load(const QString& filePath)
{
    close();
    
    if (!QFile::exists(filePath)) {
        qWarning() << "File does not exist:" << filePath;
        return false;
    }
    
    m_document = std::unique_ptr<Poppler::Document>(
        Poppler::Document::load(filePath)
    );
    
    if (!m_document) {
        qWarning() << "Failed to load PDF document:" << filePath;
        return false;
    }
    
    if (m_document->isLocked()) {
        qWarning() << "PDF document is locked:" << filePath;
        return false;
    }
    
    // Set rendering hints for better quality
    m_document->setRenderHint(Poppler::Document::Antialiasing);
    m_document->setRenderHint(Poppler::Document::TextAntialiasing);
    
    m_filePath = filePath;
    
    return true;
}

void PDFReader::close()
{
    m_document.reset();
    m_filePath.clear();
}

bool PDFReader::isLoaded() const
{
    return m_document != nullptr;
}

int PDFReader::pageCount() const
{
    if (!m_document) {
        return 0;
    }
    return m_document->numPages();
}

QPixmap PDFReader::renderPage(int pageIndex, double dpi) const
{
    if (!m_document || pageIndex < 0 || pageIndex >= m_document->numPages()) {
        return QPixmap();
    }
    
    std::unique_ptr<Poppler::Page> page(m_document->page(pageIndex));
    if (!page) {
        return QPixmap();
    }
    
    QImage image = page->renderToImage(dpi, dpi);
    if (image.isNull()) {
        qWarning() << "Failed to render page" << pageIndex;
        return QPixmap();
    }
    
    return QPixmap::fromImage(image);
}

QSizeF PDFReader::pageSize(int pageIndex) const
{
    if (!m_document || pageIndex < 0 || pageIndex >= m_document->numPages()) {
        return QSizeF();
    }
    
    std::unique_ptr<Poppler::Page> page(m_document->page(pageIndex));
    if (!page) {
        return QSizeF();
    }
    
    return page->pageSizeF();
}

QString PDFReader::title() const
{
    if (!m_document) {
        return QString();
    }
    return m_document->info("Title");
}

QString PDFReader::author() const
{
    if (!m_document) {
        return QString();
    }
    return m_document->info("Author");
}

QString PDFReader::subject() const
{
    if (!m_document) {
        return QString();
    }
    return m_document->info("Subject");
}

QString PDFReader::creator() const
{
    if (!m_document) {
        return QString();
    }
    return m_document->info("Creator");
}

QString PDFReader::producer() const
{
    if (!m_document) {
        return QString();
    }
    return m_document->info("Producer");
}

QString PDFReader::filePath() const
{
    return m_filePath;
}

bool PDFReader::supportsTextExtraction() const
{
    return true;
}

QString PDFReader::extractText(int pageIndex) const
{
    if (!m_document || pageIndex < 0 || pageIndex >= m_document->numPages()) {
        return QString();
    }
    
    std::unique_ptr<Poppler::Page> page(m_document->page(pageIndex));
    if (!page) {
        return QString();
    }
    
    return page->text(QRectF());
}

QList<int> PDFReader::searchText(const QString& searchText, bool caseSensitive) const
{
    QList<int> results;
    
    if (!m_document || searchText.isEmpty()) {
        return results;
    }
    
    for (int i = 0; i < m_document->numPages(); ++i) {
        QString pageText = extractText(i);
        if (!pageText.isEmpty()) {
            if (caseSensitive) {
                if (pageText.contains(searchText, Qt::CaseSensitive)) {
                    results.append(i);
                }
            } else {
                if (pageText.contains(searchText, Qt::CaseInsensitive)) {
                    results.append(i);
                }
            }
        }
    }
    
    return results;
}

bool PDFReader::isEncrypted() const
{
    if (!m_document) {
        return false;
    }
    return m_document->isEncrypted();
}

bool PDFReader::unlock(const QString& password)
{
    if (!m_document) {
        return false;
    }
    return m_document->unlock(password.toUtf8(), password.toUtf8());
}

QDateTime PDFReader::creationDate() const
{
    if (!m_document) {
        return QDateTime();
    }
    return m_document->date("CreationDate");
}

QDateTime PDFReader::modificationDate() const
{
    if (!m_document) {
        return QDateTime();
    }
    return m_document->date("ModDate");
}

QString PDFReader::version() const
{
    return QString("1.7");
}
