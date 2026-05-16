cmake_minimum_required(VERSION 3.16)

project(HealthAndProductivityAssistant VERSION 1.0 LANGUAGES CXX)

message("--------------------")
message("source root directory: ${CMAKE_SOURCE_DIR}")
message("build directory: ${CMAKE_BINARY_DIR}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_APPLE_SET_BUNDLE_LOCALIZATIONS ON)
set(CMAKE_PREFIX_PATH /Users/alexanderkaess/Qt/6.10.1/macos)
set(Qt6_DIR "/Users/alexanderkaess/Qt/6.10.1/macos/lib/cmake/Qt6")

message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message("Qt6_DIR: ${Qt6_DIR}")

if(NOT Qt6_DIR AND NOT Qt5_DIR)
    message(FATAL_ERROR "
        Qt installation not found. Install Qt or set environment variable:
        -DCMAKE_PREFIX_PATH=/Users/alexanderkaess/Qt/6.10.1/macos")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core LinguistTools)
message("QT_VERSION_MAJOR: ${QT_VERSION_MAJOR}")
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core LinguistTools)
find_package(log4cxx REQUIRED)

add_subdirectory(appController/)
add_subdirectory(core/)
add_subdirectory(ui/)
add_subdirectory(unitTests/)

qt_add_executable(HealthAndProductivityAssistant
    MANUAL_FINALIZATION
    main.cpp
    resources/resources.qrc
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.HealthAndProductivityAssistant)
endif()

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/Info.plist.in
    ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
    @ONLY
)

set_target_properties(HealthAndProductivityAssistant PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_APPLE_SET_BUNDLE_LOCALIZATIONS TRUE
    MACOSX_BUNDLE_INFO_PLIST_LOCALIZATIONS "de;en"
)

qt_add_lrelease(HealthAndProductivityAssistant
    RESOURCE_PREFIX "/translations"
    SOURCES
        resources/translations/HealthAndProductivityAssistant_de.ts
        resources/translations/HealthAndProductivityAssistant_en.ts
)

qt_add_resources(HealthAndProductivityAssistant "translations"
    PREFIX  "/i18n"
    BASE    "${CMAKE_CURRENT_BINARY_DIR}"
    FILES   ${QM_FILES}
)


target_link_libraries(HealthAndProductivityAssistant PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    corelib
    uilib
    log4cxx
)

include(GNUInstallDirs)
install(TARGETS HealthAndProductivityAssistant
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

message("Source log4cxx.xml file: ${CMAKE_SOURCE_DIR}/log4cxx.xml")
message("Target log4cxx.xml file: ${CMAKE_BINARY_DIR}/HealthAndProductivityAssistant.app/Contents/MacOS/log4cxx.xml")
configure_file(
    ${CMAKE_SOURCE_DIR}/log4cxx.xml
    ${CMAKE_BINARY_DIR}/HealthAndProductivityAssistant.app/Contents/MacOS/log4cxx.xml
    COPYONLY
)

qt_finalize_executable(HealthAndProductivityAssistant)
