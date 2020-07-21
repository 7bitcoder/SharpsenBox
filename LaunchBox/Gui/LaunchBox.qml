import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Window 2.12
import QtWebEngine 1.8

ApplicationWindow {
    id: window
    visible: true
    width: 1000
    height: 500
    color: "#202020"
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint
           | Qt.WindowMaximizeButtonHint
    minimumWidth: 1000
    minimumHeight: 600
    // for window movement
    property bool maximalized: false
    property point startMousePos
    property point startWindowPos
    property size startWindowSize
    property int dialogStage: _Dialog.dialogType

    // to gameChoser
    property int selectedGame: _Config.getDefaultGameId()

    function absoluteMousePos(mouseArea) {
        var windowAbs = mouseArea.mapToItem(null, mouseArea.mouseX,
                                            mouseArea.mouseY)
        return Qt.point(windowAbs.x + window.x, windowAbs.y + window.y)
    }

    TopBar {
        id: topBar
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: 25
        color: "black"
        smooth: true
    }

    Aplication {
        id: app
        anchors {
            top: topBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }


    Loader {
        id: popInfo
        anchors {
            top: topBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        property bool sw: false
        property int stage: window.dialogStage

        onStageChanged: {
            switch (stage) {
            case 0:
                if(source != ""){
                    hide()
                } else {
                    active = false
                    source = ""
                }
                break
            case 1:
                active = true
                source = "PopOutInfo.qml"
                show()
                break
            case 2:
                active = true
                source = "InstallPopOut.qml"
                show()
                break
            case 3:
                active = true
                source = "PopOutDialog.qml"
                show()
                break
            }
        }
        active: false
        source: ""
        function show(){
            showAnim.stop()
            showAnim.start()
        }
        function hide(){
            hideAnim.stop()
            hideAnim.start()
        }

        NumberAnimation {
            id: showAnim
            target: popInfo.item
            property: "opacity"
            from: 0
            to: 1
            duration: 100
            easing.type: Easing.OutCubic
        }
        NumberAnimation {
            id: hideAnim
            target: popInfo.item
            property: "opacity"
            from: 1
            to: 0
            duration: 100
            easing.type: Easing.OutCubic
            onRunningChanged: {
                if (!hideAnim.running) { // onEnd
                    popInfo.source = ""
                }
            }
        }
    }
    BottomBar {
        id: bottomBar
        len: 60
        visible: true
        //progress: 50
        anchors {
            left: parent.left
            bottom: parent.bottom
            right: parent.right
        }
    }

    ResizingFrames {
        anchors.fill: parent
        size: 3
    }
}
