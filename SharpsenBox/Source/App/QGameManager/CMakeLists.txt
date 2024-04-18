
qt_add_library(GameManager STATIC
    GameManager.cpp
    GameUninstaller.cpp)

target_include_directories(GameManager PUBLIC
    "../QConfig/h"
    "../QLoadingBar/h"
    "../QDialog/h"
    "../../UpdateManager/h"
    "../../UpdateManager"
    "../../Component/h"
    "h")

target_link_libraries(GameManager
    UpdateManager
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core)
