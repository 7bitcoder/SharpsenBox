import QtQuick 2.12

Item {
    GameChoser {
        id: gameChoser
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 70
        color: "#292929"
        size: 150
    }

    Loader {
        id: gameChoserLoader
        anchors {
            top: gameChoser.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        source: "Sylio.qml"
    }
}
