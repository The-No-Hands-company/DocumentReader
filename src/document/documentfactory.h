#pragma once

#include "documentreader.h"
#include <QString>
#include <memory>

/**
 * Factory class for creating document readers based on file extension.
 * This class implements the Factory pattern to create appropriate
 * document readers for different file formats.
 */
class DocumentFactory
{
public:
    /**
     * Create a document reader for the specified file.
     * The type of reader is determined by the file extension.
     * 
     * @param filePath Path to the document file
     * @return Unique pointer to a DocumentReader, or nullptr if unsupported format
     */
    static std::unique_ptr<DocumentReader> createReader(const QString& filePath);
    
    /**
     * Check if the specified file format is supported.
     * 
     * @param filePath Path to the document file
     * @return true if the format is supported, false otherwise
     */
    static bool isFormatSupported(const QString& filePath);
    
    /**
     * Get a list of supported file extensions.
     * 
     * @return List of supported extensions (without the dot)
     */
    static QStringList supportedExtensions();
    
    /**
     * Get a filter string for file dialogs.
     * 
     * @return Filter string suitable for QFileDialog
     */
    static QString fileDialogFilter();

private:
    DocumentFactory() = default; // Static class, no instantiation
    
    static QString getFileExtension(const QString& filePath);
};
