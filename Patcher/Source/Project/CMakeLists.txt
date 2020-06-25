find_package(libarchieve)
conan_basic_setup(TARGETS)

add_library(ProjectL STATIC
    "Project.cpp" "Project.hpp" "Packer.cpp" "Packer.hpp")

target_include_directories(ProjectL PUBLIC
    "../AppBackend"
    "../DirTree"
    "../Config")

qt5_use_modules(ProjectL 
    Quick 
    Core 
    Network)

target_link_libraries(ProjectL
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::QuickControls2
    CONAN_PKG::libarchive)