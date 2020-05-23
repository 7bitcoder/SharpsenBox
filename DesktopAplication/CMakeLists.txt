# CMakeList.txt: plik projektu CMake najwyższego poziomu, wykonaj konfigurację globalną
# i uwzględnij podprojekty w tym miejscu.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("LaunchBox")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CmakeModules")
include(Config)

if(AUTO_QT_LIBS)
	add_custom_target(deploy ALL
		WORKING_DIRECTORY ${CMAKE_PREFIX_PATH}/../../../bin 
		COMMAND ${platform}deployqt --qmldir ${CMAKE_SOURCE_DIR}/Gui --${type} --quick ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMENT "========= Deploying Aplication =========" 
		USES_TERMINAL)
		add_dependencies(deploy LaunchBox)
endif()

find_package(Qt5 REQUIRED COMPONENTS 
	Core 
	Widgets 
	Gui 
	Quick 
	Qml 
	QuickControls2 
	Network)

add_subdirectory(Source)


