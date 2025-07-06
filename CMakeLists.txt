cmake_minimum_required(VERSION 3.21)
project(DocumentReader VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

# Find Qt6 - specify your Qt installation path
set(CMAKE_PREFIX_PATH "D:/Qt/6.9.1/msvc2022_64" ${CMAKE_PREFIX_PATH})
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui PrintSupport)

# Find vcpkg packages for PDF support
find_package(PkgConfig REQUIRED)
pkg_check_modules(POPPLER_QT6 REQUIRED IMPORTED_TARGET poppler-qt6)

message(STATUS "Found Qt6: ${Qt6_VERSION}")
message(STATUS "Qt6 Installation: ${Qt6_DIR}")

# Enable Qt's MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Source files (temporarily excluding PDFReader with Poppler)
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/document/documentreader.cpp
    src/document/documentreader.h
    src/document/pdfreader.cpp
    src/document/pdfreader.h
    src/document/imagereader.cpp
    src/document/imagereader.h
    src/document/documentfactory.cpp
    src/document/documentfactory.h
    src/widgets/documentviewer.cpp
    src/widgets/documentviewer.h
    src/widgets/thumbnailwidget.cpp
    src/widgets/thumbnailwidget.h
)

# UI files
set(UI_SOURCES
    src/mainwindow.ui
)

# Resource files
set(RESOURCES
    resources/resources.qrc
)

# Create the executable
add_executable(DocumentReader ${SOURCES} ${UI_SOURCES} ${RESOURCES})

# Link Qt libraries and Poppler
target_link_libraries(DocumentReader 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Gui 
    Qt6::PrintSupport
    PkgConfig::POPPLER_QT6
)

# Set output directory
set_target_properties(DocumentReader PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy Qt libraries for Windows deployment
if(WIN32)
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_WINDEPLOYQT_EXECUTABLE ${QT_QMAKE_EXECUTABLE} PATH)
    set(QT_WINDEPLOYQT_EXECUTABLE "${QT_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")
    if(EXISTS ${QT_WINDEPLOYQT_EXECUTABLE})
        add_custom_command(TARGET DocumentReader POST_BUILD
            COMMAND ${QT_WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:DocumentReader>
            COMMENT "Deploying Qt libraries")
    endif()
endif()
