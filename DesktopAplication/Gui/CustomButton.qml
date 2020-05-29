import QtQuick 2.14

Rectangle {
    width: 138
    height: 75
    color: "transparent"
    Text {
        id: buttonPLay
        property bool pressed: false
        anchors.centerIn: parent
        text: "Install"
        font.family: "Arial"
        font.pointSize: 25
        color: "#AAAAAA"
        MouseArea {
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                parent.pressed = !parent.pressed
                parent.color = parent.pressed ? "white" : "#DDDDDD"
                _Button.buttonClicked("ads")
                if (window.tmp !== 0)
                    window.tmp = 0
                else
                    window.tmp = 1
            }
            onEntered: {
                parent.color = parent.pressed ? parent.color : "#DDDDDD"
            }
            onExited: {
                parent.color = parent.pressed ? parent.color : "#AAAAAA"
            }
        }
        Behavior on color {
            ColorAnimation {
                duration: 100
            }
        }
    }
}
