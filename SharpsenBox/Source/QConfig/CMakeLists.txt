
qt_add_library(Config STATIC
    Config.cpp)


target_include_directories(Config PUBLIC
    "../Component/h"
    "../UpdateManager/h"
    "h")

target_link_libraries(Config
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    Qt6::Widgets)
