import QtQuick 2.12

//import QtWinExtras 1.1
Rectangle {
    id: bottomBar
    color: "#292929"

    //max height
    property int len
    height: 0

    //states of instatation
    property int checking: 0
    property int downloading: 1
    property int installing: 2
    property int pause: 3
    property int error: 4
    property int stopped: 5
    property int completed: 6

    //stete
    property int state: _BottomBar.downloadState

    //porgress property
    property real progress: _BottomBar.progress

    property color barColor: "green"

    //progress settings
    property int updateInterval: 100
    property real actualDown: _BottomBar.actual
    property real total: _BottomBar.total

    property real lastDown: _BottomBar.actual
    property string actualDownloaded: actualDown
    property string totalStr: actualDown
    onTotalChanged: {
        console.log(total)
        if (total > 1000)
            totalStr = (total / 1000).toFixed(2) + "GB"
        else {
            totalStr = total.toFixed(2) + "GB"
        }
    }

    property string speed: "0"

    property string statusInfo: "Downloading: " + actualDownloaded + "/"
                                + totalStr + " (" + speed + "MB/s)"
    property string percentage: qsTr(progress.toFixed(1) + "%")

    //bar states
    property int hidden: 0
    property int showed: 1
    property int minimalized: 2

    property int visibleState: _BottomBar.visibleState

    property bool hideLock: _BottomBar.hideLock
    onStateChanged: {
        if (state === checking) {
            bottomBar.barColor = "white"
            loader.visible = true
            anim.start()
            //bottomBar.progress = 100
        } else if (state === downloading) {
            bottomBar.barColor = "orange"
            loader.visible = false
            anim.stop()
        } else if (state === installing) {
            loader.visible = true
            anim.start()
            bottomBar.barColor = "orange"
        } else if (state === pause) {
            anim.stop()
            bottomBar.barColor = "yellow"
        } else if (state === error) {
            anim.stop()
            bottomBar.barColor = "red"
        } else if (state === completed) {
            anim.stop()
            loader.visible = false
            bottomBar.barColor = "green"
        }
    }

    //hiding showing animation
    Behavior on height {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
            onRunningChanged: {
                if (!running
                        && bottomBar.visibleState === bottomBar.minimalized)
                    minimalizedBar.height = 10
            }
        }
    }

    onVisibleStateChanged: {
        if (visibleState === minimalized) {
            bottomBar.height = 0
        } else if (visibleState === showed) {
            if (minimalizedBar.height > 0)
                minimalizedBar.height = 0
            else
                bottomBar.height = bottomBar.len
        } else {
            bottomBar.height = 0
            minimalizedBar.height = 0
        }
    }

    //tmp timers for test purposes
    Timer {
        id: prog
        interval: updateInterval
        onTriggered: {
            var actualDown = _BottomBar.actual

            var dt = actualDown - lastDown
            speed = (dt * 1000 / updateInterval).toFixed(2)
            lastDown = actualDown

            if (actualDown > 1000) {
                actualDown /= 1000
                actualDownloaded = actualDown.toFixed(2) + "GB"
            } else {
                actualDownloaded = actualDown.toFixed(2) + "MB"
            }
        }
        repeat: true
        running: state == downloading
    }

    //Timer {
    //    interval: 5000
    //    onTriggered: {
    //        bottomBar.state = ((bottomBar.state + 1) % 7)
    //    }
    //    repeat: true
    //    running: true
    //}

    //minimalized bar
    Rectangle {
        id: minimalizedBar
        anchors {
            left: bottomBar.left
            right: bottomBar.right
            bottom: bottomBar.bottom
        }
        visible: true
        property int minLen: 6
        color: "grey"
        Rectangle {
            id: minimalizedPrBar
            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
            }
            color: bottomBar.barColor
            width: (bottomBar.progress / 100) * parent.width
        }
        //normal mouse area
        MouseArea {
            anchors.fill: parent
            onClicked: if (!bottomBar.hideLock)
                           bottomBar.visibleState = bottomBar.showed
        }

        Behavior on height {
            NumberAnimation {
                duration: 100
                onRunningChanged: {
                    if (running === false
                            && bottomBar.visibleState === bottomBar.showed)
                        bottomBar.height = bottomBar.len
                }
            }
        }
    }

    // TaskbarButton {
    //     id: winPRogress
    //     progress {
    //         visible: true
    //         value: progress
    //         minimum: 0
    //         maximum: 100
    //         paused: state === pause
    //     }
    // }
    Item {
        id: controls
        property int size: 30
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: 100
        Item {
            id: goPause
            anchors {
                left: parent.left
                top: parent.top
                topMargin: 10
                leftMargin: 20
            }
            width: parent.size
            height: parent.size
            property bool play: false
            onPlayChanged: {
                console.log("heheheh")
                if (play)
                    _BottomBar.pauseD()
                else
                    _BottomBar.resumeD()
            }

            property real opac: 0.6
            Image {
                anchors.fill: parent
                source: "content/play-button.png"
                visible: parent.play
                opacity: parent.opac
                Behavior on opacity {
                    OpacityAnimator {
                        duration: 100
                    }
                }
            }
            Image {
                anchors.fill: parent
                source: "content/pause-button.png"
                visible: !parent.play
                opacity: parent.opac
                Behavior on opacity {
                    OpacityAnimator {
                        duration: 100
                    }
                }
            }
            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    parent.play = !parent.play
                }
                onEntered: goPause.opac = 1
                onExited: goPause.opac = 0.6
                onPressed: goPause.opac = 0.6
                onReleased: goPause.opac = 1
            }
        }
        Item {
            id: stop
            anchors {
                left: goPause.right
                top: parent.top
                topMargin: 10
                leftMargin: 20
            }
            width: parent.size
            height: parent.size
            Image {
                id: stopImag
                source: "content/plain-square.png"
                anchors.fill: parent
                opacity: 0.6
                Behavior on opacity {
                    OpacityAnimator {
                        duration: 100
                    }
                }
            }
            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    _BottomBar.stopD()
                }
                onEntered: stopImag.opacity = 1
                onExited: stopImag.opacity = 0.6
                onPressed: stopImag.opacity = 0.6
                onReleased: stopImag.opacity = 1
            }
        }
    }

    Rectangle {
        id: loadingBar
        anchors {
            left: controls.right
            leftMargin: 20
            rightMargin: 20
            right: parent.right
            top: parent.top
            topMargin: 10
        }
        height: 10
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
            color: bottomBar.barColor
            property int max: parent.width - 2 * parent.border.width
            width: (bottomBar.progress / 100) * max
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
                visible: true

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

    //status text
    Text {
        id: status
        anchors {
            left: loadingBar.left
            top: loadingBar.bottom
            topMargin: 9
        }
        font.family: "Arial"
        font.pointSize: 12
        color: "white"
        text: statusInfo
    }
    Text {
        id: percent
        anchors {
            right: loadingBar.right
            top: loadingBar.bottom
            topMargin: 9
        }
        font.family: "Arial"
        font.pointSize: 12
        color: "white"
        text: percentage
    }
    //minimalize mouse area
    MouseArea {
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            left: loadingBar.left
        }

        onClicked: if (!bottomBar.hideLock)
                       bottomBar.visibleState = bottomBar.minimalized
    }
}
