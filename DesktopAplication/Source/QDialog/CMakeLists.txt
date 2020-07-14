
add_library(Dialog STATIC
    Dialog.cpp)

target_include_directories(Dialog PUBLIC
    "../QGameManager/h"
    "../Component"
    "../QConfig/h"
    "h")

qt5_use_modules(Dialog 
    Quick 
    Core 
    Network)

target_link_libraries(Dialog
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)