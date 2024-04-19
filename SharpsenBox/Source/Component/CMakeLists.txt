
qt_add_library(ComponentRegister STATIC
        ComponentRegister.cpp)

target_include_directories(ComponentRegister PUBLIC
        "h")

target_link_libraries(ComponentRegister
        Qt6::Widgets
        Qt6::Gui
        Qt6::Qml
        Qt6::Core)
