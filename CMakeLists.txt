cmake_minimum_required(VERSION 3.16)

project(GBemulator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(inc/gui)
include_directories(inc/emulator)

file (GLOB HEADERS "${PROJECT_SOURCE_DIR}/inc/*/*")
file (GLOB SRC "${PROJECT_SOURCE_DIR}/src/*/*")

set(PROJECT_SOURCES
        src/main.cpp
        ${HEADERS}
        ${SRC}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GBemulator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(GBemulator
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(GBemulator PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

include(GNUInstallDirs)
install(TARGETS GBemulator
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GBemulator)
endif()
