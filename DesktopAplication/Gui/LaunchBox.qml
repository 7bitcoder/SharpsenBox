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

        property int checking: 0
        property int downing: 1
        property int installing: 2
        property int pause: 3
        property int error: 4
        property int stopped: 5
        property int completed: 6

        property int state: completed

        onStateChanged: {
            if (state === checking) {
                loadBar.barColor = "white"
                loadBar.vis = true
                loadBar.animation = true
                //loadBar.progress = 100
            } else if (state === downing) {
                loadBar.barColor = "orange"
                loadBar.vis = false
                loadBar.animation = false
            } else if (state === installing) {
                loadBar.vis = true
                loadBar.animation = true
                loadBar.barColor = "orange"
            } else if (state === pause) {
                loadBar.animation = false
                loadBar.barColor = "yellow"
            } else if (state === error) {
                loadBar.animation = false
                loadBar.barColor = "red"
            } else if (state === completed) {
                loadBar.animation = false
                loadBar.vis = false
                loadBar.barColor = "green"
            }
        }

        CustomButton {
            id: button
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 30
            }
        }
        LoadingBar {
            id: loadBar
            visible: true
            progress: 1
            barColor: "green"
            animation: false
            vis: false
            anchors {
                verticalCenter: parent.verticalCenter
                left: button.right
                leftMargin: 10
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
