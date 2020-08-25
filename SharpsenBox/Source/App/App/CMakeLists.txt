
qt5_add_resources(
    RESOURCES 
    "../../../Gui/SharpsenBox.qrc")

add_executable(SharpsenBox
    main.cpp
    Backend.cpp
    ${RESOURCES})

message(${WIN} AND ${RELEASE} asfa ${64BIT})

if(WIN AND RELEASE)
    set_target_properties(SharpsenBox PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

qt5_use_modules(SharpsenBox 
    Quick 
    Core 
    Network)

target_link_libraries(SharpsenBox
    ComponentRegister
    Dialog
    GameManager
    LoadingBar
    Config
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::QuickControls2
    ${CURL_LIBRARY})

target_include_directories(SharpsenBox PUBLIC 
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
    "../QLoadingBar/h")

file( COPY  ${CMAKE_SOURCE_DIR}/orbit2.ico DESTINATION  ${CMAKE_BINARY_DIR} )

install(TARGETS SharpsenBox DESTINATION ${SharpsenBoxPath}/bin)