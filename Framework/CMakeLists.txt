include_directories(../.)


find_package(Qt5QuickControls2)
add_executable(helloworld
    main.cpp
    backend.cpp
 "backend.cpp")

target_link_libraries(helloworld
Qt5::Widgets
Qt5::Gui
Qt5::Widgets
Qt5::Quick
Qt5::Qml
Qt5::QuickControls2
#Qt5::Application
)