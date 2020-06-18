import QtQuick 2.12
import QtQuick.Controls 2.12


Item {
    id: element

    Rectangle {
        id: pathDisplay
        anchors {
            top: parent.top
            left: parent.left
        }
        height: 28
        width: 241
        border {
            color: "grey"
            width: 2
        }
        radius: 3
        TextInput {
            id: textInput
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 10
                right: parent.right
                rightMargin: 4
            }
            clip: true
            text: gameDir
            font.family: "Arial"
            font.pixelSize: 14
            selectByMouse: true
            onAccepted: installDirectory = text
        }
    }

    MyButton {
            onClicked: folderDialog.open()
            text: "Chose"
    }

    PatherWindows{
        anchors{
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: pathDisplay.bottom
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:4;anchors_width:100;anchors_x:247}
}
##^##*/
