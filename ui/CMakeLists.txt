message("----------")
message("add ui files to sources variable")
set(UI_SOURCES
	InactivityDialog.ui
	MainWindow.ui
	TimerDialog.ui
)

add_library(uilib ${UI_SOURCES})
target_include_directories(uilib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(uilib PUBLIC USE_QT)
