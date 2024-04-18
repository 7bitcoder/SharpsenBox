
qt_add_library(LoadingBar STATIC
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

target_link_libraries(LoadingBar
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core)
