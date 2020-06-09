import QtQuick 2.12

Item {
    Rectangle {
        anchors {
            left: parent.left
            leftMargin: 100
            verticalCenter: parent.verticalCenter
        }
        height: 500
        Text {
            anchors {
                top: parent.top
                topMargin: 50
                left: parent.left
            }
            id: speedInfo
            text: qsTr("Maximum download speed, 0 is treat as unlimited")
            font.family: "Arial"
            font.pixelSize: 20
            color: "white"
        }
        Rectangle {
            id: speedInput
            anchors {
                top: speedInfo.bottom
                topMargin: 10
                left: parent.left
            }
            height: 28
            width: 200
            border {
                color: "grey"
                width: 2
            }
            radius: 3
            TextInput {
                id: textInput
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 10
                    right: parent.right
                    rightMargin: 4
                }
                clip: true
                text: "0"
                font.family: "Arial"
                font.pixelSize: 20
                selectByMouse: true
            }
        }
    }
}
