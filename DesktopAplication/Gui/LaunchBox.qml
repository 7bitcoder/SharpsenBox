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

    property int tmp: 1

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
        source: sw ? "Aplication.qml" : "MainChecker.qml"
    }

    BottomBar {
        id: bottomBar
        len: 55
        visible: true
        progress: 50
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
    Timer {
        running: true
        interval: 3000
        onTriggered: contentLoader.sw = true
        Component.onCompleted: start()
    }
}
