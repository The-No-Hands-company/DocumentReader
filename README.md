# Document Reader

A modern, extensible document reader built with C++23 and Qt6. Currently supports PDF documents with an architecture designed for easy expansion to other document formats.

## Features

- **PDF Support**: Full PDF viewing capabilities using Poppler library
- **Modern UI**: Clean, dark-themed interface built with Qt6 and scalable SVG icons
- **Zoom Controls**: Zoom in/out, fit to width, fit to page, actual size
- **Navigation**: Page-by-page navigation with thumbnail sidebar
- **Extensible Architecture**: Designed to easily add support for new document formats
- **Cross-Platform**: Built with CMake for Windows, macOS, and Linux support

## Building

### Quick Setup (Windows)

**Option 1: Automatic Setup with vcpkg (Recommended)**
```powershell
# Run the setup script to install vcpkg and dependencies
.\setup-dependencies.ps1

# Build the project
.\build.ps1
```

**Option 2: Manual vcpkg Setup**
```powershell
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat

# Install dependencies (correct package names)
.\vcpkg install qt:x64-windows
.\vcpkg install poppler[qt]:x64-windows

# Set environment variable
$env:VCPKG_ROOT = "C:\vcpkg"

# Build
cd D:\dev\documentreader
.\build.ps1
```

### Prerequisites

- CMake 3.21 or higher
- Qt6 (Core, Widgets, Gui, PrintSupport)
- Poppler-Qt6 library
- C++23 compatible compiler (GCC 11+, Clang 14+, MSVC 2022+)

### Windows

1. Install Qt6 and Poppler-Qt6:
   ```powershell
   # Using vcpkg (recommended)
   vcpkg install qt:x64-windows
   vcpkg install poppler[qt]:x64-windows
   ```

2. Build the project:
   ```powershell
   mkdir build
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
   cmake --build . --config Release
   ```

### Linux

1. Install dependencies:
   ```bash
   # Ubuntu/Debian
   sudo apt install cmake qt6-base-dev qt6-tools-dev libpoppler-qt6-dev
   
   # Fedora
   sudo dnf install cmake qt6-qtbase-devel qt6-qttools-devel poppler-qt6-devel
   ```

2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make -j$(nproc)
   ```

### macOS

1. Install dependencies:
   ```bash
   brew install cmake qt6 poppler-qt6
   ```

2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make -j$(sysctl -n hw.ncpu)
   ```

## Usage

1. Launch the application
2. Use File → Open to load a PDF document
3. Navigate using:
   - Page Up/Page Down keys
   - Navigation buttons in toolbar
   - Thumbnail sidebar (click on any thumbnail)
4. Zoom using:
   - Ctrl + Mouse wheel
   - Zoom buttons in toolbar
   - View menu options

## Architecture

The project uses a modular, extensible architecture:

- **DocumentReader**: Abstract base class defining the document interface
- **PDFReader**: Poppler-based PDF implementation
- **DocumentFactory**: Factory pattern for creating appropriate readers
- **DocumentViewer**: Main viewing widget with zoom and navigation
- **ThumbnailWidget**: Sidebar with page thumbnails

### Adding New Document Formats

To add support for a new document format:

1. Create a new reader class inheriting from `DocumentReader`
2. Implement all virtual methods
3. Update `DocumentFactory` to recognize the new format
4. Add the new format to supported extensions

Example:
```cpp
class DocxReader : public DocumentReader {
    // Implement all DocumentReader methods
};

// In DocumentFactory::createReader()
if (extension == "docx") {
    return std::make_unique<DocxReader>();
}
```

## License

This project is licensed under the MIT License. See LICENSE file for details.

## UI Features

- **Professional Dark Theme**: Modern dark interface that's easy on the eyes
- **Scalable SVG Icons**: Vector-based icons that look crisp at any size and DPI
- **Responsive Layout**: Adaptable interface that works well on different screen sizes
- **Intuitive Navigation**: Familiar keyboard shortcuts and mouse interactions
- **Context Tooltips**: Helpful tooltips on all interface elements

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## Planned Features

- [ ] Text search and highlighting
- [ ] Bookmarks and navigation history
- [ ] Print support
- [ ] Export pages as images
- [ ] Document metadata viewer
- [ ] Support for additional formats:
  - [ ] Microsoft Word (.docx)
  - [ ] OpenDocument Text (.odt)
  - [ ] EPUB (.epub)
  - [ ] Plain text (.txt)
  - [ ] Markdown (.md)
- [ ] Plugin system for custom document formats
