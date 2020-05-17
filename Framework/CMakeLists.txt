include_directories(../.)
include_directories(../LaunchBox)

set(CMAKE_AUTOMOC ON)

find_package(Qt5QuickControls2)
find_package(Qt5 COMPONENTS Quick Core Network)

qt5_add_resources(RESOURCES LaunchBox.qrc)
message(xDDDDDDDDDDDDDD)
message( ${RESOURCES})
add_executable(helloworld
    main.cpp
    backend.cpp
    ${RESOURCES})

qt5_use_modules(helloworld Quick Core Network)

target_link_libraries(helloworld
Qt5::Widgets
Qt5::Gui
Qt5::Widgets
Qt5::Quick
Qt5::Qml
Qt5::QuickControls2
)