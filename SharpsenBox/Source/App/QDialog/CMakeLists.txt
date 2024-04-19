
qt_add_library(Dialog STATIC
        Dialog.cpp)

target_include_directories(Dialog PUBLIC
        "../../Component/h"
        "../QGameManager/h"
        "../QConfig/h"
        "h")

target_link_libraries(Dialog
        Qt6::Widgets
        Qt6::Gui
        Qt6::Qml
        Qt6::Core)
