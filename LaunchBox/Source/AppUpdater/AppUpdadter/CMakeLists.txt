    
qt5_add_resources(
    RESOURCES 
    "../../../Gui/AppUpdater.qrc")

add_executable(Updater
    main.cpp
    Backend.cpp
    ${RESOURCES})

if(WIN AND RELEASE)
    #set_target_properties(AppUpdater PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

qt5_use_modules(Updater 
    Quick 
    Core 
    Network)

target_link_libraries(Updater
    ComponentRegister
    AppUpdaterManager
    Config
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::QuickControls2)

target_include_directories(Updater PUBLIC 
    "../QAppUpdaterManager"
    "../QAppUpdaterManager/h"
    "../../UpdateManager"
    "../../UpdateManager/h"
    "../../QConfig"
    "../../QConfig/h"
    "../../Component"
    "../../Component/h")

set_target_properties(Updater PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Updater")


install(TARGETS Updater DESTINATION ${UpdaterPath}/Updater)
