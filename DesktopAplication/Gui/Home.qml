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

            width: 120
            height: 60
            color: "transparent"
            Text {
                id: buttonPLay
                property bool lock: _GameManager.lock
                property bool installed: _Config.installed(window.selectedGame)
                property bool realLock: false
                onLockChanged: {
                    installed = _Config.installed(window.selectedGame)
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
                            _GameManager.runGame(window.selectedGame)
                        } else {
                            _GameManager.installGame(window.selectedGame)
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

    //Loader {
    //    id: gameChoserLoader
    //    anchors {
    //        top: gameChoser.bottom
    //        //left: parent.left
    //        //right: parent.right
    //        bottom: parent.bottom
    //    }
    //    property int destX: parent.x
    //    width: parent.width
    //    source: window.gameBarS
    //    onSourceChanged: {
    //        animation.stop()
    //        animationOp.stop()
    //        animation.start()
    //        animationOp.start()
    //    }
    //
    //    NumberAnimation {
    //        id: animation
    //        target: gameChoserLoader.item
    //        property: "x"
    //        from: gameChoserLoader.destX + gameChoserLoader.width
    //        to: gameChoserLoader.destX
    //        duration: 300
    //        easing.type: Easing.OutQuart
    //    }
    //
    //    NumberAnimation {
    //        id: animationOp
    //        target: gameChoserLoader.item
    //        property: "opacity"
    //        from: 0
    //        to: 1
    //        duration: 300
    //        easing.type: Easing.OutCubic
    //    }
    //}
}
