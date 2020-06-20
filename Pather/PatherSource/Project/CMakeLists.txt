
add_library(ProjectL STATIC
    "Project.cpp" "Project.hpp")

target_include_directories(ProjectL PUBLIC
    "../AppBackend"
    "../DirTree")

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
    Qt5::QuickControls2)