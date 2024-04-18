qt_add_library( AppUpdaterManager STATIC
    "AppUpdaterManager.cpp" "h/IAppUpdaterManager.hpp")

target_include_directories(AppUpdaterManager PUBLIC
    "../../UpdateManager/h"
    "../../UpdateManager"
    "../../QConfig/h"
    "../../Component"
    "../../Component/h"
    "h")

target_link_libraries(AppUpdaterManager
    UpdateManager
    Qt6::Gui
    Qt6::Core)
