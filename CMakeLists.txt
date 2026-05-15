cmake_minimum_required(VERSION 3.16)

project(HealthAndProductivityAssistant VERSION 0.1 LANGUAGES CXX)

message("--------------------")
message("source root directory: ${CMAKE_SOURCE_DIR}")
message("build directory: ${CMAKE_BINARY_DIR}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_APPLE_SET_BUNDLE_LOCALIZATIONS OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core LinguistTools)
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

set_target_properties(HealthAndProductivityAssistant PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_APPLE_SET_BUNDLE_LOCALIZATIONS OFF
    MACOSX_BUNDLE_INFO_PLIST_LOCALIZATIONS "de;en"
)

set(TS_FILES
    resources/translations/HealthAndProductivityAssistant_de.ts
    resources/translations/HealthAndProductivityAssistant_en.ts
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_lrelease(HealthAndProductivityAssistant
        TS_FILES ${TS_FILES}
        QM_FILES_OUTPUT_VARIABLE QM_FILES
    )

    qt_add_resources(HealthAndProductivityAssistant "translations"
        PREFIX  "/i18n"
        BASE    "${CMAKE_CURRENT_BINARY_DIR}"
        FILES   ${QM_FILES}
    )
else()
    qt5_add_translation(QM_FILES ${TS_FILES})
endif()

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

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HealthAndProductivityAssistant)
endif()

message("Source log4cxx.xml file: ${CMAKE_SOURCE_DIR}/log4cxx.xml")
message("Target log4cxx.xml file: ${CMAKE_BINARY_DIR}/HealthAndProductivityAssistant.app/Contents/MacOS/log4cxx.xml")
configure_file(
    ${CMAKE_SOURCE_DIR}/log4cxx.xml
    ${CMAKE_BINARY_DIR}/HealthAndProductivityAssistant.app/Contents/MacOS/log4cxx.xml
    COPYONLY
)
