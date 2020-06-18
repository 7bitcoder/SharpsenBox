import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    property string txt
    id: choseFolder
    y: 0
    contentItem: Text {
        text: choseFolder.text
        font.family: "Arial"
        font.pixelSize: 22
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
    background: Rectangle {
        implicitWidth: 50
        implicitHeight: 25
        opacity: choseFolder.down ? 0.7 : 1
        radius: 5
        color: "green"
        Behavior on opacity {
            NumberAnimation {
                duration: 100
            }
        }
    }
}
