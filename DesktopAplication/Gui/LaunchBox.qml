import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtWebEngine 1.0

Window {
    id: window
    visible: true
    width: 1200
    height: 500
    color: "#202020"
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint
           | Qt.WindowMaximizeButtonHint
    minimumWidth: 800
    minimumHeight: 600
    // for window movement
    property bool maximalized: false
    property point startMousePos
    property point startWindowPos
    property size startWindowSize

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
            bottom: bottomBar.top
        }
        width: 180
    }

    Item {
        id: rightBar
        anchors {
            top: topBar.bottom
            right: parent.right
            bottom: bottomBar.top
        }
        width: 50
    }

    Item {
        id: content
        anchors {
            top: topBar.bottom
            left: leftBar.right
            right: rightBar.left
            bottom: bottomBar.top
        }
        AnimatedImage {
            anchors.centerIn: parent
            id: animation
            source: "content/sea.gif"
        }
    }

    Rectangle {
        id: bottomBar
        color: "#252525"
        anchors {
            left: parent.left
            bottom: parent.bottom
            right: parent.right
        }
        height: 80
        CustomButton {
            id: button
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 30
            }
        }
        LoadingBar {
            visible: true
            anchors {
                verticalCenter: parent.verticalCenter
                left: button.right
                leftMargin: 30
                right: parent.right
                rightMargin: 30
            }
        }
    }

    ResizingFrames {
        anchors.fill: parent
        size: 3
    }
}
