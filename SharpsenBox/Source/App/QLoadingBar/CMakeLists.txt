
add_library(LoadingBar STATIC 
    LoadingBar.cpp)

target_include_directories(LoadingBar PUBLIC
    "../JsonParsers"
    "../AppUpdater"
    "../QDialog/h"
    "../QGameManager/h"
    "../../UpdateManager/h"
    "../../QConfig/h"
    "../../Component/h"
    "h")

qt5_use_modules(LoadingBar 
    Quick 
    Core 
    Network)

target_link_libraries(LoadingBar
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)