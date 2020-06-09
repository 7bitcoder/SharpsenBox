import QtQuick 2.12
import QtQuick.Controls 2.15

Item {
    ScrollView {
        anchors {
            fill: parent
            margins: 50
            rightMargin: 100
        }
        height: 500
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            Item {
                anchors.fill: parent
                anchors.leftMargin: 50
                Text {
                    anchors {
                        top: parent.top
                        topMargin: 50
                        left: parent.left
                    }
                    id: speedInfo
                    text: qsTr(
                              "Maximum download speed, 0 is treat as unlimited")
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
    }
}
