cmake_minimum_required(VERSION 4.0)

project(Pomodoro LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET 11.0)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt6 COMPONENTS Core Quick Multimedia MultimediaWidgets REQUIRED)

set (SOURCES
    Main.cpp
    Pie.h
    Pie.cpp
    PieSlice.h
    PieSlice.cpp
    qml.qrc)

if (ANDROID)
    add_library(Pomodoro SHARED ${SOURCES})
else()
    add_executable(Pomodoro MACOSX_BUNDLE ${SOURCES})
endif()

qt_import_qml_plugins(Pomodoro)

target_link_libraries(Pomodoro
    PRIVATE Qt6::Core Qt6::Quick Qt6::Multimedia Qt6::MultimediaWidgets)
