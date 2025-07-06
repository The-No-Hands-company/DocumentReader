# Development Guide for├── resources/             # Application resources
    ├── resources.qrc      # Qt resource file
    └── *.svg              # Scalable vector icon filescument Reader

## Project Structure

```
documentreader/
├── CMakeLists.txt          # Main build configuration
├── README.md               # Project documentation
├── LICENSE                 # MIT license
├── .gitignore             # Git ignore rules
├── build.ps1              # Windows build script
├── src/                   # Source code
│   ├── main.cpp           # Application entry point
│   ├── config.h           # Build configuration
│   ├── mainwindow.h/cpp   # Main application window
│   ├── mainwindow.ui      # Qt Designer UI file
│   ├── document/          # Document handling classes
│   │   ├── documentreader.h/cpp    # Abstract base class
│   │   ├── pdfreader.h/cpp         # PDF implementation
│   │   └── documentfactory.h/cpp   # Factory pattern
│   └── widgets/           # Custom Qt widgets
│       ├── documentviewer.h/cpp    # Main document display
│       └── thumbnailwidget.h/cpp   # Thumbnail sidebar
└── resources/             # Application resources
    ├── resources.qrc      # Qt resource file
    └── *.png              # Icon files
```

## Design Patterns Used

### 1. Abstract Factory Pattern
- **DocumentReader**: Abstract base class defining the interface
- **PDFReader**: Concrete implementation for PDF files
- **DocumentFactory**: Creates appropriate readers based on file type

### 2. Observer Pattern
- Qt's signal-slot mechanism for loose coupling between components
- Document viewer emits signals when page/zoom changes
- Main window and thumbnail widget respond to these signals

### 3. Strategy Pattern
- Different zoom strategies (fit to width, fit to page, actual size)
- Extensible rendering strategies for different document types

## Key Components

### DocumentReader Interface
```cpp
class DocumentReader {
public:
    virtual bool load(const QString& filePath) = 0;
    virtual QPixmap renderPage(int pageIndex, double dpi = 72.0) const = 0;
    virtual int pageCount() const = 0;
    // ... other methods
};
```

### PDF Implementation
- Uses Poppler-Qt6 for PDF rendering
- Implements page caching for performance
- Supports text extraction and search

### GUI Components
- **DocumentViewer**: Main display area with zoom and pan
- **ThumbnailWidget**: Sidebar with page thumbnails
- **MainWindow**: Coordinates all components

## Adding New Document Formats

### Step 1: Create Reader Class
```cpp
class NewFormatReader : public DocumentReader {
public:
    bool load(const QString& filePath) override {
        // Implementation specific to new format
    }
    
    QPixmap renderPage(int pageIndex, double dpi = 72.0) const override {
        // Render page to QPixmap
    }
    
    // Implement all other virtual methods...
};
```

### Step 2: Update Factory
```cpp
// In documentfactory.cpp
std::unique_ptr<DocumentReader> DocumentFactory::createReader(const QString& filePath) {
    QString extension = getFileExtension(filePath).toLower();
    
    if (extension == "pdf") {
        return std::make_unique<PDFReader>();
    }
    if (extension == "newformat") {  // Add this
        return std::make_unique<NewFormatReader>();
    }
    
    return nullptr;
}
```

### Step 3: Update Supported Extensions
```cpp
QStringList DocumentFactory::supportedExtensions() {
    return QStringList() << "pdf" << "newformat";  // Add extension
}
```

## Building for Different Platforms

### Windows (MSVC)
```powershell
# Install dependencies with vcpkg
vcpkg install qt6[core,widgets,gui,printsupport] poppler[qt6]

# Build
./build.ps1 -BuildType Release
```

### Linux (GCC/Clang)
```bash
# Install system packages
sudo apt install cmake qt6-base-dev libpoppler-qt6-dev

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

### macOS (Clang)
```bash
# Install with Homebrew
brew install cmake qt6 poppler-qt6

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)
```

## Performance Considerations

### Page Caching
- PDFReader caches rendered pages for faster navigation
- Thumbnails use lower DPI for faster generation
- Memory usage scales with document size

### Rendering Optimization
- Render pages at appropriate DPI for zoom level
- Use Qt's image scaling for smooth zoom operations
- Background thumbnail generation for large documents

### Memory Management
- Smart pointers (std::unique_ptr) for automatic cleanup
- RAII pattern throughout the codebase
- Qt's parent-child memory management for widgets

## Debugging Tips

### Common Issues
1. **Poppler not found**: Ensure PKG_CONFIG_PATH includes poppler-qt6
2. **Qt6 missing**: Install qt6-base-dev and qt6-tools-dev
3. **C++23 errors**: Use GCC 11+, Clang 14+, or MSVC 2022+

### Debug Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

### Logging
```cpp
#include <QDebug>
qDebug() << "Debug message";
qWarning() << "Warning message";
```

## Code Style Guidelines

### C++ Style
- Use C++23 features where appropriate
- RAII and smart pointers for resource management
- const-correctness throughout
- Clear, descriptive variable names

### Qt Style
- Use Qt's naming conventions for slots/signals
- Prefer Qt containers over STL where appropriate
- Use Qt's meta-object system (Q_OBJECT, signals, slots)

### File Organization
- Header files (.h) contain class declarations
- Implementation files (.cpp) contain definitions
- One class per file pair
- Include guards or #pragma once in headers

## Testing Strategy

### Unit Tests (Future)
```cpp
// Example test structure
class PDFReaderTest : public QObject {
    Q_OBJECT
private slots:
    void testLoadValidPDF();
    void testLoadInvalidFile();
    void testPageRendering();
};
```

### Integration Tests
- Test complete document loading workflow
- Verify GUI responsiveness with large documents
- Cross-platform rendering consistency

## Future Enhancements

### Planned Features
1. **Text Search**: Highlight search results across pages
2. **Bookmarks**: Save and navigate to specific pages
3. **Print Support**: Direct printing with page layout options
4. **Plugin System**: Dynamic loading of document format plugins
5. **OCR Integration**: Text recognition for scanned documents

### Architecture Improvements
1. **Async Loading**: Background document loading
2. **Streaming**: Handle very large documents efficiently
3. **GPU Acceleration**: Use hardware acceleration for rendering
4. **Undo/Redo**: For future editing capabilities
