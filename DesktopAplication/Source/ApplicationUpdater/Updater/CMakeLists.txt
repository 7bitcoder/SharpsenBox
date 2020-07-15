add_library( Updater STATIC
    "Updater.cpp" )

target_include_directories(Updater PUBLIC
    "../../UpdateManager/h"
    "../../UpdateManager"
    "../../QConfig/h"
    "../../Component"
    "../../Component/h")

qt5_use_modules(Updater 
    Quick 
    Core)

target_link_libraries(Updater
    LoadingBar
    GameManager
    Dialog
    UpdateManager
    Config
    Qt5::Gui
    Qt5::Core)
