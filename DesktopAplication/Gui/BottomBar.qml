import QtQuick 2.12
import QtWinExtras 1.1

Rectangle {
    id: loadingBar
    color: "red"

    //bottomBar.states
    property int progress
    property color barColor
    property bool animation
    property bool vis
    onAnimationChanged: {
        if (animation)
            anim.start()
        else
            anim.stop()
    }

    TaskbarButton {
        id: winPRogress
        progress {
            visible: true
            value: loadingBar.progress
            minimum: 0
            maximum: 100
            paused: bottomBar.state === bottomBar.pause
        }
    }
    Timer {
        id: prog
        interval: 50
        onTriggered: {
            progress = ((progress + 1) % 101)
            //winPRogress.setBalue(progress)
        }
        repeat: true
        running: true
    }

    Timer {
        interval: 5000
        onTriggered: {
            bottomBar.state = ((bottomBar.state + 1) % 7)
        }
        repeat: true
        running: true
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
                visible: loadingBar.vis

                NumberAnimation on x {
                    id: anim
                    from: outter.x - loader.sourceSize.width
                    to: outter.x
                    duration: 2000
                    loops: Animation.Infinite
                    running: true
                }
            }
        }
    }
}
