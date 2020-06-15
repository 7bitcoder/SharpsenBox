import QtQuick 2.12
import QtQuick.Controls 2.15

Item {
    ScrollView {
        id: scroll
        anchors {
            fill: parent
            margins: 20
            rightMargin: 100
        }
        contentHeight: content.contentHeigh
        property int main: 15
        property int inner: 30
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        clip: true
        Rectangle {
            id: content
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            property int contentHeigh: gamesList.y + gamesList.contentHeight - content.y

            color: "transparent"

            Item {
                anchors.fill: parent
                anchors.leftMargin: 30
                Text {
                    id: mainSettings
                    anchors {
                        top: parent.top
                        topMargin: 15
                        left: parent.left
                    }
                    text: "Launcher Settings"
                    font.family: "Arial"
                    color: "white"
                    font.pixelSize: 25
                }

                Text {
                    anchors {
                        top: mainSettings.bottom
                        topMargin: 20
                        left: parent.left
                        leftMargin: scroll.inner
                    }
                    id: speedInfo
                    text: qsTr(
                              "Maximum download speed, 0 is treat as unlimited")
                    font.family: "Arial"
                    font.pixelSize: 20
                    color: "white"
                }
                Rectangle {
                    id: speedInput
                    anchors {
                        top: speedInfo.bottom
                        topMargin: 10
                        left: parent.left
                        leftMargin: scroll.inner
                    }
                    height: 28
                    width: 200
                    color: "transparent"
                    border {
                        color: "grey"
                        width: 3
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
                        text: _Config.getDownloadSpeed()
                        font.family: "Arial"
                        color: "white"
                        font.pixelSize: 20
                        selectByMouse: true
                        onAccepted: {
                            var x = Number(textInput.text)
                            if (isNaN(x)) {
                                parseError.text = "Value should be number"
                            } else {
                                var n = Math.floor(x)
                                if (n < 0)
                                    n = 0
                                if (n > 2147483647)
                                    n = 2147483647
                                textInput.text = n.toString()
                                parseError.text = ""
                                _Config.setDownloadSpeed(n)
                            }
                        }
                    }
                    Text {
                        id: kb
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: textInput.right
                            leftMargin: 10
                        }
                        clip: true
                        text: "KB/s"
                        font.family: "Arial"
                        color: "White"
                        font.pixelSize: 20
                    }
                    Text {
                        id: parseError
                        anchors {
                            top: textInput.bottom
                            topMargin: 2
                            left: textInput.left
                        }
                        clip: true
                        visible: text != ""
                        text: ""
                        font.family: "Arial"
                        color: "red"
                        font.pixelSize: 15
                    }
                }

                CheckBox {
                    id: autostart
                    anchors {
                        top: speedInput.bottom
                        topMargin: 15
                        left: parent.left
                        leftMargin: scroll.inner
                    }
                }
                Text {
                    text: "Run launcher at system startup"
                    font.family: "Arial"
                    font.pixelSize: 20
                    color: "white"
                    anchors {
                        verticalCenter: autostart.verticalCenter
                        left: autostart.right
                        leftMargin: 10
                    }
                }

                Text {
                    id: gameSettingsS
                    anchors {
                        top: autostart.bottom
                        topMargin: 15
                        left: parent.left
                    }
                    text: "Games Settings"
                    font.family: "Arial"
                    color: "white"
                    font.pixelSize: 25
                }

                Component{
                    id: gameBLock
                    GameSettings{ gameId: gId; }
                }

                ListModel {
                     id: gameBarModel
                }

                ListView {
                    id: gamesList
                    anchors {
                        top: gameSettingsS.bottom
                        topMargin: 15
                        left: parent.left
                        leftMargin: scroll.inner
                    }
                    height: contentHeight
                    width: 600
                    model: gameBarModel
                    delegate: gameBLock
                    spacing: 10
                }

                Component.onCompleted: {
                    console.log("here")
                    while (true) {
                        var gameId = _Config.getGameId()
                        //end
                        console.log("gameID " + gameId)
                        if (gameId === 0){
                            console.log("break")
                            break
                        }
                        gameBarModel.append({ gId: gameId })
                        console.log("size" + gameBarModel.count)
                    }
                }
            }
        }
    }
}
