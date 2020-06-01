import QtQuick 2.12

Rectangle {
    anchors.fill: parent
    color: window.color
    Rectangle {
        anchors.centerIn: parent
        width: 300
        height: 300
        color: "#151515"
        radius: 20
        Text {
            id: info
            anchors {
                bottom: parent.bottom
                bottomMargin: 40
                horizontalCenter: parent.horizontalCenter
            }
            text: qsTr("Checking for updates")
            font.family: "Arial"
            font.pointSize: 20
            color: "#AAAAAA"
        }

        Item {

            anchors {
                top: parent.top
                topMargin: 80
                horizontalCenter: parent.horizontalCenter
            }
            width: 100
            height: 80

            property alias barCount: repeater.model
            property color color: "white"
            property int spacing: 5

            id: root

            Repeater {
                id: repeater
                model: 4
                delegate: Component {
                    Rectangle {
                        width: (root.width / root.barCount) - root.spacing
                        height: root.height
                        x: index * width + root.spacing * index
                        transform: Scale {
                            id: rectScale
                            origin {
                                x: width / 2
                                y: height / 2
                            }
                        }
                        transformOrigin: Item.Center
                        color: root.color

                        SequentialAnimation {
                            id: anim
                            loops: Animation.Infinite

                            NumberAnimation {
                                target: rectScale
                                property: "yScale"
                                from: 1
                                to: 1.5
                                duration: 300
                            }
                            NumberAnimation {
                                target: rectScale
                                property: "yScale"
                                from: 1.5
                                to: 1
                                duration: 300
                            }
                            PauseAnimation {
                                duration: root.barCount * 150
                            }
                        }

                        function playAnimation() {
                            if (anim.running == false) {
                                anim.start()
                            }
                        }
                    }
                }
            }

            Timer {
                // ----- Private Properties ----- //
                property int _barIndex: 0

                interval: 80
                repeat: true
                onTriggered: {
                    if (_barIndex === root.barCount) {
                        stop()
                    } else {
                        repeater.itemAt(_barIndex).playAnimation()
                        _barIndex++
                    }
                }
                Component.onCompleted: start()
            }
        }
    }
}
