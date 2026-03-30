cmake_minimum_required(VERSION 3.16)

project(HealthAndProductivityAssistant VERSION 0.1 LANGUAGES CXX)

message(STATUS "Top level source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Top level build directory: ${CMAKE_BINARY_DIR}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(log4cxx REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
)
message(STATUS "Project sources: ${PROJECT_SOURCES}")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HealthAndProductivityAssistant
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(HealthAndProductivityAssistant SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(HealthAndProductivityAssistant
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(HealthAndProductivityAssistant PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    log4cxx
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.HealthAndProductivityAssistant)
endif()

set_target_properties(HealthAndProductivityAssistant PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS HealthAndProductivityAssistant
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HealthAndProductivityAssistant)
endif()

message(STATUS "Source log4cxx.xml file: ${CMAKE_SOURCE_DIR}/log4cxx.xml")
message(STATUS "Target log4cxx.xml file: ${CMAKE_BINARY_DIR}/HealthAndProductivityAssistant.app/Contents/MacOS/log4cxx.xml")
configure_file(
    ${CMAKE_SOURCE_DIR}/log4cxx.xml
    ${CMAKE_BINARY_DIR}/HealthAndProductivityAssistant.app/Contents/MacOS/log4cxx.xml
    COPYONLY
)
