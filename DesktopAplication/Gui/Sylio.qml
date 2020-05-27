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
            verticalCenter: image.verticalCenter
        }
        width: 200
        wrapMode: Text.WordWrap
        color: "white"
        font.family: "Arial"
        font.pointSize: 15
    }
    // Video {
    //     transformOrigin: Item.TopLeft
    //     source: "content/sylioPresentation.avi"
    //     visible: true
    //     autoPlay: true
    //     autoLoad: true
    //     loops: Animation.Infinite
    // }
    WebEngineView {
        id: image
        anchors {
            left: parent.left
            top: parent.top
            leftMargin: 30
            topMargin: 100
            right: description.left
            rightMargin: 30
            bottom: parent.bottom
            bottomMargin: 65
        }
        settings.javascriptCanAccessClipboard: true
        settings.javascriptCanOpenWindows: true
        settings.javascriptEnabled: true
        settings.pluginsEnabled: true
        settings.autoLoadImages: true
        visible: true
        url: "https://www.youtube.com/embed/T5zxTI1gxOQ?autoplay=0&fs=0&iv_load_policy=3&showinfo=0&rel=0&cc_load_policy=0"
    }

    CustomButton {
        id: button
        anchors.left: parent.left
        anchors.top: parent.top
    }
}
