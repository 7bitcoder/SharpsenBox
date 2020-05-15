# CMakeList.txt: plik projektu CMake najwyższego poziomu, wykonaj konfigurację globalną
# i uwzględnij podprojekty w tym miejscu.
#
cmake_minimum_required (VERSION 3.8)

project ("LaunchBox")

set (CMAKE_PREFIX_PATH "D:\\DevelPrograms\\Qt\\5.14.2\\msvc2017" )

find_package(Qt5Widgets)

add_subdirectory(Framework)

# Uwzględnij podprojekty.
