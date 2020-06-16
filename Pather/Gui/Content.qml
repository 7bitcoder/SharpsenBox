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
    Button {
        id: choseFolder
        y: 0
        width: 100
        anchors {
            verticalCenter: pathDisplay.verticalCenter
        }
        height: 30
        text: "Chose"
        anchors.top: parent.top
        anchors.left: pathDisplay.right
        anchors.leftMargin: 6
        onClicked: folderDialog.open()
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
