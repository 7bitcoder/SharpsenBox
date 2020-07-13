
add_library(GameManager STATIC
    GameManager.cpp
    GameUninstaller.cpp)

target_include_directories(GameManager PUBLIC
    "../AppBackend"
    "../Config/h"
    "../LoadingBar/h"
    "../InstalationManager/h"
    "../Dialog/h"
    "../JsonParsers"
    "../AppUpdater"
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