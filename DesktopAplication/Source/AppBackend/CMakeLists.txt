
add_library(AppBackend STATIC
    AppBackend.cpp)

target_include_directories(AppBackend PUBLIC
    "../Dialog"
    "../InstalationManager"
    "../Config"
    "../LoadingBar"
    "../GameManager"
    "../Dialog/h"
    "../InstalationManager/h"
    "../ObjectRepository"
    "../Config/h"
    "../LoadingBar/h"
    "../GameManager/h"
    "h")

qt5_use_modules(AppBackend 
    Quick 
    Core 
    Network)

target_link_libraries(AppBackend
    Dialog
    Config
    LoadingBar
    GameManager
    InstalationManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)
