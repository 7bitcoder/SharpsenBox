
add_library(GameManager STATIC
    GameManager.cpp
    GameUninstaller.cpp)

target_include_directories(GameManager PUBLIC
    "../QConfig/h"
    "../QLoadingBar/h"
    "../InstalationManager/h"
    "../QDialog/h"
    "../Component"
    "../JsonParsers"
    "h")

qt5_use_modules(GameManager 
    Quick 
    Core 
    Network
    Widgets)

target_link_libraries(GameManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)