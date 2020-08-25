add_library( AppUpdaterManager STATIC
    "AppUpdaterManager.cpp" "h/IAppUpdaterManager.hpp")

target_include_directories(AppUpdaterManager PUBLIC
    "../../UpdateManager/h"
    "../../UpdateManager"
    "../../QConfig/h"
    "../../Component"
    "../../Component/h"
    "h")

qt5_use_modules(AppUpdaterManager 
    Quick 
    Core)

target_link_libraries(AppUpdaterManager
    UpdateManager
    Qt5::Gui
    Qt5::Core)
