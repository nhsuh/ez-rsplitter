# FindTesseract.cmake
find_path(Tesseract_INCLUDE_DIR NAMES tesseract/baseapi.h PATH_SUFFIXES tesseract)
find_library(Tesseract_LIBRARY NAMES tesseract)

if (Tesseract_INCLUDE_DIR AND Tesseract_LIBRARY)
    set(Tesseract_FOUND TRUE)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Tesseract DEFAULT_MSG Tesseract_LIBRARY Tesseract_INCLUDE_DIR)

if (Tesseract_FOUND)
    set(Tesseract_LIBRARIES ${Tesseract_LIBRARY})
    set(Tesseract_INCLUDE_DIRS ${Tesseract_INCLUDE_DIR})
endif()

mark_as_advanced(Tesseract_INCLUDE_DIR Tesseract_LIBRARY)