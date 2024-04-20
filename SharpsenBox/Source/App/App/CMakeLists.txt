
qt_add_executable(SharpsenBox
        main.cpp
        Backend.cpp)

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

qt_add_qml_module(SharpsenBox
        URI gui
        QML_FILES
        ../../../Gui/SharpsenBox.qml
        ../../../Gui/CustomButton.qml
        ../../../Gui/TopBar.qml
        ../../../Gui/Home.qml
        ../../../Gui/Settings.qml
        ../../../Gui/Aplication.qml
        ../../../Gui/GameChoser.qml
        ../../../Gui/GameChoserBlock.qml
        ../../../Gui/BottomBar.qml
        ../../../Gui/InstallPopOut.qml
        ../../../Gui/PopOutDialog.qml
        ../../../Gui/PopOutInfo.qml
        ../../../Gui/GameSettings.qml
        ../../../Gui/ResizingFrames.qml
        ../../../Gui/LeftBar.qml
        ../../../Gui/WebGamePage.qml

        RESOURCES
        ../../../Gui/Icons/person.png
        ../../../Gui/Icons/cog.png
        ../../../Gui/Icons/house.png
        ../../../Gui/Icons/loader.png
        ../../../Gui/Icons/plain-square.png
        ../../../Gui/Icons/pause-button.png
        ../../../Gui/Icons/play-button.png
        ../../../Gui/Font/Lato-Black.ttf
        ../../../Gui/Font/Lato-BlackItalic.ttf
        ../../../Gui/Font/Lato-Bold.ttf
        ../../../Gui/Font/Lato-BoldItalic.ttf
        ../../../Gui/Font/Lato-Italic.ttf
        ../../../Gui/Font/Lato-Light.ttf
        ../../../Gui/Font/Lato-LightItalic.ttf
        ../../../Gui/Font/Lato-Regular.ttf
        ../../../Gui/Font/Lato-Thin.ttf
        ../../../Gui/Font/Lato-ThinItalic.ttf
        ../../../Gui/clear.html
)

set_target_properties(SharpsenBox PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)


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

#if (AUTO_QT_LIBS)
#    add_custom_target(deploy ALL
#            WORKING_DIRECTORY ${QtLib}/bin
#            COMMAND ${platform}deployqt --qmldir ${CMAKE_SOURCE_DIR}/Gui --${type} --quick ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#            COMMAND ${platform}deployqt --qmldir ${CMAKE_SOURCE_DIR}/Gui --${type} --quick ${UpdaterPath}
#            COMMENT "========= Deploying Aplication ========="
#            USES_TERMINAL)
#    add_dependencies(deploy SharpsenBox)
#endif ()

qt_generate_deploy_qml_app_script(
        TARGET SharpsenBox
        OUTPUT_SCRIPT deploy_script_qml
)

add_custom_command(TARGET SharpsenBox
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -DINCLUDE=${QT_DEPLOY_SUPPORT}
        -DTARGET_DIR=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
        -DQML_DIR=${CMAKE_SOURCE_DIR}/Gui
        -P ${deploy_script_qml}
        COMMENT "Copying qml dependencies into application"
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

install(SCRIPT ${deploy_script_qml})

install(TARGETS SharpsenBox DESTINATION ${SharpsenBoxPath}/bin)
