    
qt6_add_resources(
    RESOURCES 
    "../../../Gui/AppUpdater.qrc")

qt_add_executable(Updater
    main.cpp
    Backend.cpp
    ${RESOURCES})

if(WIN AND RELEASE)
    set_target_properties(Updater PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

target_link_libraries(Updater PUBLIC
    ComponentRegister
    AppUpdaterManager
    Config
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::QuickControls2)

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
file( COPY  ${CMAKE_SOURCE_DIR}/orbit2.ico DESTINATION  "${CMAKE_BINARY_DIR}/Updater" )


install(TARGETS Updater DESTINATION ${UpdaterPath}/Updater)
