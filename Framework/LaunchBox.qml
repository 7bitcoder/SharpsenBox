import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

Window   {
    id: window
    visible: true
    width: 1200
    height: 500
    color: "#202020"
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint | Qt.WindowMaximizeButtonHint
    minimumWidth: 300
    minimumHeight: 200
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
