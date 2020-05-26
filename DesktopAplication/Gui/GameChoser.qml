import QtQuick 2.12

Rectangle {
    id: gameChoser
    property int size
    property int sylioId: 0
    property int restId: 1

    property int globSelected: sylioId
    function resetAll(id) {
        globSelected = id
        rest.selected = false
        sylio.selected = false
    }
    property color normal: "#808080"
    property real clicked: 0.3
    property real hovered: 1
    property real timeHover: 200
    property real timeClicked: 80

    Rectangle {
        id: sylio
        property bool selected: true
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: parent.size
        color: selected ? window.color : "transparent"
        onSelectedChanged: {
            if (selected)
                sylioText.color = "white"
            else
                sylioText.color = gameChoser.normal
        }
        Text {
            id: sylioText
            anchors.centerIn: parent
            text: "Sylio"
            font.family: "Arial"
            font.pointSize: 25
            color: "white"
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
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                resetAll(sylioId)
                parent.selected = true
            }
            onEntered: sylioText.color = "white"
            onExited: {
                if (!sylio.selected)
                    sylioText.color = gameChoser.normal
            }
            // onPressed: sylioText.opacity = gameChoser.clicked
            // onReleased: sylioText.opacity = gameChoser.hovered
        }
    }
    Rectangle {
        id: rest
        property bool selected: false
        anchors {
            top: parent.top
            left: sylio.right
            bottom: parent.bottom
        }
        width: parent.size
        color: selected ? window.color : "transparent"
        onSelectedChanged: {
            if (selected)
                restText.color = "white"
            else
                restText.color = gameChoser.normal
        }

        Text {
            id: restText
            anchors.centerIn: parent
            text: "Rest"
            font.family: "Arial"
            font.pointSize: 25
            color: gameChoser.normal
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
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                resetAll(restId)
                parent.selected = true
            }
            onEntered: restText.color = "white"
            onExited: {
                if (!rest.selected)
                    restText.color = gameChoser.normal
            }
            // onPressed: restText.opacity = gameChoser.clicked
            // onReleased: restText.opacity = gameChoser.hovered
        }
    }
}
