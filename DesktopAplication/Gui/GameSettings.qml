import QtQuick 2.12
import QtQuick.Controls 2.15

Rectangle {
    id: gameBarList
    property string name
    property bool hide
    property bool listed: false
    property int minHeight: 50
    property int maxHeight: 150
    property int gameId
    anchors.topMargin: hide ? 0 : 20
    color: "#404040"
    height: hide ? 0 : bar.height + list.height
    Rectangle {
        id: bar
        color: "#808080"
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: parent.hide ? 0 : parent.minHeight
        Text {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 10
            }
            visible: !gameBarList.hide
            text: name
            color: "white"
            font.family: "Arial"
            font.pixelSize: 20
        }
        MouseArea {
            anchors.fill: parent
            onClicked: gameBarList.listed = !gameBarList.listed
        }
    }
    Rectangle {
        id: list
        anchors {
            top: bar.bottom
            left: bar.left
            right: bar.right
        }
        property int inner: 10
        height: parent.listed ? parent.maxHeight : 0
        color: gameBarList.color
        Behavior on height {
            NumberAnimation {
                duration: 200
            }
        }
        Button {
            id: checkFilesIntegrity
            anchors {
                bottom: uninstall.top
                bottomMargin: 10
                left: parent.left
                leftMargin: list.inner
            }
            height: 30
            width: 300
            text: "Check files integrity"
            onClicked: {

            }

            contentItem: Text {
                text: checkFilesIntegrity.text
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
                opacity: checkFilesIntegrity.down ? 0.7 : 1
                radius: 5
                color: "grey"
                Behavior on opacity {
                    NumberAnimation {
                        duration: 100
                    }
                }
            }
        }
        Button {
            id: uninstall
            anchors {
                bottom: updater.top
                bottomMargin: 10
                left: parent.left
                leftMargin: list.inner
            }
            height: 30
            width: 300
            text: "Uninstall"
            onClicked: {

            }

            contentItem: Text {
                text: uninstall.text
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
                opacity: uninstall.down ? 0.7 : 1
                radius: 5
                color: "grey"
                Behavior on opacity {
                    NumberAnimation {
                        duration: 100
                    }
                }
            }
        }
        CheckBox {
            id: updater
            anchors {
                bottom: list.bottom
                bottomMargin: 10
                left: parent.left
                leftMargin: list.inner
            }
            checked: _Config.getGameAutoCheck(gameBarList.gameId)
            onCheckedChanged: _Config.setGameAutoCheck(gameBarList.gameId,
                                                       checked)
        }
        clip: true
        Text {
            text: "Automatically check for updates"
            font.family: "Arial"
            font.pixelSize: 20
            color: "white"
            anchors {
                verticalCenter: updater.verticalCenter
                left: updater.right
                leftMargin: 10
            }
        }
    }
}
