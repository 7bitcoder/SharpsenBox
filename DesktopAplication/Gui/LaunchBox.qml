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
    property int stage: 0

    // to gameChoser
    property int selectedGame: _Config.getDefaultGameId()
    property string info: ""

    property bool dialogTrigger: _Dialog.showDialog
    onDialogTriggerChanged: {
        if (dialogTrigger) {
            stage = 3
            info = _Dialog.info
        }
    }

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

    Loader {
        id: contentLoader
        anchors {
            top: topBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        property bool sw: false
        property int stage: window.stage

        onStageChanged: {
            switch (stage) {
            case 0:
                source = "Aplication.qml"
                break
            case 1:
                source = "InstallPopOut.qml"
                break
            case 2:
                source = "PopOutInfo.qml"
                break
            case 3:
                source = "PopOutDialog.qml"
                break
            }
        }

        source: "Aplication.qml"
        onSourceChanged: {
            animation.stop()
            animation.start()
        }

        NumberAnimation {
            id: animation
            target: contentLoader.item
            property: "opacity"
            from: 0
            to: 1
            duration: 200
            easing.type: Easing.OutCubic
        }
    }

    BottomBar {
        id: bottomBar
        len: 55
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
