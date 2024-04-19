
qt_add_resources(
        RESOURCES
        "../../../Gui/SharpsenBox.qrc")

qt_add_executable(SharpsenBox
        main.cpp
        Backend.cpp
        ${RESOURCES})

message(${WIN} AND ${RELEASE} asfa ${64BIT})

if (WIN AND RELEASE)
    set_target_properties(SharpsenBox PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()

target_link_libraries(SharpsenBox PUBLIC
        ComponentRegister
        Dialog
        GameManager
        LoadingBar
        Config
        Qt6::Widgets
        Qt6::Gui
        Qt6::Core
        Qt6::Quick
        Qt6::Qml
        Qt6::QuickControls2
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

file(COPY ${CMAKE_SOURCE_DIR}/orbit2.ico DESTINATION ${CMAKE_BINARY_DIR})

qt_generate_deploy_app_script(
        TARGET SharpsenBox
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

install(TARGETS SharpsenBox DESTINATION ${SharpsenBoxPath}/bin)
