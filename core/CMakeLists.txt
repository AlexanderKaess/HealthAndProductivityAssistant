message("----------")
message("add core files to sources variable")
set(CORE_SOURCES
	InactivityDialog.h
	InactivityDialog.cpp
	InactivityWatcher.h
	InactivityWatcher.cpp
	MainWindow.h
	MainWindow.cpp
	SoundManager.h
	SoundManager.cpp
	ThemeManager.h
	ThemeManager.cpp
	TimerDialog.h
	TimerDialog.cpp
)

add_library(corelib ${CORE_SOURCES})
target_include_directories(corelib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
)

target_link_libraries(corelib PUBLIC
	Qt6::Widgets
	Qt6::Multimedia
	log4cxx
)
