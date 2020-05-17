qt5_add_resources(
    RESOURCES 
    LaunchBox.qrc)

add_executable(LaunchBox
    main.cpp
    ${RESOURCES})

qt5_use_modules(LaunchBox 
    Quick 
    Core 
    Network)

target_link_libraries(LaunchBox
    Qt5::Widgets
    Qt5::Gui
    Qt5::Widgets
    Qt5::Quick
    Qt5::Qml
    Qt5::QuickControls2)