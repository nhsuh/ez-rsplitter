cmake_minimum_required(VERSION 3.26)
project(ez_splitter)


set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/tesseract/5.3.3")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")


add_executable(ez_splitter main.cpp
        libs/receipt.cpp
        libs/partition.cpp
        libs/partition.h
        libs/calculator.cpp
        libs/calculator.h)
find_package(OpenCV REQUIRED)
find_package(Tesseract REQUIRED)
find_package(Leptonica CONFIG REQUIRED)

include_directories(${Tesseract_INCLUDE_DIRS})
include_directories(${Leptonica_INCLUDE_DIRS})

target_include_directories(ez_splitter PRIVATE libs)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(ez_splitter ${OpenCV_LIBS})
target_link_libraries(ez_splitter ${Tesseract_LIBRARIES})
target_link_libraries(ez_splitter ${Leptonica_LIBRARIES})