# CMakeList.txt: plik projektu CMake najwyższego poziomu, wykonaj konfigurację globalną
# i uwzględnij podprojekty w tym miejscu.
#
cmake_minimum_required (VERSION 3.8)

project ("LaunchBox")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (CMAKE_PREFIX_PATH "D:\\DevelPrograms\\Qt\\5.14.2\\msvc2017\\lib\\cmake\\Qt5" )
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui Quick Qml QuickControls2)
add_subdirectory(Framework)

# Uwzględnij podprojekty.
