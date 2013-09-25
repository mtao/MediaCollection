cmake_minimum_required(VERSION 2.8.9)
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

# Qt
SET(CMAKE_INCLUDE_CURRENT_DIR_ON)
SET(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt5Core)

ADD_SUBDIRECTORY(drivers)
ADD_DEFINITIONS(-DBOOST_ASIO_DYN_LINK)

# Source files
SET(MediaCollection_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/scanner.cpp
    src/rssmanager.cpp
    src/download_manager.cpp
    src/torrentmanager.cpp
    )
# Include files
SET(MediaCollection_HEADERS
    include/mainwindow.h
    include/scanner.h
    include/rssmanager.h
    include/download_manager.h
    include/torrentmanager.h
    )
SET(MediaCollection_SOURCES ${MediaCollection_SOURCES} ${MediaCollection_HEADERS})


# Executable to build
ADD_EXECUTABLE(MediaCollection
    ${MediaCollection_SOURCES}
    ${MediaCollection_HEADERS})
TARGET_LINK_LIBRARIES(MediaCollection musicDriver torrent_rb)
QT5_USE_MODULES(MediaCollection Widgets Gui Sql Network)

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/MediaCollection DESTINATION bin)
