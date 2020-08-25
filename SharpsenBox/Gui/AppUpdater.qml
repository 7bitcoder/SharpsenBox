import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Window 2.12

ApplicationWindow {
    id: window
    visible: true
    width: 300
    height: 300
    color: "#202020"
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint
           | Qt.WindowMaximizeButtonHint

    function absoluteMousePos(mouseArea) {
        var windowAbs = mouseArea.mapToItem(null, mouseArea.mouseX,
                                            mouseArea.mouseY)
        return Qt.point(windowAbs.x + window.x, windowAbs.y + window.y)
    }

    FontLoader { id: latoFont; source: "Font/Lato-Regular.ttf" }

    property int status: _AppUpdaterManager.updateState
    property string statusStr: "Searching for updates"
    property int progress: _AppUpdaterManager.progress
    onStatusChanged: {
        switch (status) {
        case -1:
        case 0:
            //none
            statusStr = "Searching for updates"
            barStatusBackground.visible = false
            break
        case 1:
            //downloading
            statusStr = "Downloading update"
            barStatusBackground.visible = true
            break
        case 2:
            // installing
            statusStr = "Installing update"
            barStatusBackground.visible = true
            break
        case 6:
            //ended
            statusStr = "Starting SharpsenBox"
            barStatusBackground.visible = false
            exiter.start()
            break
        case 4:
            //error
            statusStr = "Error ocured:\n" + _AppUpdaterManager.statusStr
            barStatusBackground.visible = false
            exiter.start()
            break
        case 5:
            //noUpdateFound
            statusStr = "No updates found"
            barStatusBackground.visible = false
            exiter.start()
            break
        case 7:
            // updating games information
            statusStr = "Updating game pages"
            barStatusBackground.visible = false
            break
        }
    }

    Timer {
        id: exiter
        interval: 1000
        onTriggered: {
            Qt.quit()
        }
        repeat: false
        running: false
    }

    Timer {
        id: infoTim
        interval: 500
        onTriggered: {
            _AppUpdaterManager.checkForUpdates()
        }
        repeat: false
        running: true
    }

    property point startMousePos
    property point startWindowPos

    Rectangle {
        anchors.fill: parent
        color: "#151515"
        Text {
            id: info
            anchors {
                bottom: parent.bottom
                bottomMargin: 60
                horizontalCenter: parent.horizontalCenter
            }
            text: statusStr //_AppUpdater.statusStr
            font.family: latoFont.name
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            color: "#AAAAAA"
        }

        Rectangle {
            id: barStatusBackground
            visible: false
            anchors {
                bottom: parent.bottom
                bottomMargin: 30
                left: parent.left
                right: parent.right
                leftMargin: 20
                rightMargin: 20
            }
            height: 8
            color: "black"
            radius: 4
            Rectangle {
                id: barStatus
                anchors {
                    top: parent.top
                    left: parent.left
                    bottom: parent.bottom
                }
                visible: parent.visible
                width: window.progress / 100 * parent.width
                color: "white"
                radius: parent.radius
            }
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
    MouseArea {
        id: moveBarArea
        anchors.fill: parent

        onPressed: {
            startMousePos = absoluteMousePos(this)
            startWindowPos = Qt.point(window.x, window.y)
        }
        onMouseXChanged: {
            if (pressed) {
                var abs = absoluteMousePos(this)
                window.x = startWindowPos.x + (abs.x - startMousePos.x)
            }
        }
        onMouseYChanged: {
            if (pressed) {
                var abs = absoluteMousePos(this)
                window.y = Math.max(
                            Screen.virtualY,
                            startWindowPos.y + (abs.y - startMousePos.y))
            }
        }
    }
}
