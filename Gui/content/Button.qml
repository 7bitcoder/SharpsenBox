import QtQuick 2.0

Item {
    id: button
    property alias text: textItem.text
    property color color: "#eceeea"

    property bool operator: false
    property bool dimmable: false
    property bool dimmed: false

    width: 30
    height: 50

    Text {
        id: textItem
        font.pixelSize: 48
        wrapMode: Text.WordWrap
        lineHeight: 0.75
        color: (dimmable && dimmed) ? Qt.darker(button.color) : button.color
        Behavior on color { ColorAnimation { duration: 120; easing.type: Easing.OutElastic} }
        states: [
            State {
                name: "pressed"
                when: mouse.pressed && !dimmed
                PropertyChanges {
                    target: textItem
                    color: Qt.lighter(button.color)
                }
            }
        ]
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        anchors.margins: -5
        onClicked: {
            if (operator)
                window.operatorPressed(parent.text)
            else
                window.digitPressed(parent.text)
        }
    }

    function updateDimmed() {
        dimmed = window.isButtonDisabled(button.text)
    }

    Component.onCompleted: {
        numPad.buttonPressed.connect(updateDimmed)
        updateDimmed()
    }
}