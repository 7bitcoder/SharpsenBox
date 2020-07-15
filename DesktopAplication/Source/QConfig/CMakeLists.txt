
add_library(Config STATIC
    Config.cpp)


target_include_directories(Config PUBLIC
    "../Component/h"
    "../UpdateManager/h"
    "h")

qt5_use_modules(Config 
    Quick 
    Core 
    Network
    Widgets)

target_link_libraries(Config
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Widgets)

