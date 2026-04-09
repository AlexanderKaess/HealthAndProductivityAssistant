message("----------")
message("add ui files to sources variable")
set(UI_SOURCES
	MainWindow.ui
)

add_library(uilib ${UI_SOURCES})
target_include_directories(uilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(uilib PUBLIC USE_QT)
