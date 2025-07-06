#include "documentfactory.h"
#include "pdfreader.h"
#include "imagereader.h"
#include <QFileInfo>
#include <QStringList>

std::unique_ptr<DocumentReader> DocumentFactory::createReader(const QString& filePath)
{
    QString extension = getFileExtension(filePath).toLower();
    
    if (extension == "pdf") {
        return std::make_unique<PDFReader>();
    }
    
    // Image formats
    if (extension == "jpg" || extension == "jpeg" || 
        extension == "png" || extension == "bmp" || 
        extension == "gif" || extension == "tiff" || 
        extension == "tif" || extension == "svg" ||
        extension == "webp") {
        return std::make_unique<ImageReader>();
    }
    
    // Add support for other formats here in the future
    // if (extension == "docx") {
    //     return std::make_unique<DocxReader>();
    // }
    // if (extension == "odt") {
    //     return std::make_unique<OdtReader>();
    // }
    
    return nullptr; // Unsupported format
}

bool DocumentFactory::isFormatSupported(const QString& filePath)
{
    QString extension = getFileExtension(filePath).toLower();
    QStringList supported = supportedExtensions();
    return supported.contains(extension);
}

QStringList DocumentFactory::supportedExtensions()
{
    return QStringList() << "pdf" 
                        << "jpg" << "jpeg" << "png" << "bmp" 
                        << "gif" << "tiff" << "tif" << "svg" << "webp";
    
    // Future extensions will be added here:
    // return QStringList() << "pdf" << "docx" << "odt" << "epub" << "txt";
}

QString DocumentFactory::fileDialogFilter()
{
    QStringList filters;
    
    // Add specific format filters
    filters << "PDF Documents (*.pdf)";
    filters << "Image Files (*.jpg *.jpeg *.png *.bmp *.gif *.tiff *.tif *.svg *.webp)";
    
    // Future formats:
    // filters << "Word Documents (*.docx)";
    // filters << "OpenDocument Text (*.odt)";
    // filters << "EPUB Books (*.epub)";
    // filters << "Text Files (*.txt)";
    
    // Add "All Supported" filter
    QStringList extensions;
    for (const QString& ext : supportedExtensions()) {
        extensions << QString("*.%1").arg(ext);
    }
    
    QString allSupportedFilter = QString("All Supported Documents (%1)").arg(extensions.join(" "));
    filters.prepend(allSupportedFilter);
    
    // Add "All Files" filter
    filters << "All Files (*)";
    
    return filters.join(";;");
}

QString DocumentFactory::getFileExtension(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    return fileInfo.suffix();
}
