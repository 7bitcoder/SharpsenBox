
add_library(Button STATIC
    Button.cpp)

qt5_use_modules(Button 
    Quick 
    Core 
    Network)

target_link_libraries(Button
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)