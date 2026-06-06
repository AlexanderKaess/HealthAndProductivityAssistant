message("----------")
message("add appController files to sources variable")

set(APPCONTROLLER_SOURCES
	AppController.h
	AppController.cpp
)

add_library(appcontrollerlib ${APPCONTROLLER_SOURCES})

target_include_directories(appcontrollerlib PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
)

target_link_libraries(appcontrollerlib PUBLIC
	corelib
	uilib
	Qt6::Widgets
	Qt6::Multimedia
	log4cxx
)
