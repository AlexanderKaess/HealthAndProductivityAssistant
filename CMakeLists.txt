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

if(APPLE)
    set(CMAKE_PREFIX_PATH /Users/alexanderkaess/Qt/6.10.1/macos)
    set(Qt6_DIR /Users/alexanderkaess/Qt/6.10.1/macos/lib/cmake/Qt6)
    set(QT_APPLE_SET_BUNDLE_LOCALIZATIONS ON)
    set(PLATFORM_OUTPUT_DIR "${CMAKE_BINARY_DIR}/out/macos")

elseif(WIN32)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/msvc2022_64")
    set(Qt6_DIR "C:/Qt/6.10.1/msvc2022_64/lib/cmake/Qt6")
    set(PLATFORM_OUTPUT_DIR "${CMAKE_BINARY_DIR}/out/windows")
endif()

message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message("Qt6_DIR: ${Qt6_DIR}")
message("PLATFORM_OUTPUT_DIR: ${PLATFORM_OUTPUT_DIR}")

if(NOT Qt6_DIR AND NOT Qt5_DIR)
    message(FATAL_ERROR
        "Qt installation not found. Install Qt or set environment variable:
        macOS : -DCMAKE_PREFIX_PATH=/Users/alexanderkaess/Qt/6.10.1/macos
        Windows: -DCMAKE_PREFIX_PATH=C:/Qt/6.10.1/msvc2022_64")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core LinguistTools Multimedia)
message("QT_VERSION_MAJOR: ${QT_VERSION_MAJOR}")
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core LinguistTools Multimedia)
find_package(log4cxx REQUIRED)

add_subdirectory(appController/)
add_subdirectory(core/)
add_subdirectory(ui/)
add_subdirectory(unitTests/)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/translations.qrc.in
    ${CMAKE_BINARY_DIR}/translations.qrc
    @ONLY
)

qt_add_executable(HealthAndProductivityAssistant
    MANUAL_FINALIZATION
    main.cpp
    resources/resources.qrc
    ${CMAKE_BINARY_DIR}/translations.qrc
)

qt_add_translations(HealthAndProductivityAssistant
    TS_FILES
        resources/translations/HealthAndProductivityAssistant_de.ts
        resources/translations/HealthAndProductivityAssistant_en.ts
)

if(APPLE)
    if(${QT_VERSION} VERSION_LESS 6.1.0)
        set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER
            com.example.HealthAndProductivityAssistant)
    endif()

    configure_file(
        ${CMAKE_SOURCE_DIR}/resources/Info.plist.in
        ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
        @ONLY
    )

    set_target_properties(HealthAndProductivityAssistant PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE_INFO_PLIST_LOCALIZATIONS "de;en"
        QT_APPLE_SET_BUNDLE_LOCALIZATIONS TRUE
        WIN32_EXECUTABLE TRUE
        ${BUNDLE_ID_OPTION}
        RUNTIME_OUTPUT_DIRECTORY "${PLATFORM_OUTPUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${PLATFORM_OUTPUT_DIR}/lib"
        ARCHIVE_OUTPUT_DIRECTORY "${PLATFORM_OUTPUT_DIR}/lib"
    )

    message("Source log4cxx.xml: ${CMAKE_SOURCE_DIR}/log4cxx.xml")
    message("Target log4cxx.xml: ${PLATFORM_OUTPUT_DIR}/HealthAndProductivityAssistant.app/Contents/MacOS/log4cxx.xml")

    configure_file(
        ${CMAKE_SOURCE_DIR}/log4cxx.xml
        ${PLATFORM_OUTPUT_DIR}/HealthAndProductivityAssistant.app/Contents/MacOS/log4cxx.xml
        COPYONLY
    )
elseif(WIN32)
    if(EXISTS "${CMAKE_SOURCE_DIR}/resources/app.rc")
        target_sources(HealthAndProductivityAssistant PRIVATE
            resources/app.rc
        )
    endif()

    set_target_properties(HealthAndProductivityAssistant PROPERTIES
        WIN32_EXECUTABLE TRUE
        RUNTIME_OUTPUT_DIRECTORY "${PLATFORM_OUTPUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${PLATFORM_OUTPUT_DIR}/lib"
        ARCHIVE_OUTPUT_DIRECTORY "${PLATFORM_OUTPUT_DIR}/lib"
    )

    message("Source log4cxx.xml: ${CMAKE_SOURCE_DIR}/log4cxx.xml")
    message("Target log4cxx.xml: ${PLATFORM_OUTPUT_DIR}/log4cxx.xml")

    configure_file(
        ${CMAKE_SOURCE_DIR}/log4cxx.xml
        ${PLATFORM_OUTPUT_DIR}/log4cxx.xml
        COPYONLY
    )

    find_program(WINDEPLOYQT_EXECUTABLE windeployqt
        HINTS "${CMAKE_PREFIX_PATH}/bin"
    )

    if(WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET HealthAndProductivityAssistant POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --no-compiler-runtime
                --no-system-d3d-compiler
                --translations de,en
                "$<TARGET_FILE:HealthAndProductivityAssistant>"
            COMMENT "windeployqt: Qt DLLs copy to ${PLATFORM_OUTPUT_DIR}"
        )
    else()
        message(WARNING
            "windeployqt not found – Qt DLLs have to be manually copied to .exe")
    endif()

    if(MSVC)
        target_compile_options(HealthAndProductivityAssistant PRIVATE
            /W4
            /WX-
            /MP
            /utf-8
        )
        target_compile_definitions(HealthAndProductivityAssistant PRIVATE
            _UNICODE
            UNICODE
            WIN32_LEAN_AND_MEAN
            NOMINMAX
        )
    endif()

endif()

target_link_libraries(HealthAndProductivityAssistant PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Multimedia
    appcontrollerlib
    corelib
    uilib
    log4cxx
)

if(WIN32)
    target_link_libraries(HealthAndProductivityAssistant PRIVATE
        user32
        gdi32
        shell32
        ole32
        advapi32
    )
endif()

include(GNUInstallDirs)

if(APPLE)
    install(TARGETS HealthAndProductivityAssistant
        BUNDLE  DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
elseif(WIN32)
    install(TARGETS HealthAndProductivityAssistant
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    if(WINDEPLOYQT_EXECUTABLE)
        install(CODE "
            execute_process(
                COMMAND \"${WINDEPLOYQT_EXECUTABLE}\"
                    --no-compiler-runtime
                    --translations de,en
                    \"\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/HealthAndProductivityAssistant.exe\"
            )
        ")
    endif()
endif()
