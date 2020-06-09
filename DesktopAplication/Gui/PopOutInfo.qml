import QtQuick 2.12
import Qt.labs.platform 1.1
import QtQuick.Controls 2.12

Rectangle {
    id: popInfo
    implicitHeight: 800
    implicitWidth: 800
    property int gameId: 1
    property int normalTextSize: 15
    property string info: window.info
    color: "transparent"
    Rectangle {
        anchors.fill: parent
        opacity: 0.5
        color: "grey"
    }
    MouseArea {
        anchors.fill: parent
    }
    Rectangle {
        color: "white"
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }
        height: 150
        Rectangle {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }
            width: 500
            Text {
                id: infromation
                anchors.centerIn: parent
                font.family: "Arial"
                font.pixelSize: 25
                color: "black"
                text: popInfo.info
            }

            Button {
                id: ok
                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    bottomMargin: 10
                    rightMargin: 20
                }
                height: 30
                width: 100
                text: "Ok"
                onClicked: {
                    window.stage = 0
                }

                contentItem: Text {
                    text: ok.text
                    font.family: "Arial"
                    font.pixelSize: 22
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                background: Rectangle {
                    implicitWidth: 50
                    implicitHeight: 25
                    opacity: ok.down ? 0.7 : 1
                    radius: 5
                    color: "green"
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }
        }
    }
}
