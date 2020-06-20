
add_library(DirTree STATIC
    TreeItem.cpp
    TreeModel.cpp)

target_include_directories(DirTree PUBLIC
    "../AppBackend"
    "../Project")

qt5_use_modules(DirTree 
    Quick 
    Core 
    Network
    Widgets)

target_link_libraries(DirTree
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)