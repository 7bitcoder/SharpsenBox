import QtQuick 2.12
import QtMultimedia 5.12
import QtWebEngine 1.8

Item {
    Text {
        id: description
        text: "<h2>Sylio</h2> Simple multiplayer game, players controls snail and tries to survive as long as it is possible, using various powerups and collecting points. In game you can change music settings choose your nickname, controls and change number of rounds."
        anchors {
            right: parent.right
            rightMargin: 40
            verticalCenter: holder.verticalCenter
        }
        width: 200
        wrapMode: Text.WordWrap
        color: "white"
        font.family: "Arial"
        font.pointSize: 15
    }

    Text {
        id: gameInfo
        text: "Platform:\tWindows/Linux\nSize:\t200MB"
        anchors {
            left: description.left
            top: parent.top
            topMargin: 15
        }
        width: 200
        color: "white"
        font.family: "Arial"
        font.pointSize: 12
    }

    Rectangle {
        anchors {
            left: parent.left
            top: parent.top
            topMargin: 0
            leftMargin: 20
        }
        id: button
        width: 120
        height: 80
        color: "transparent"
        Text {
            id: buttonPLay
            property bool pressed: false
            anchors.centerIn: parent
            text: "Install"
            font.family: "Arial"
            font.pointSize: 25
            color: "#AAAAAA"
            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    parent.pressed = !parent.pressed
                    parent.color = parent.pressed ? "white" : "#DDDDDD"
                    //_Button.buttonClicked("ads")
                    if (window.tmp !== 0)
                        window.tmp = 0
                    else
                        window.tmp = 1
                }
                onEntered: {
                    parent.color = parent.pressed ? parent.color : "#DDDDDD"
                }
                onExited: {
                    parent.color = parent.pressed ? parent.color : "#AAAAAA"
                }
            }
            Behavior on color {
                ColorAnimation {
                    duration: 100
                }
            }
        }
    }
    Item {
        id: holder

        anchors {
            left: parent.left
            top: parent.top
            leftMargin: 30
            topMargin: 80
            right: description.left
            rightMargin: 30
            bottom: parent.bottom
            bottomMargin: 65
        }
        Rectangle {
            color: "black"
            anchors.fill: parent
            WebEngineView {
                id: webView
                anchors.fill: parent
                url: "https://www.youtube.com/embed/T5zxTI1gxOQ?&autohide=1&showinfo=0&amp;fs=0&amp;showinfo=0"
            }
        }
        //Rectangle {
        //    id: ram
        //    color: "black"
        //    property bool toggle: true
        //    property bool pause: true
        //    onPauseChanged: pause ? video.pause() : video.play()
        //    anchors.centerIn: parent
        //    property real scal: Math.min(parent.width / 1280,
        //                                 parent.height / 720)
        //    width: 1280 * scal
        //    height: 720 * scal
        //    clip: true
        //    Video {
        //        id: video
        //        anchors.fill: parent
        //        source: "content/sylioPresentation.mp4"
        //        visible: !parent.toggle
        //        focus: true
        //        onScaleChanged: console.log(scale)
        //    }
        //    Image {
        //        id: imag
        //        anchors.fill: parent
        //        visible: parent.toggle
        //        source: "content/startVideo.jpg"
        //    }
        //    MouseArea {
        //        id: playArea
        //        anchors.fill: parent
        //        hoverEnabled: true
        //        onClicked: {
        //            if (parent.toggle)
        //                parent.toggle = false
        //            parent.pause = !parent.pause
        //        }
        //        onEntered: videoBar.show = true
        //        onExited: videoBar.show = false
        //    }
        //    Item {
        //        id: goPause
        //        anchors {
        //            bottom: parent.bottom
        //            left: parent.left
        //            leftMargin: 10
        //            bottomMargin: 3
        //        }
        //        property int size: 25
        //        width: size
        //        height: size
        //        property color col: "#CCCCCC"
        //        property real opac: videoBar.show ? 1 : 0
        //        Image {
        //            anchors.fill: parent
        //            source: "content/play-button.png"
        //            visible: ram.pause
        //            opacity: parent.opac
        //            Behavior on opacity {
        //                OpacityAnimator {
        //                    duration: 200
        //                }
        //            }
        //        }
        //        Image {
        //            anchors.fill: parent
        //            source: "content/pause-button.png"
        //            visible: !ram.pause
        //            opacity: parent.opac
        //            Behavior on opacity {
        //                OpacityAnimator {
        //                    duration: 200
        //                }
        //            }
        //        }
        //        MouseArea {
        //            anchors.fill: parent
        //            onClicked: {
        //                if (ram.toggle)
        //                    ram.toggle = false
        //                ram.pause = !ram.pause
        //            }
        //            onEntered: {
        //                goPause.col = "white"
        //                videoBar.show = true
        //            }
        //            onExited: goPause.col = "#CCCCCC"
        //            // onPressed: goPause.opac = 0.6
        //            // onReleased: goPause.opac = 1
        //        }
        //    }
        //    Text {
        //        id: videoInfo
        //        anchors {
        //            bottom: parent.bottom
        //            left: goPause.right
        //            leftMargin: 10
        //        }
        //        height: 20
        //        width: 60
        //        opacity: goPause.opac
        //        //duration
        //        property int secDur: video.duration / 1000
        //        property int minDur: secDur / 60
        //        property int modDur: secDur % 60
        //        property string dur: qsTr(minDur + ":" + (modDur < 10 ? "0" : "") + modDur)
        //        //actual
        //        property int secAct: video.position / 1000
        //        property int minAct: secAct / 60
        //        property int modAct: secAct % 60
        //        property string act: qsTr(minAct + ":" + (modAct < 10 ? "0" : "") + modAct)
        //        text: act + "/" + dur
        //        font.family: "Arial"
        //        font.pointSize: 10
        //        color: "white"
        //        Behavior on opacity {
        //            OpacityAnimator {
        //                duration: 200
        //            }
        //        }
        //    }
        //    Rectangle {
        //        id: videoBar
        //        property bool show: false
        //        property int len: 25
        //        color: "grey"
        //        visible: true
        //        anchors {
        //            bottom: parent.bottom
        //            right: parent.right
        //            left: videoInfo.right
        //            leftMargin: 5
        //        }
        //        height: videoBar.show ? 8 : 0
        //        MouseArea {
        //            anchors.fill: parent
        //            onClicked: {
        //                var seekDur = mouseX / parent.width * video.duration
        //                video.seek(seekDur)
        //            }
        //        }
        //        Rectangle {
        //            color: "red"
        //            anchors {
        //                left: parent.left
        //                top: parent.top
        //                bottom: parent.bottom
        //            }
        //            width: (video.position / video.duration) * parent.width
        //        }
        //        Behavior on height {
        //            NumberAnimation {
        //                duration: 200
        //            }
        //        }
        //    }
        //}
    }
}
