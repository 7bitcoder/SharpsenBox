import QtQuick 2.12

Rectangle {
    id: block
    property bool selected: window.selectedGame === gameId
    property int gameId
    width: gameChoser.size
    height: gameChoser.height
    opacity: 0.85
    color: selected ? "transparent" : gameChoser.background
    Text {
        id: sylioText
        anchors.centerIn: parent
        text: _Config.getGameName(gId)
        font.family: "Arial"
        font.pointSize: 25
        color: blockMouse.containsMouse ? "white" : gameChoser.normal
        Behavior on color {
            ColorAnimation {
                duration: gameChoser.timeClicked
            }
        }
    }
    MouseArea {
        id: blockMouse
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            window.selectedGame = gameId
        }
    }
}
