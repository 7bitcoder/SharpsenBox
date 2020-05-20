import QtQuick 2.14
import QtQuick.Window 2.14

Rectangle {
    id: bar
    property int savedWidth: window.width
    property int savedHeigh: window.height
    property int savedX: window.x
    property int savedY: window.y
    MouseArea {
        id: moveBarArea
        anchors{
            top: parent.top
            right: minimalize.left
            left: parent.left
            bottom: parent.bottom
        }
        property point lastMousePos: Qt.point(0, 0)
        function maximalizeFunc(protect) {
            window.maximalized = !window.maximalized
            if(protect === true) {
                moveProtector.protect = true
                moveProtector.restart()
            }
            if(window.maximalized){
                 window.showMaximized()
            }
            else{
                 window.showNormal()
            }
        }

        Timer {
            id: moveProtector
            property bool protect: false
            property real len
            property int ycoord
            property point abs
            interval: 1; running: false; repeat: false
            onTriggered: {
                protect = false
                if(len > 0) {
                    var xcoord = Math.floor(abs.x - (moveBarArea.width*len))
                    startWindowPos = Qt.point(xcoord, ycoord)
                    startWindowSize = Qt.size(window.width, window.height)
                    window.x = xcoord
                    window.y = ycoord
                }
                len = 0
            }
        }

        onDoubleClicked: maximalizeFunc(true)
        onPressed: {
            startMousePos = absoluteMousePos(this)
            startWindowPos = Qt.point(window.x, window.y)
            startWindowSize = Qt.size(window.width, window.height)
        }
        onMouseXChanged:{
            if(pressed && !window.maximalized && !moveProtector.protect) {
                var abs = absoluteMousePos(this)
                window.x = startWindowPos.x + (abs.x - startMousePos.x)
            }
        }
        onMouseYChanged: {
            if(pressed && !moveProtector.protect) {
                var abs = absoluteMousePos(this)
                window.y = Math.max(Screen.virtualY, startWindowPos.y + (abs.y - startMousePos.y))

                if( window.y < Screen.virtualY + 1 && !window.maximalized )
                    maximalizeFunc(true)
                if( window.y > Screen.virtualY && window.maximalized ) {
                    var xcoord = mouseX/moveBarArea.width;
                    maximalizeFunc(false)
                    moveProtector.len = xcoord
                    moveProtector.abs = abs
                    moveProtector.ycoord = abs.y - mouseY
                    moveProtector.protect = true
                    moveProtector.start()
                    startMousePos = abs
                }
            }
        }
    }

    Rectangle {
        id: close
        color: "transparent"
        width: 45
        anchors{
            right: parent.right
            top: parent.top
        }
        height: parent.height
        Rectangle {
            color: "white"
            rotation: -45
            transformOrigin: Item.Center
            anchors.centerIn: parent
            height: parent.height - 7
            width: 2.5
        }
        Rectangle {
            color: "white"
            rotation: 45
            transformOrigin: Item.Center
            anchors.centerIn: parent
            height: parent.height - 7
            width: 2.5
        }
        MouseArea {
           anchors.fill: parent
           hoverEnabled: true;
           onEntered: parent.color = "tomato"
           onExited: parent.color = "transparent"
           onClicked: Qt.quit()
        }
        Behavior on color {
            ColorAnimation { duration: 200 }
        }

    }

    Rectangle {
        id: maximalize;
        color: bar.color

        anchors{
            right: close.left
            top: parent.top
        }
        height: parent.height
        width: 35
        Rectangle {
            id: max;
            visible: !parent.maximalized
            anchors {
                fill: parent
                margins: 7
            }
            color: "transparent"
            border{
                color: "white"
                width: 1
            }
        }
        Rectangle {
            id: min;
            visible: window.maximalized
            anchors {
                fill: maximalize
                leftMargin: 7
                rightMargin: 9
                topMargin: 9
                bottomMargin: 7
            }
            color: maximalize.color
            border{
                color: "white"
                width: 1
            }
            Rectangle {
                visible: window.maximalized
                anchors {
                    fill: parent
                    leftMargin: -2
                    rightMargin: 2
                    topMargin: 2
                    bottomMargin: -2
                }
                color: maximalize.color
                border{
                    color: "white"
                    width: 1
                }
            }
        }
        MouseArea {
           id: maximalizator
           anchors.fill: parent
           hoverEnabled: true;
           onEntered: maximalize.color = "skyblue"
           onExited: maximalize.color = bar.color
           onClicked: {
               window.maximalized = !window.maximalized
               if(window.maximalized){
                    window.showMaximized()
               }
               else{
                    window.showNormal()
               }
            }
        }
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }


    Rectangle {
        id: minimalize
        color: "transparent"
        anchors{
            right: maximalize.left
            top: parent.top
        }
        height: parent.height
        width: 35
        Rectangle {
            anchors.centerIn: parent
            height: 2
            width: 15
            color: "white"
        }
        MouseArea {
           anchors.fill: parent
           hoverEnabled: true;
           onEntered: parent.color = "skyblue"
           onExited: parent.color = "transparent"
           onPressed: parent.opacity = 0.6
           onReleased: parent.opacity = 1
           onClicked: window.showMinimized()
        }
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
        Behavior on opacity {
            opa { duration: 100 }
        }
    }
}
