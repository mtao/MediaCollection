cmake_minimum_required(VERSION 2.6)
PROJECT(MediaCollection)

SET(CMAKE_RUNTIME_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-Werror)

# Include dirs
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
# Autogen'd moc code appears in the BINARY_DIR I believe
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})


ADD_SUBDIRECTORY(drivers)

# Source files
SET(MediaCollection_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/scanner.cpp
    )
# Include files
SET(MediaCollection_HEADERS_TOMOC
    include/mainwindow.h
    include/scanner.h
    )
SET(MediaCollection_HEADERS
    ${MediaCollection_HEADERS_TOMOC}
    )
SET(MediaCollection_SOURCES ${MediaCollection_SOURCES} ${MediaCollection_HEADERS})

# Qt
SET(QT_QMAKE_EXECUTABLE qmake-qt4 qmake4 qmake)
FIND_PACKAGE(Qt4 REQUIRED)
SET(QT_USE_QTOPENGL TRUE)
QT4_WRAP_CPP(MediaCollection_HEADERS_MOC ${MediaCollection_HEADERS_TOMOC})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

# Executable to build
ADD_EXECUTABLE(MediaCollection
    ${MediaCollection_SOURCES}
    ${MediaCollection_HEADERS}
    ${MediaCollection_HEADERS_MOC})
TARGET_LINK_LIBRARIES(MediaCollection ${QT_LIBRARIES} musicDriver)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/MediaCollection DESTINATION bin)
