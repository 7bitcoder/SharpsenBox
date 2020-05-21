import QtQuick 2.14

Item {
    id: frame
    property Item content
    property int size
    MouseArea {
        enabled: !window.maximalized
        id: resizeTopRight
        anchors{
            top: parent.top
            right: parent.right
        }
        height: size
        width: size
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
        enabled: !window.maximalized
        anchors{
            top: resizeTopLeft.bottom
            left: parent.left
            bottom: resizeBottomLeft.top
        }
        width: size
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
        enabled: !window.maximalized
        anchors{
            bottom: parent.bottom
            left: parent.left
        }
        height: size
        width: size
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
        enabled: !window.maximalized
        anchors{
            left: resizeBottomLeft.right
            bottom: parent.bottom
            right: resizeBottomRight.left
        }
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
        enabled: !window.maximalized
        anchors{
            bottom: parent.bottom
            right: parent.right
        }
        height: size
        width: size
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
        enabled: !window.maximalized
        anchors{
            top: resizeTopRight.bottom
            right: parent.right
            bottom: resizeBottomRight.top
        }
        width: size
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
