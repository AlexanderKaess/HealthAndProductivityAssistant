message("----------")
message("add unit test files to sources variable")
cmake_minimum_required(VERSION 3.16)
project(HealthAndProductivityAssistant_Tests)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia Test)
find_package(GTest REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(TEST_SOURCES
	gmain.cpp
	AppControllerTest.cpp
	AppSettingsTest.cpp
	InactivityWatcherTest.cpp
	MainWindowTest.cpp
	Mocks.cpp
	Mocks.h
	SoundManagerTest.cpp
	ThemeManagerTest.cpp
	TimerDialogTest.cpp
)

qt_add_executable(HealthAndProductivityAssistant_Tests ${TEST_SOURCES})

target_link_libraries(HealthAndProductivityAssistant_Tests PRIVATE
		corelib
		appcontrollerlib
		Qt6::Widgets
		Qt6::Multimedia
		Qt6::Test
		GTest::GTest
		GTest::gmock
		GTest::gmock_main
		log4cxx
)

include(GoogleTest)
gtest_discover_tests(HealthAndProductivityAssistant_Tests)

message("--------------------")
