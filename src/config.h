#pragma once

// Application information
#define APP_NAME "Document Reader"
#define APP_VERSION "1.0.0"
#define APP_ORGANIZATION "DocumentReader"

// Build configuration
#ifndef CMAKE_BUILD_TYPE
#define CMAKE_BUILD_TYPE "Release"
#endif

// Feature flags
#define FEATURE_PDF_SUPPORT 1
#define FEATURE_TEXT_EXTRACTION 1
#define FEATURE_SEARCH 1

// Future feature flags (currently disabled)
#define FEATURE_DOCX_SUPPORT 0
#define FEATURE_ODT_SUPPORT 0
#define FEATURE_EPUB_SUPPORT 0
#define FEATURE_PLUGIN_SYSTEM 0

// Default settings
#define DEFAULT_ZOOM_FACTOR 1.0
#define DEFAULT_DPI 96.0
#define THUMBNAIL_DPI 36.0
#define MAX_ZOOM_FACTOR 10.0
#define MIN_ZOOM_FACTOR 0.1
