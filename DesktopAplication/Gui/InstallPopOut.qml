import QtQuick 2.12
import Qt.labs.platform 1.1
import QtQuick.Controls 2.12

Rectangle {
    id: installbar
    implicitHeight: 800
    implicitWidth: 800
    property int gameId: window.gameId
    property int normalTextSize: 15
    property string installDirectory: folderDialog.folder.toString(
                                          ).substring(8)
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
        height: 200
        Rectangle {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
            }
            width: 500
            Text {
                id: installLoc
                anchors {
                    top: parent.top
                    topMargin: 10
                    left: parent.left
                }
                font.family: "Arial"
                font.pixelSize: 25
                color: "black"
                text: "Chose installation paremeters"
            }
            Text {
                id: folder
                anchors {
                    top: installLoc.bottom
                    topMargin: 15
                    left: parent.left
                }
                text: "Folder: "
                font.family: "Arial"
                font.pixelSize: installbar.normalTextSize
            }
            FolderDialog {
                id: folderDialog
                title: "Chose installation folder"
                folder: _Config.gamePath(gameId)
                options: FolderDialog.ShowDirsOnly
                onAccepted: {
                    installDirectory = folderDialog.folder.toString(
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
                    text: installDirectory
                    font.family: "Arial"
                    font.pixelSize: normalTextSize
                    selectByMouse: true
                    onAccepted: installDirectory = text
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
                id: fullPath
                anchors {
                    top: folder.bottom
                    topMargin: 10
                    left: parent.left
                    right: parent.right
                }
                clip: true
                text: "Full Path: " + installDirectory + "/" + "Gra"
                font.family: "Arial"
                font.pixelSize: installbar.normalTextSize
                wrapMode: Text.WrapAnywhere
            }
            CheckBox {
                id: createShortcut
                anchors {
                    top: fullPath.bottom
                    topMargin: 30
                    left: parent.left
                }
            }
            Text {
                text: "Create Desktop Shortcut"
                font.family: "Arial"
                font.pixelSize: normalTextSize
                anchors {
                    verticalCenter: createShortcut.verticalCenter
                    left: createShortcut.right
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
                    _GameInstaller.installGame(installbar.gameId,
                                               installbar.installDirectory,
                                               createShortcut.enabled)
                    window.stage = 0
                }
                contentItem: Text {
                    text: install.text
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
                    window.stage = 0
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
        }
    }
}
