import QtQuick 2.12

Rectangle {
    width: 138
    height: 75
    color: "transparent"
    Text {
        id: buttonPLay
        property bool pressed: false
        anchors.centerIn: parent
        text: "PLAY"
        font.family: "Arial"
        font.pointSize: 25
        color: "#AAAAAA"
        MouseArea {
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                parent.pressed = !parent.pressed
                parent.color = parent.pressed ? "white" :  "#DDDDDD"
            }
            onEntered: {
                parent.color = parent.pressed ? parent.color : "#DDDDDD"
            }
            onExited: {
                parent.color = parent.pressed ? parent.color : "#AAAAAA"
            }
        }
        Behavior on color {
            ColorAnimation { duration: 100 }
        }
    }
}
