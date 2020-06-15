import QtQuick 2.12
import QtWebEngine 1.8

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

    Rectangle {
        id: content
        property int destX: parent.x
        anchors {
            top: gameChoser.bottom
            bottom: parent.bottom
        }
        width: parent.width
        color: "transparent"
        Rectangle {
            id: button
            anchors {
                left: parent.left
                top: parent.top
                topMargin: 0
                leftMargin: 20
            }

            width: 120
            height: 80
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
                            _GameExecutor.execute(window.selectedGame)
                        } else {
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

        Rectangle {
            id: webBar
            color: "grey"
            anchors{
                bottom: webView.top
                left: parent.left
            }
            height: 5
            width: (webView.loadProgress/100)*parent.width
        }

        WebEngineView {
            visible: true
            id: webView
            property int actualGame: window.selectedGame
            anchors {
                top: button.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            backgroundColor: "transparent"
            url: _Config.getGamePresentationUrl(actualGame) //"https://www.youtube.com/embed/T5zxTI1gxOQ?&autohide=1&showinfo=0&amp;fs=0&amp;showinfo=0"
            onActualGameChanged: {
                profile.clearHttpCache()
                url = _Config.getGamePresentationUrl(actualGame)
                _GameManager.checkAutoUpdate(actualGame)
                //animation.stop()
               // animationOp.stop()
               // animation.start()
                //animationOp.start()

            }
            onContextMenuRequested: function(request) {
                request.accepted = true;
            }
        }
        MouseArea{
        }
    }
    //NumberAnimation {
    //    id: animation
    //    target: content
    //    property: "x"
    //    from: content.destX + parent.width
    //    to: content.destX
    //    duration: 300
    //    easing.type: Easing.OutQuart
    //}
    //NumberAnimation {
    //    id: animationOp
    //    target: content
    //    property: "opacity"
    //    from: 0
    //    to: 1
    //    duration: 300
    //    easing.type: Easing.OutCubic
    //}
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
