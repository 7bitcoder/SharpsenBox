import QtQuick 2.12
import Qt.labs.platform 1.1
import QtQuick.Controls 2.12

Rectangle {
    id: installbar
    implicitHeight: 800
    implicitWidth: 800
    property int normalTextSize: 15
    property string projectDir: folderDialog.folder.toString(
                                          ).substring(8)

    property string gameDir: gamefolderDialog.folder.toString(
                                          ).substring(8)

    property string projectNameStr: ""
    property string gameNameStr: ""

    Rectangle {
        anchors.fill: parent
        opacity: 0.5
        color: "grey"
    }
    MouseArea {
        anchors.fill: parent
    }
    Rectangle {
        color: "white"
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            right: parent.right
        }
        height: 300
        Rectangle {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }
            width: 500
            Text {
                id: installLoc
                width: 400
                height: 28
                anchors {
                    top: parent.top
                    topMargin: 10
                    left: parent.left
                }
                font.family: "Arial"
                font.pixelSize: 25
                color: "black"
                text: "Project Parameters"
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: folder
                anchors {
                    top: installLoc.bottom
                    topMargin: 15
                    left: parent.left
                }
                text: "Project Directory"
                font.family: "Arial"
                font.pixelSize: installbar.normalTextSize
            }
            FolderDialog {
                id: folderDialog
                title: "Chose installation folder"
                options: FolderDialog.ShowDirsOnly
                onAccepted: {
                    projectDir = folderDialog.folder.toString(
                                ).substring(8)
                }
            }
            Rectangle {
                id: pathDisplay
                anchors {
                    verticalCenter: folder.verticalCenter
                    right: choseFolder.left
                    left: folder.right
                }
                height: 28
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
                    text: projectDir
                    font.family: "Arial"
                    font.pixelSize: normalTextSize
                    selectByMouse: true
                    onEditingFinished:  projectDir = text
                }
            }
            Button {
                id: choseFolder
                anchors {
                    verticalCenter: folder.verticalCenter
                    right: parent.right
                }
                height: 30
                width: 100
                text: "Chose"
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

            Text {
                id: gamefolder
                anchors {
                    top: pathDisplay.bottom
                    topMargin: 15
                    left: parent.left
                }
                text: "Game Directory"
                font.family: "Arial"
                font.pixelSize: installbar.normalTextSize
            }
            FolderDialog {
                id: gamefolderDialog
                title: "Chose installation folder"
                options: FolderDialog.ShowDirsOnly
                onAccepted: {
                    gameDir = gamefolderDialog.folder.toString(
                                ).substring(8)
                }
            }
            Rectangle {
                id: gamepathDisplay
                anchors {
                    verticalCenter: gamefolder.verticalCenter
                    right: gamechoseFolder.left
                    left: gamefolder.right
                }
                height: 28
                border {
                    color: "grey"
                    width: 2
                }
                radius: 3
                TextInput {
                    id: gametextInput
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
                    font.pixelSize: normalTextSize
                    selectByMouse: true
                    onEditingFinished:  gameDir = text
                }
            }
            Button {
                id: gamechoseFolder
                anchors {
                    verticalCenter: gamefolder.verticalCenter
                    right: parent.right
                }
                height: 30
                width: 100
                text: "Chose"
                onClicked: gamefolderDialog.open()
                contentItem: Text {
                    text: gamechoseFolder.text
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
                    opacity: gamechoseFolder.down ? 0.7 : 1
                    radius: 5
                    color: "green"
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }


            Text {
                id: gameName
                anchors {
                    top: gamefolder.bottom
                    topMargin: 15
                    left: parent.left
                }
                text: "Game Name"
                font.family: "Arial"
                font.pixelSize: installbar.normalTextSize
            }
            Rectangle {
                id: gameNameR
                anchors {
                    verticalCenter: gameName.verticalCenter
                    left: gameName.right
                    right: choseFolder.left
                }
                height: 28
                border {
                    color: "grey"
                    width: 2
                }
                radius: 3
                TextInput {
                    id: gameNameT
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 10
                        right: parent.right
                        rightMargin: 4
                    }
                    clip: true
                    text: ""
                    font.family: "Arial"
                    font.pixelSize: normalTextSize
                    selectByMouse: true
                    onEditingFinished: gameNameStr = text
                }
            }

            Text {
                id: projectName
                anchors {
                    top: gameName.bottom
                    topMargin: 15
                    left: parent.left
                }
                text: "Project Name"
                font.family: "Arial"
                font.pixelSize: installbar.normalTextSize
            }
            Rectangle {
                id: projectNameR
                anchors {
                    verticalCenter: projectName.verticalCenter
                    left: projectName.right
                    right: choseFolder.left
                }
                height: 28
                border {
                    color: "grey"
                    width: 2
                }
                radius: 3
                TextInput {
                    id: projectNameT
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 10
                        right: parent.right
                        rightMargin: 4
                    }
                    clip: true
                    text: ""
                    font.family: "Arial"
                    font.pixelSize: normalTextSize
                    selectByMouse: true
                    onEditingFinished: projectNameStr = text
                }
            }

            Button {
                id: install
                y: 160
                anchors {
                    bottom: parent.bottom
                    right: cancel.left
                    rightMargin: 5
                    bottomMargin: 10
                }
                height: 30
                width: 100
                text: "Install"
                onClicked: {
                    if( !_Config.exists(gameDir) ){
                        error.what = "Game directory does not exists"
                    } else if( !_Config.exists(projectDir) ){
                        error.what = "Project directory does not exists"
                    } else if( gameNameStr === "" ){
                        error.what = "Game name is empty"
                    } else if( projectNameStr === ""  ){
                        error.what = "Project name is empty"
                    } else {
                        _Project.setGameDir(gameDir)
                        _Project.setProjectDir(projectDir)
                        _Project.setGameName(gameNameStr)
                        _Project.setProjectName(projectNameStr)
                        gameChoserLoader.source = "Content.qml"
                    }
                }
                contentItem: Text {
                    font.family: "Arial"
                    font.pixelSize: 22
                    color: "white"
                    text: "Create"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                background: Rectangle {
                    implicitWidth: 50
                    implicitHeight: 25
                    opacity: install.down ? 0.7 : 1
                    radius: 5
                    color: "green"
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }
            Button {
                id: cancel
                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    bottomMargin: 10
                }
                height: 30
                width: 100
                text: "Cancel"
                onClicked: {
                    gameChoserLoader.source = "Empty.qml"
                }

                contentItem: Text {
                    text: cancel.text
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
                    opacity: cancel.down ? 0.7 : 1
                    radius: 5
                    color: "grey"
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }
            Text {
                anchors{
                    left: parent.left
                    top: projectName.bottom
                    topMargin: 30
                }
                color: "red"
                id: error
                property string what: ""
                text: what
                visible: what !== ""
                font.family: "Arial"
                font.pixelSize: 22
            }
        }
    }
}
