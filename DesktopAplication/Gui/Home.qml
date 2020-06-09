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
            //left: parent.left
            //right: parent.right
            bottom: parent.bottom
        }
        property int destX: parent.x
        width: parent.width
        source: window.gameBarS
        onSourceChanged: {
            animation.stop()
            animationOp.stop()
            animation.start()
            animationOp.start()
        }

        NumberAnimation {
            id: animation
            target: gameChoserLoader.item
            property: "x"
            from: gameChoserLoader.destX + gameChoserLoader.width
            to: gameChoserLoader.destX
            duration: 300
            easing.type: Easing.OutQuart
        }
        NumberAnimation {
            id: animationOp
            target: gameChoserLoader.item
            property: "opacity"
            from: 0
            to: 1
            duration: 300
            easing.type: Easing.OutCubic
        }
    }
}
