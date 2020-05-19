import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

Window   {
    id: window
    visible: true
    width: 1200
    height: 500
    color: "#202020"
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint | Qt.WindowMaximizeButtonHint

    MouseArea {
        anchors.fill: parent
        property point lastMousePos: Qt.point(0, 0)
        onPressed: { lastMousePos = Qt.point(mouseX, mouseY); }
        onMouseXChanged: window.x += (mouseX - lastMousePos.x)
        onMouseYChanged: window.y += (mouseY - lastMousePos.y)
    }

    TopBar {
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
}

/*##^##
Designer {
    D{i:0;3d-active-scene:-1}
}
##^##*/
