# Quick Start Guide

## Prerequisites

Before building Document Reader, ensure you have the following installed:

### Windows
1. **Visual Studio 2022** (Community, Professional, or Enterprise)
   - Include "Desktop development with C++" workload
   - Ensure C++23 support is enabled

2. **Qt6** - Choose one method:
   - **Option A: Qt Online Installer** (Recommended)
     - Download from https://www.qt.io/download-qt-installer
     - Install Qt 6.6 or later with MSVC 2022 compiler
   
   - **Option B: vcpkg** (For developers)
     ```powershell
     git clone https://github.com/Microsoft/vcpkg.git
     cd vcpkg
     .\bootstrap-vcpkg.bat
     .\vcpkg install qt6[core,widgets,gui,printsupport]:x64-windows
     ```

3. **Poppler-Qt6**
   ```powershell
   # Using vcpkg (recommended)
   .\vcpkg install poppler[qt6]:x64-windows
   ```

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install cmake build-essential
sudo apt install qt6-base-dev qt6-tools-dev
sudo apt install libpoppler-qt6-dev pkg-config
```

### Linux (Fedora/RHEL)
```bash
sudo dnf install cmake gcc-c++
sudo dnf install qt6-qtbase-devel qt6-qttools-devel
sudo dnf install poppler-qt6-devel pkgconf
```

### macOS
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake qt6 poppler-qt6
```

## Building

### Method 1: PowerShell Script (Windows, Recommended)
```powershell
# Clone the repository
git clone <repository-url> documentreader
cd documentreader

# Build release version
.\build.ps1

# Build debug version
.\build.ps1 -BuildType Debug

# Clean build
.\build.ps1 -Clean
```

### Method 2: Batch Script (Windows)
```cmd
rem Clone the repository
git clone <repository-url> documentreader
cd documentreader

rem Build release version
build.bat release

rem Build debug version
build.bat debug

rem Clean build
build.bat clean
```

### Method 3: Manual CMake (All Platforms)
```bash
# Clone the repository
git clone <repository-url> documentreader
cd documentreader

# Create build directory
mkdir build
cd build

# Configure (adjust generator as needed)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --parallel

# On Windows with vcpkg
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Method 4: VS Code (Recommended for Development)
1. Open the project folder in VS Code
2. Install the C/C++ and CMake Tools extensions
3. Press `Ctrl+Shift+P` and run "CMake: Configure"
4. Press `Ctrl+Shift+P` and run "CMake: Build"
5. Or use the provided tasks: `Ctrl+Shift+P` → "Tasks: Run Task" → "build-release"

## Running

### Windows
```powershell
# After successful build
.\build\bin\Release\DocumentReader.exe

# Or double-click the executable in File Explorer
```

### Linux/macOS
```bash
# After successful build
./build/bin/DocumentReader
```

## First Use

1. **Launch the application**
2. **Open a PDF document**:
   - Click "File" → "Open" (or press Ctrl+O)
   - Select a PDF file from your computer
3. **Navigate the document**:
   - Use the thumbnail sidebar to jump to pages
   - Use Page Up/Page Down keys or toolbar buttons
   - Mouse wheel for scrolling, Ctrl+Mouse wheel for zooming
4. **Zoom controls**:
   - Toolbar buttons for zoom in/out
   - "View" menu for fit-to-width, fit-to-page options
   - Drag with left mouse button to pan around zoomed documents

## Troubleshooting

### Build Errors

**"Qt6 not found"**
- Ensure Qt6 is installed and in PATH
- On Windows, check that Qt6_DIR is set or use the Qt installer's environment

**"Poppler not found"**
- Install poppler-qt6-dev (Linux) or poppler[qt6] (vcpkg)
- Ensure pkg-config can find poppler-qt6

**"C++23 not supported"**
- Update to GCC 11+, Clang 14+, or MSVC 2022+
- Check compiler version: `gcc --version` or `clang --version`

### Runtime Errors

**"Application failed to start"**
- On Windows, ensure Qt6 DLLs are in PATH or in the application directory
- The windeployqt tool should handle this automatically

**"Cannot load PDF"**
- Check that the file is a valid PDF
- Ensure file permissions allow reading
- Try with a different PDF file

**"Thumbnails not generating"**
- Large PDFs may take time to generate thumbnails
- Check the console for error messages

### Performance Issues

**Slow loading**
- Large PDFs (>100 pages) may take time to load
- Thumbnails are generated in the background

**High memory usage**
- Each loaded page is cached for performance
- Consider closing and reopening for very large documents

## Getting Help

1. **Check the README.md** for detailed information
2. **Read DEVELOPMENT.md** for architecture details
3. **Open an issue** on the project repository
4. **Check the Qt documentation** for Qt-specific questions

## Next Steps

- Explore the **View menu** for different zoom options
- Try the **keyboard shortcuts** (listed in tooltips)
- Check out the **development guide** if you want to contribute
- **Report bugs** or **request features** via the issue tracker

## Example Workflow

```powershell
# Quick development setup (Windows with vcpkg)
git clone <repository-url> documentreader
cd documentreader

# Install dependencies (one-time setup)
vcpkg install qt6[core,widgets,gui,printsupport]:x64-windows
vcpkg install poppler[qt6]:x64-windows

# Build and run
.\build.ps1
.\build\bin\Release\DocumentReader.exe
```

Happy document reading! 📖
