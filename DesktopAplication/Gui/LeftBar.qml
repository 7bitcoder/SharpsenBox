import QtQuick 2.12

Rectangle {
    Item {
        id: accessButtons
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
            topMargin: 10
            leftMargin: 30
        }
        property real normal: 0.6
        property real clicked: 0.4
        property real hovered: 1
        property real timeHover: 200
        property real timeClicked: 80
        Item {
            id: home
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: homeImage.height
            anchors.rightMargin: 0
            property real op: 0.6
            property bool timeType: false
            Image {
                id: homeImage
                source: "content/house.png"
                height: 30
                width: 30
                opacity: parent.op
            }
            Text {
                id: homeText
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: homeImage.right
                    leftMargin: 8
                }
                text: qsTr("Home")
                font.pointSize: 15
                color: "white"
                opacity: parent.op
            }
            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onPressed: {
                    home.timeType = false
                    home.op = accessButtons.clicked
                }
                onReleased: {
                    home.timeType = false
                    home.op = containsMouse ? accessButtons.hovered : accessButtons.normal
                }
                onEntered: {
                    home.timeType = true
                    home.op = pressed ? parent.op : accessButtons.hovered
                }
                onExited: {
                    home.timeType = true
                    home.op = pressed ? parent.op : accessButtons.normal
                }
            }

            Behavior on op {
                NumberAnimation {
                    duration: home.timeType
                              === true ? accessButtons.timeHover : accessButtons.timeClicked
                }
            }
        }
        Item {
            anchors.top: home.bottom
            anchors.topMargin: 7
            id: account
            height: accountImage.height
            Image {
                id: accountImage
                source: "content/person.png"
                height: 30
                width: 30
            }
            Text {
                id: accountText
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: accountImage.right
                    leftMargin: 8
                }
                text: qsTr("Account")
                font.pointSize: 15
                color: "white"
            }
        }
        Item {
            anchors.top: account.bottom
            anchors.topMargin: 7
            id: settings
            height: settingsImage.height
            Image {
                id: settingsImage
                source: "content/cog.png"
                height: 30
                width: 30
            }
            Text {
                id: settingsText
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: settingsImage.right
                    leftMargin: 8
                }
                text: qsTr("Settings")
                font.pointSize: 15
                color: "white"
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

