message("----------")
message("add core files to sources variable")
set(CORE_SOURCES
	MainWindow.cpp
	MainWindow.h
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
	log4cxx
)
