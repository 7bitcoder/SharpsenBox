
qt5_add_resources(
    RESOURCES 
    "../../Gui/Pather.qrc")

add_executable(Patcher
    main.cpp
    ${RESOURCES})

if(WIN AND RELEASE)
    set_target_properties(Patcher PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

qt5_use_modules(Patcher 
    Quick 
    Core 
    Network)

target_link_libraries(Patcher
    AppBackend
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::QuickControls2
    ${CURL_LIBRARY})

target_include_directories(Patcher PUBLIC "../AppBackend" "../test")

install(TARGETS Patcher DESTINATION ${LaunchBoxPath}/bin)