message("----------")
message("add core files to sources variable")
set(CORE_SOURCES
	MainWindow.h
	MainWindow.cpp
	TimerDialog.h
	TimerDialog.cpp
	ThemeManager.h
	ThemeManager.cpp
)

add_library(corelib ${CORE_SOURCES}
	ThemeManager.h ThemeManager.cpp)
target_include_directories(corelib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
)

target_link_libraries(corelib PUBLIC
	Qt6::Widgets
	log4cxx
)
