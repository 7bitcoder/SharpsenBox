# CMakeList.txt: plik projektu CMake najwyższego poziomu, wykonaj konfigurację globalną
# i uwzględnij podprojekty w tym miejscu.
#

cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("SharpsenBox")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CmakeModules")
include(Config)


include(${CMAKE_SOURCE_DIR}/conanbuildinfo.cmake)

find_package(Qt5 REQUIRED COMPONENTS 
	Core 
	Widgets 
	Gui 
	Quick 
	Qml
	Test
	QuickControls2 
	Network)

add_subdirectory(Source)

install(FILES Config/Updater/SharpsenBoxInfo.json DESTINATION ${UpdaterPath}/Config)
install(FILES Config/Updater/Games.json DESTINATION ${UpdaterPath}/Config)

install(FILES Config/SharpsenBox/SharpsenBoxInfo.json DESTINATION ${SharpsenBoxPath}/Config)
install(FILES Config/SharpsenBox/Games.json DESTINATION ${SharpsenBoxPath}/Config)

install(FILES Config/SharpsenBox/SharpsenBoxInfo.json DESTINATION ${SharpsenBoxPath}/..)