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
        property bool sw: false
        //property int changeTo: "Home.qml"

        // onSourceChanged: {
        //switch (changeTo) {
        //case "Home.qml":
        //    source = changeTo
        //    break
        //case "Rest.qml":
        //    source = changeTo
        //    break
        //}
        //}
        onSourceChanged: {
            animation.stop()
            animation.start()
        }

        NumberAnimation {
            id: animation
            target: contentLoader.item
            property: "opacity"
            from: 0
            to: 1
            duration: 200
            easing.type: Easing.OutCubic
        }
    }
}
