
qt5_add_resources(
    RESOURCES 
    "../../../Gui/LaunchBox.qrc")

add_executable(LaunchBox
    main.cpp
    Backend.cpp
    ${RESOURCES})

message(${WIN} AND ${RELEASE} asfa ${64BIT})

if(WIN AND RELEASE)
    #set_target_properties(LaunchBox PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

qt5_use_modules(LaunchBox 
    Quick 
    Core 
    Network)

target_link_libraries(LaunchBox
    ComponentRegister
    Dialog
    GameManager
    LoadingBar
    UpdateManager
    Config
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::QuickControls2
    ${CURL_LIBRARY})

target_include_directories(LaunchBox PUBLIC 
    "../../Component"
    "../../Component/h"
    "../../QConfig"
    "../../QConfig/h"
    "../../UpdateManager"
    "../../UpdateManager/h"
    "../QDialog"
    "../QDialog/h"
    "../QGameManager"
    "../QGameManager/h"
    "../QLoadingBar"
    "../QLoadingBar/h"
    
    
    )


install(TARGETS LaunchBox DESTINATION ${LaunchBoxPath}/bin)