import QtQuick 2.12

Item {
    id: frame
    property Item content
    property int size
    property var topBorder: resizeTop.bottom
    property var leftBorder: resizeTopLeft.right
    property var rightBorder: resizeTopRight.left
    MouseArea {
        enabled: !window.maximalized
        id: resizeTopRight
        anchors{
            top: parent.top
            right: parent.right
        }
        height: size
        width: size
        property point lastMousePos: Qt.point(0, 0)
        hoverEnabled: true
        onHoveredChanged: cursorShape = (!window.maximalized && (containsMouse || pressed) ?  Qt.SizeBDiagCursor :  Qt.ArrowCursor)
        onPressed: {
            startMousePos = absoluteMousePos(this)
            startWindowPos = Qt.point(window.x, window.y)
            startWindowSize = Qt.size(window.width, window.height)
        }
        onMouseXChanged:{
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newWidth = Math.max(window.minimumWidth, startWindowSize.width + (abs.x - startMousePos.x))
                window.setGeometry(window.x, window.y, newWidth, window.height)
            }
        }
        onMouseYChanged: {
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newHeigh = Math.max(window.minimumHeight, startWindowSize.height - (abs.y - startMousePos.y))
                var newY = startWindowPos.y - (newHeigh - startWindowSize.height)
                window.setGeometry(window.x, newY, window.width, newHeigh)
            }
        }
    }

    MouseArea {
        id: resizeTopLeft
        enabled: !window.maximalized
        anchors{
            top: parent.top
            left: parent.left
        }
        height: size
        width: size
        property point lastMousePos: Qt.point(0, 0)
        hoverEnabled: true
        onHoveredChanged: cursorShape = (!window.maximalized && (containsMouse || pressed) ?  Qt.SizeFDiagCursor :  Qt.ArrowCursor)
        onPressed: {
            startMousePos = absoluteMousePos(this)
            startWindowPos = Qt.point(window.x, window.y)
            startWindowSize = Qt.size(window.width, window.height)
        }
        onMouseXChanged:{
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newWidth = Math.max(window.minimumWidth, startWindowSize.width - (abs.x - startMousePos.x))
                var newX = startWindowPos.x - (newWidth - startWindowSize.width)
                window.setGeometry(newX, window.y, newWidth, window.height)
            }
        }
        onMouseYChanged: {
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newHeigh = Math.max(window.minimumHeight, startWindowSize.height - (abs.y - startMousePos.y))
                var newY = startWindowPos.y - (newHeigh - startWindowSize.height)
                window.setGeometry(window.x, newY, window.width, newHeigh)
            }
        }
    }

    MouseArea {
        id: resizeTop
        enabled: !window.maximalized
        anchors{
            top: parent.top
            right: resizeTopRight.left
            left: resizeTopLeft.right
        }
        property point lastMousePos: Qt.point(0, 0)
        height: size
        hoverEnabled: true
        onHoveredChanged: cursorShape = (!window.maximalized && (containsMouse || pressed) ?  Qt.SizeVerCursor :  Qt.ArrowCursor)
        onPressed: {
            startMousePos = absoluteMousePos(this)
            startWindowPos = Qt.point(window.x, window.y)
            startWindowSize = Qt.size(window.width, window.height)
        }
        onMouseYChanged: {
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newHeigh = Math.max(window.minimumHeight, startWindowSize.height - (abs.y - startMousePos.y))
                var newY = startWindowPos.y - (newHeigh - startWindowSize.height)
                window.setGeometry(window.x, newY, window.width, newHeigh)
            }
        }
    }

    MouseArea {
        id: resizeLeft
        anchors{
            top: resizeTopLeft.bottom
            left: parent.left
            bottom: resizeBottomLeft.top
        }
        width: size
        property point lastMousePos: Qt.point(0, 0)
        hoverEnabled: true
        onHoveredChanged: cursorShape = (!window.maximalized && (containsMouse || pressed) ?  Qt.SizeHorCursor :  Qt.ArrowCursor)
        onPressed: {
            startMousePos = absoluteMousePos(this)
            startWindowPos = Qt.point(window.x, window.y)
            startWindowSize = Qt.size(window.width, window.height)
        }
        onMouseXChanged:{
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newWidth = Math.max(window.minimumWidth, startWindowSize.width - (abs.x - startMousePos.x))
                var newX = startWindowPos.x - (newWidth - startWindowSize.width)
                window.setGeometry(newX, window.y, newWidth, window.height)
            }
        }
    }

    MouseArea {
        id: resizeBottomLeft
        anchors{
            bottom: parent.bottom
            left: parent.left
        }
        height: size
        width: size
        property point lastMousePos: Qt.point(0, 0)
        hoverEnabled: true
        onHoveredChanged: cursorShape = (!window.maximalized && (containsMouse || pressed) ?  Qt.SizeBDiagCursor :  Qt.ArrowCursor)
        onPressed: {
            startMousePos = absoluteMousePos(this)
            startWindowPos = Qt.point(window.x, window.y)
            startWindowSize = Qt.size(window.width, window.height)
        }
        onMouseXChanged:{
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newWidth = Math.max(window.minimumWidth, startWindowSize.width - (abs.x - startMousePos.x))
                var newX = startWindowPos.x - (newWidth - startWindowSize.width)
                window.setGeometry(newX, window.y, newWidth, window.height)
            }
        }
        onMouseYChanged: {
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newHeigh = Math.max(window.minimumHeight, startWindowSize.height + (abs.y - startMousePos.y))
                window.setGeometry(window.x, window.y, window.width, newHeigh)
            }
        }
    }

    MouseArea {
        id: resizeBottom
        anchors{
            left: resizeBottomLeft.right
            bottom: parent.bottom
            right: resizeBottomRight.left
        }
        property point lastMousePos: Qt.point(0, 0)
        height: size
        hoverEnabled: true
        onHoveredChanged: cursorShape = (!window.maximalized && (containsMouse || pressed) ?  Qt.SizeVerCursor :  Qt.ArrowCursor)
        onPressed: {
            startMousePos = absoluteMousePos(this)
            startWindowSize = Qt.size(window.width, window.height)
        }
        onMouseYChanged: {
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newHeigh = Math.max(window.minimumHeight, startWindowSize.height + (abs.y - startMousePos.y))
                window.setGeometry(window.x, window.y, window.width, newHeigh)
            }
        }
    }

    MouseArea {
        id: resizeBottomRight
        anchors{
            bottom: parent.bottom
            right: parent.right
        }
        height: size
        width: size
        property point lastMousePos: Qt.point(0, 0)
        hoverEnabled: true
        onHoveredChanged: cursorShape = (!window.maximalized && (containsMouse || pressed) ?  Qt.SizeFDiagCursor :  Qt.ArrowCursor)
        onPressed: {
            startMousePos = absoluteMousePos(this)
            startWindowSize = Qt.size(window.width, window.height)
        }
        onMouseXChanged:{
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newWidth = Math.max(window.minimumWidth, startWindowSize.width + (abs.x - startMousePos.x))
                window.setGeometry(window.x, window.y, newWidth, window.height)
            }
        }
        onMouseYChanged: {
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newHeigh = Math.max(window.minimumHeight, startWindowSize.height + (abs.y - startMousePos.y))
                window.setGeometry(window.x, window.y, window.width, newHeigh)
            }
        }
    }

    MouseArea {
        id: resizeRight
        anchors{
            top: resizeTopRight.bottom
            right: parent.right
            bottom: resizeBottomRight.top
        }
        width: size
        property point lastMousePos: Qt.point(0, 0)
        hoverEnabled: true
        onHoveredChanged: cursorShape = (!window.maximalized && (containsMouse || pressed) ?  Qt.SizeHorCursor :  Qt.ArrowCursor)
        onPressed: {
            startMousePos = absoluteMousePos(this)
            startWindowSize = Qt.size(window.width, window.height)
        }
        onMouseXChanged:{
            if(pressed && !window.maximalized) {
                var abs = absoluteMousePos(this)
                var newWidth = Math.max(window.minimumWidth, startWindowSize.width + (abs.x - startMousePos.x))
                window.setGeometry(window.x, window.y, newWidth, window.height)
            }
        }
    }
}
