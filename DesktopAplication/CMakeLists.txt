# CMakeList.txt: plik projektu CMake najwyższego poziomu, wykonaj konfigurację globalną
# i uwzględnij podprojekty w tym miejscu.
#

cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("asdadsas----------- ${CMAKE_TOOLCHAIN_FILE}")
project ("LaunchBox")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CmakeModules")
include(Config)


include(${CMAKE_SOURCE_DIR}/conanbuildinfo.cmake)

find_package(Qt5 REQUIRED COMPONENTS 
	Core 
	Widgets 
	Gui 
	Quick 
	Qml 
	QuickControls2 
	Network)

add_subdirectory(Source)