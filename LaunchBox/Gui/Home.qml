import QtQuick 2.12
import QtWebEngine 1.8

Item {
    Rectangle {
        id: content
        property int destX: parent.x
        anchors {
            top: parent.top
            bottom: parent.bottom
        }
        width: parent.width
        color: "transparent"

        Loader {
            id: gameChoserLoader
            property int actualGame: window.selectedGame
            property int destX: parent.x
            anchors.fill: parent
            source: _Config.getPresentationFile(actualGame)
            onActualGameChanged: {
                source = _Config.getPresentationFile(actualGame)
                _GameManager.checkAutoUpdate(actualGame)
                button.installed = _Config.installed(actualGame)
            }
        }

        Rectangle {
            id: button
            anchors {
                left: parent.left
                top: parent.top
                topMargin: 80
                leftMargin: 20
            }
            property bool lock: _GameManager.lock
            property bool installed: _Config.installed(window.selectedGame)
            onLockChanged: {
                installed = _Config.installed(window.selectedGame)
            }
            width: 120
            height: 60
            color: "transparent"
            Text {
                id: buttonPLay
                anchors.centerIn: parent
                text: button.installed ? "Play" : "Install"
                font.family: latoFont.name
                font.pixelSize: 35
                color: button.lock ? "#404040" : pressArea.pressed ? "#707070" : pressArea.containsMouse ? "white" : "#AAAAAA"
                MouseArea {
                    id: pressArea
                    visible: !button.lock
                    hoverEnabled: true
                    anchors.fill: parent
                    onClicked: {
                        if (button.installed) {
                            _GameManager.runGame(window.selectedGame)
                        } else {
                            _GameManager.installGameRequest(window.selectedGame)
                        }
                    }
                }
                Behavior on color {
                    ColorAnimation {
                        duration: 100
                    }
                }
            }
        }
    }

    GameChoser {
        id: gameChoser
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 70
        color: "transparent"
        size: 150
    }
}
