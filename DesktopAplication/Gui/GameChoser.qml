import QtQuick 2.12

Rectangle {
    id: gameChoser
    property int size
    // game state id
    property int sylioId: 0
    property int restId: 1
    //state
    property int globSelected: sylioId
    function resetAll(id) {
        globSelected = id
        rest.selected = false
        sylio.selected = false
    }
    //buttons settings/color/opavity etc
    property color normal: "#808080"
    property real clicked: 0.3
    property real hovered: 1
    property real timeHover: 200
    property real timeClicked: 80

    Rectangle {
        id: sylio
        property bool selected: window.gameBarS === "Sylio.qml"
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: parent.size
        color: selected || sylioM.containsMouse ? window.color : "transparent"
        onSelectedChanged: {
            if (selected)
                window.gameBarS = "Sylio.qml"
        }
        Text {
            id: sylioText
            anchors.centerIn: parent
            text: "Sylio"
            font.family: "Arial"
            font.pointSize: 25
            color: parent.selected ? "white" : gameChoser.normal
            Behavior on opacity {
                OpacityAnimator {
                    duration: gameChoser.timeClicked
                }
            }
            Behavior on color {
                ColorAnimation {
                    duration: gameChoser.timeHover
                }
            }
        }
        MouseArea {
            id: sylioM
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                resetAll(sylioId)
                parent.selected = true
            }
            // onPressed: sylioText.opacity = gameChoser.clicked
            // onReleased: sylioText.opacity = gameChoser.hovered
        }
    }
    Rectangle {
        id: rest
        property bool selected: window.gameBarS === "Rest.qml"
        anchors {
            top: parent.top
            left: sylio.right
            bottom: parent.bottom
        }
        width: parent.size
        color: selected || restM.containsMouse ? window.color : "transparent"
        onSelectedChanged: {
            if (selected)
                window.gameBarS = "Rest.qml"
        }

        Text {
            id: restText
            anchors.centerIn: parent
            text: "Rest"
            font.family: "Arial"
            font.pointSize: 25
            color: parent.selected ? "white" : gameChoser.normal
            Behavior on opacity {
                OpacityAnimator {
                    duration: gameChoser.timeClicked
                }
            }
            Behavior on color {
                ColorAnimation {
                    duration: gameChoser.timeHover
                }
            }
        }
        MouseArea {
            id: restM
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                resetAll(restId)
                parent.selected = true
            }
            // onPressed: restText.opacity = gameChoser.clicked
            // onReleased: restText.opacity = gameChoser.hovered
        }
    }
}
