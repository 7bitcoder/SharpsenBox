    
qt5_add_resources(
    RESOURCES 
    "../../Gui/AppUpdater.qrc")

add_executable(AppUpdater
    main.cpp
    ${RESOURCES})

if(WIN AND RELEASE)
    set_target_properties(AppUpdater PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

qt5_use_modules(AppUpdater 
    Quick 
    Core 
    Network)

target_link_libraries(AppUpdater
    AppUpdaterBackend
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::QuickControls2)

target_include_directories(AppUpdater PUBLIC 
    "../AppUpdater"
    "../InstalationManager")

set_target_properties(AppUpdater PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Updater")