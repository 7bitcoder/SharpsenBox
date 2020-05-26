import QtQuick 2.12

Rectangle {
    property int size
    property int sylioId: 0
    property int restId: 1

    property int globSelected: sylioId
    function resetAll(id) {
        globSelected = id
        rest.selected = false
        sylio.selected = false
    }

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
        Text {
            id: sylioText
            opacity: 0.6
            anchors.centerIn: parent
            text: "Sylio"
            font.family: "Arial"
            font.pointSize: 25
            color: "white"
            Behavior on opacity {
                OpacityAnimator {
                    duration: 100
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
            onEntered: sylioText.opacity = 1
            onExited: sylioText.opacity = 0.6
            onPressed: sylioText.opacity = 0.6
            onReleased: sylioText.opacity = 1
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
        Text {
            id: restText
            anchors.centerIn: parent
            text: "Rest"
            font.family: "Arial"
            font.pointSize: 25
            color: "white"
            opacity: 0.6
            Behavior on opacity {
                OpacityAnimator {
                    duration: 100
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
            onEntered: restText.opacity = 1
            onExited: restText.opacity = 0.6
            onPressed: restText.opacity = 0.6
            onReleased: restText.opacity = 1
        }
    }
}
