import QtQuick 2.12

Item {
    LeftBar {
        color: "#101010"
        id: leftBar
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: 180
    }

    Loader {
        id: contentLoader
        anchors {
            top: parent.top
            left: leftBar.right
            right: parent.right
            bottom: parent.bottom
        }
        source: "Home.qml"
    }
}
