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

macro(add_mvc_test TEST_NAME TEST_FILE)
	add_executable(${TEST_NAME} ${TEST_FILE})
	target_include_directories(${TEST_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/core
		${CMAKE_SOURCE_DIR}/appController
		${GTEST_INCLUDE_DIRS}
	)
	target_link_libraries(${TEST_NAME} PRIVATE
		corelib
		appcontrollerlib
		Qt6::Widgets
		Qt6::Multimedia
		Qt6::Test
		GTest::GTest
		GTest::Main
		GTest::gmock
		GTest::gmock_main
		log4cxx
	)
	add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endmacro()

add_mvc_test(AppSettingsTest AppSettingsTest.cpp)
add_mvc_test(TimerDialogTest TimerDialogTest.cpp)


message("--------------------")
