project(kdeplasma-qstardict)
cmake_minimum_required(VERSION 3.5)

option(KF5 "Compile against KF5" ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})


if (KF5)
	message(STATUS "Compile against KF5")
	find_package(Qt5Core REQUIRED)
	find_package(Qt5Gui REQUIRED)
	find_package(KF5Plasma REQUIRED)
else(KF5)
	message(STATUS "Compile against KDE4")
	find_package(KDE4 REQUIRED)
	include (KDE4Defaults)
endif(KF5)
#include(MacroOptionalAddSubdirectory)


add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})


add_subdirectory(applet)
add_subdirectory(dataengine)
