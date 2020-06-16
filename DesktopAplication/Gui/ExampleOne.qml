import QtQuick 2.12
import QtMultimedia 5.12
import QtWebEngine 1.8

Item {
    id: sylio
    property int gameId: 1
    property string descriptionStr: "<h2>Sylio</h2> Simple multiplayer game, players controls snail and tries to survive as long as it is possible, using various powerups and collecting points. In game you can change music settings choose your nickname, controls and change number of rounds."
    property string platformStr: "Platform:\tWindows/Linux\nSize:\t200MB"
    Text {
        id: description
        text: parent.descriptionStr
        anchors {
            right: parent.right
            rightMargin: 40
            verticalCenter: holder.verticalCenter
        }
        width: 200
        wrapMode: Text.WordWrap
        color: "white"
        font.family: "Arial"
        font.pointSize: 15
    }

    Text {
        id: gameInfo
        text: parent.platformStr
        anchors {
            left: description.left
            top: parent.top
            topMargin: 15
        }
        width: 200
        color: "white"
        font.family: "Arial"
        font.pointSize: 12
    }

    Rectangle {
        anchors {
            left: parent.left
            top: parent.top
            topMargin: 0
            leftMargin: 20
        }
        id: button
        width: 120
        height: 80
        color: "transparent"
        Text {
            id: buttonPLay
            property bool lock: _GameManager.lock
            property bool installed: _Config.installed(gameId)
            property bool realLock: false
            onLockChanged: {
                installed = _Config.installed(gameId)
            }

            anchors.centerIn: parent
            text: installed ? "Play" : "Install"
            font.family: "Arial"
            font.pointSize: 25
            color: lock ? "#404040" : pressArea.pressed ? "#707070" : pressArea.containsMouse ? "white" : "#AAAAAA"
            MouseArea {
                id: pressArea
                visible: !parent.lock
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    if (buttonPLay.installed) {
                        _GameExecutor.execute(gameId)
                    } else {
                        window.gameId = sylio.gameId
                        window.stage = 1
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
    Item {
        id: holder

        anchors {
            left: parent.left
            top: parent.top
            leftMargin: 30
            topMargin: 80
            right: description.left
            rightMargin: 30
            bottom: parent.bottom
            bottomMargin: 65
        }
        Rectangle {
            color: "black"
            anchors.fill: parent
            visible: true
            WebEngineView {
                id: webView
                anchors.fill: parent
                url: "https://www.youtube.com/embed/T5zxTI1gxOQ?&autohide=1&showinfo=0&amp;fs=0&amp;showinfo=0"
            }
        }
    }
}
