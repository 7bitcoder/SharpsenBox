cmake_minimum_required(VERSION 3.15.0)

set(SHARPSEN_BOX_LIBRARY SharpsenBox)

set(SHARPSEN_BOX_VERSION_MAJOR 1)
set(SHARPSEN_BOX_VERSION_MINOR 1)
set(SHARPSEN_BOX_VERSION_PATCH 0)

set(SHARPSEN_BOX_VERSION ${SHARPSEN_BOX_VERSION_MAJOR}.${SHARPSEN_BOX_VERSION_MINOR}.${SHARPSEN_BOX_VERSION_PATCH})

project(${SHARPSEN_BOX_LIBRARY} LANGUAGES CXX VERSION ${SHARPSEN_BOX_VERSION})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CmakeModules")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(Config)
include(CMakeDependentOption)
include(GNUInstallDirs)


find_package(Qt6 REQUIRED COMPONENTS
	Core
	Widgets
	Gui
	Quick
	Qml
	Test
	QuickControls2
	Network)
qt_standard_project_setup()

add_subdirectory(Source)

install(FILES Config/Updater/SharpsenBoxInfo.json DESTINATION ${UpdaterPath}/Config)
install(FILES Config/Updater/Games.json DESTINATION ${UpdaterPath}/Config)

install(FILES Config/SharpsenBox/SharpsenBoxInfo.json DESTINATION ${SharpsenBoxPath}/Config)
install(FILES Config/SharpsenBox/Games.json DESTINATION ${SharpsenBoxPath}/Config)

install(FILES Config/SharpsenBox/SharpsenBoxInfo.json DESTINATION ${SharpsenBoxPath}/..)
