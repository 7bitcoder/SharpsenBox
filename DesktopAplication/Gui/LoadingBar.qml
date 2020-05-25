import QtQuick 2.12

Rectangle {
    id: loadingBar
    color: "red"
    //states
    property int downloading: 1
    property int installing: 2
    property int pause: 3
    property int error: 4
    property int stopped: 5

    property int state: error
    property real progress: 60
    property color barColor: "green"
    onStateChanged: {
        if (state === downloading) {
            barColor = "orange"
            loader.visible = true
            loader.stop = false
        } else if (state === installing) {
            loader.visible = true
            loader.stop = false
            barColor = "green"
        } else if (state === pause) {
            loader.stop = true
        } else if (state === error) {
            loader.stop = true
            barColor = "red"
        }
    }

    Rectangle {
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
        height: 30
        color: "grey"
        border {
            color: "black"
            width: 0
        }
        smooth: true
        Rectangle {
            id: outter
            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
                margins: parent.border.width
            }
            clip: true
            color: loadingBar.barColor
            property int max: parent.width - 2 * parent.border.width
            width: (loadingBar.progress / 100) * max
            radius: parent.radius
            BorderImage {
                id: loader
                source: "content/loader.png"
                x: parent.x
                y: parent.y
                height: parent.height
                width: parent.width * 2
                horizontalTileMode: BorderImage.Repeat
                opacity: 0.5
                property bool stop: false
                NumberAnimation on x {
                    from: outter.x - loader.sourceSize.width
                    to: outter.x
                    duration: 2000
                    loops: Animation.Infinite
                    paused: visible && !stop
                }
            }
        }
    }
}
