
add_library(ComponentRegister STATIC
    ComponentRegister.cpp)

target_include_directories(ComponentRegister  PUBLIC
    "h")

qt5_use_modules(ComponentRegister  
    Quick 
    Core 
    Network)

target_link_libraries(ComponentRegister 
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)
