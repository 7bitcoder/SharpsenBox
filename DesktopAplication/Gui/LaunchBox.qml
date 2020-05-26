import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtWebEngine 1.0

Window {
    id: window
    visible: true
    width: 1000
    height: 500
    color: "#202020"
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint
           | Qt.WindowMaximizeButtonHint
    minimumWidth: 600
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

    LeftBar {
        color: "#101010"
        id: leftBar
        anchors {
            top: topBar.bottom
            left: parent.left
            bottom: parent.bottom
        }
        width: 180
    }

    Item {
        id: content
        anchors {
            top: topBar.bottom
            left: leftBar.right
            right: parent.right
            bottom: parent.bottom
        }

        GameChoser {
            id: gameChoser
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: 70
            color: "#292929"
            size: 150
        }

        AnimatedImage {
            anchors.centerIn: parent
            source: "content/sea.gif"
        }

        CustomButton {
            id: button
            anchors.left: parent.left
            anchors.top: gameChoser.bottom
        }
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
}
