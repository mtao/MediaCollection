# Qt



INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include})
SET(MusicDriver_SOURCES
    src/tag.cpp
   )
SET(MusicDriver_HEADERS
    include/tag.h
   )

ADD_LIBRARY(musicDriver STATIC ${MusicDriver_SOURCES} ${MusicDriver_HEADERS})
TARGET_LINK_LIBRARIES(musicDriver tag)
QT5_USE_MODULES(musicDriver Sql)
