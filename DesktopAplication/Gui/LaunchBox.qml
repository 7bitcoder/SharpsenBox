import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtWebEngine 1.0

Window   {
    id: window
    visible: true
    width: 1200
    height: 500
    color: "#202020"
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint | Qt.WindowMaximizeButtonHint
    minimumWidth: 800
    minimumHeight: 600
    // for window movement
    property bool maximalized: false
    property point startMousePos
    property point startWindowPos
    property size startWindowSize

    function absoluteMousePos(mouseArea) {
        var windowAbs = mouseArea.mapToItem(null, mouseArea.mouseX, mouseArea.mouseY)
        return Qt.point(windowAbs.x + window.x,
                        windowAbs.y + window.y)
    }

    TopBar {
        id: topBar
        anchors{
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: 25
        color: "black"
        smooth: true
    }

    Rectangle {
        id:addressBar
        color: "#303030"
        anchors{
            top: topBar.bottom
            left: parent.left
            right: parent.right
        }
        height: 20

        Rectangle {
            anchors {
                fill: parent
                leftMargin: 15
                rightMargin: 15
            }
            radius: 10
            TextInput {
                id: textInput
                text: "asd"
                color: "#151515";
                font.pixelSize: 15;
                width: parent.width-30
                anchors.centerIn: parent
            }
        }
    }

    CustomButton {
       id: button
       x: 45
       y: 359
    }

    ResizingFrames {
        anchors.fill: parent
        size: 3
    }
}

/*##^##
Designer {
    D{i:0;3d-active-scene:-1}
}
##^##*/
