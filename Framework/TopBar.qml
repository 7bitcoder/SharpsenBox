import QtQuick 2.12
import QtQuick.Window 2.14

Rectangle {
    property Window root
    property int savedWidth: window.width
    property int savedHeigh: window.height
    property int savedX: window.x
    property int savedY: window.y
    Rectangle {
        id: close
        color: "transparent"
        width: 45
        anchors{
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        Rectangle {
            color: "white"
            rotation: -45
            transformOrigin: Item.Center
            anchors.centerIn: parent
            height: parent.height - 7
            width: 2.5
        }
        Rectangle {
            color: "white"
            rotation: 45
            transformOrigin: Item.Center
            anchors.centerIn: parent
            height: parent.height - 7
            width: 2.5
        }
        MouseArea {
           anchors.fill: parent
           hoverEnabled: true;
           onEntered: parent.color = "red"
           onExited: parent.color = "transparent"
           onClicked: Qt.quit()
        }
        Behavior on color {
            ColorAnimation { duration: 200 }
        }

    }

    Rectangle {
        id: maximalize
        color: "transparent"
        anchors{
            right: close.left
            top: parent.top
            bottom: parent.bottom
        }
        width: 35
        Rectangle {
            anchors {
                fill: parent
                margins: 7
            }
            color: "transparent"
            border{
                color: "white"
                width: 1
            }
        }
        MouseArea {
           anchors.fill: parent
           hoverEnabled: true;
           onEntered: parent.color = "blue"
           onExited: parent.color = "transparent"
           onClicked: window.showMaximized()
        }
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }


    Rectangle {
        id: minimalize
        color: "transparent"
        anchors{
            right: maximalize.left
            top: parent.top
            bottom: parent.bottom
        }
        width: 35
        Rectangle {
            anchors.centerIn: parent
            height: 2
            width: 15
            color: "white"
        }
        MouseArea {
           anchors.fill: parent
           hoverEnabled: true;
           onEntered: parent.color = "blue"
           onExited: parent.color = "transparent"
           onClicked: window.showMinimized()
        }
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }
}
