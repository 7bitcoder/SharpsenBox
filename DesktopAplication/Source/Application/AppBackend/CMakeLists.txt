
add_library(AppBackend STATIC
    Appbackend.cpp)

target_include_directories(AppBackend PUBLIC
    "../../QDialog"
    "../../InstalationManager"
    "../../QConfig"
    "../../QLoadingBar"
    "../../QGameManager"
    "../../QDialog/h"
    "../../InstalationManager/h"
    "../../QConfig/h"
    "../../QLoadingBar/h"
    "../../QGameManager/h"
    "../../GlobalBackend"
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
