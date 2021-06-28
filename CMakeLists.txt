cmake_minimum_required(VERSION 3.5)

project(GenesVisual VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

find_package(HDF5 COMPONENTS CXX REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets OpenGL REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets OpenGL REQUIRED)
find_package(Qt5Charts)
find_package(Qt5OpenGL)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.hpp
        src/mainwindow.ui
        src/container/h5container.hpp
        src/container/h5container.cpp
        src/container/datasetcontainer.hpp
        src/container/datasetcontainer.cpp
        src/utils/colorpalette.hpp
        src/utils/colorpalette.cpp
        src/graph/graphprojection.hpp
        src/graph/graphprojection.cpp
        src/graph/graphspatial.hpp
        src/graph/graphspatial.cpp
        src/container/spatialcontainer.hpp
        src/container/spatialcontainer.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GenesVisual
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(GenesVisual SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(GenesVisual
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(GenesVisual PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt5::Charts Qt5::OpenGL ${HDF5_LIBRARIES})

set_target_properties(GenesVisual PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GenesVisual)
endif()
