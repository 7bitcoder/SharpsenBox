import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQml.Models 2.12
import Qt.labs.folderlistmodel 2.11

Rectangle {
    id: windows
    color: "transparent"
    property int packetCnt: 1
    Component{
        id: element
        Rectangle{
            color: "green"
            height: 50
            width: 300
        }
    }

    ListModel {
         id: elementList
    }

    Rectangle {
        anchors {
            right: parent.horizontalCenter
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: 50
        }
        color: "white"

        TreeView {
            id: treeview
            property  int dragItemIndex: -1
            anchors.fill: parent
            model: _left
            selectionMode: SelectionMode.ExtendedSelection
               selection: ItemSelectionModel {
                   model: treeview.model
               }
            // broken due to MouseArea in itemDelegate !
            TableViewColumn {
                role: "display" //this will show the folder/file name from the model
            }
            itemDelegate: Item {
                   Rectangle {
                       id: rect
                       anchors.left: parent.left
                       anchors.verticalCenter: parent.verticalCenter
                       height: 20
                       width: 100
                       color: Drag.active ? "lightgreen" : "transparent"
                       Text {
                           anchors.verticalCenter: parent.verticalCenter
                           color: styleData.selected ? "white" : "black"
                           text: styleData.value
                       }

                       Drag.active: mouseArea.drag.active
                       Drag.hotSpot.x: width / 2
                       Drag.hotSpot.y: height / 2

                       MouseArea {
                           id: mouseArea
                           anchors.fill: parent
                           drag.target: rect

                           drag.onActiveChanged: {
                               if (mouseArea.drag.active) {
                                   //treeview.dragItemIndex = index;
                               }
                               console.log(styleData.row)
                               rect.Drag.drop();
                           }
                           onClicked: treeview.model.removeRows(styleData.row, 1, styleData.index)
                       }
                       states: [
                            State {
                                when: rect.Drag.active

                                ParentChange {
                                    target: rect
                                    parent: windows
                                }
                                AnchorChanges {
                                    target: rect
                                    anchors.left: undefined
                                    anchors.verticalCenter: undefined
                                }
                            }
                        ]
                      //MouseArea {
                      //    anchors.fill: parent
                      //    drag.target: symbolAvatar
                      //    onPressed: {
                      //        var tmp = mapToItem(container, mouse.x, mouse.y);
                      //        symbolAvatar.x = tmp.x;
                      //        symbolAvatar.y = tmp.y;
                      //        symbolAvatar.dragging = true;
                      //        symbolAvatar.text = styleData.value;
                      //    }
                      //}
                   }
               }
        }

    }

    MyButton {
        anchors{
         bottom: packeter.top
         right: packeter.right
        }
            onClicked: {
                packetModel.append({})
                windows.packetCnt = windows.packetCnt + 1
            }
            text: "Add Packet"
    }

    Rectangle {
        id: packeter
        anchors {
            left: parent.horizontalCenter
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            margins: 50
        }
        color: "white"


        Component{
            id: packet
            Item {
                id: packetIt
                //implicitHeight: name.implicitHeight + treeviewL.implicitHeight
                //implicitWidth: name.implicitWidth
                property bool show: false
                implicitHeight: name.height + treeviewL.height
                width: parent.width
                Rectangle{
                    id: rect
                    anchors{
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }
                     implicitHeight: name.implicitHeight
                    MouseArea{
                        anchors.fill: parent
                        onClicked: show = !show
                    }

                    TextInput {
                        id: name
                        anchors.left: parent.left
                        anchors.top: parent.top
                        font.pixelSize: 15
                        height: font.pixelSize
                        text: "pcaket" + packetCnt + ".zip"
                    }
                }
                DropArea {
                  id:setL
                  anchors.fill: parent
                    onDropped: {
                       console.log(styleData.index)
                    }
                }

                TreeView {
                    anchors{
                        left: parent.left
                        right: parent.right
                        top: rect.bottom
                    }
                    height: packetIt.show ? 300 : 0
                    id: treeviewL
                    property  int dragItemIndex: -1
                    model: _left
                    selectionMode: SelectionMode.ExtendedSelection
                       selection: ItemSelectionModel {
                           model: treeview.model
                       }
                    // broken due to MouseArea in itemDelegate !
                    TableViewColumn {
                        role: "display" //this will show the folder/file name from the model
                    }
                    itemDelegate: Item {
                           Rectangle {
                               id: rectL
                               anchors.left: parent.left
                               anchors.verticalCenter: parent.verticalCenter
                               height: 20
                               width: 100
                               color: Drag.active ? "lightgreen" : "transparent"
                               Text {
                                   anchors.verticalCenter: parent.verticalCenter
                                   color: styleData.selected ? "white" : "black"
                                   text: styleData.value
                               }

                               Drag.active: mouseAreaL.drag.active
                               Drag.hotSpot.x: width / 2
                               Drag.hotSpot.y: height / 2

                               MouseArea {
                                   id: mouseAreaL
                                   anchors.fill: parent
                                   drag.target: rectL

                                   drag.onActiveChanged: {
                                       if (mouseAreaL.drag.active) {
                                           //treeview.dragItemIndex = index;
                                       }
                                       console.log(styleData.row)
                                       rectL.Drag.drop();
                                   }
                                   onClicked: treeviewL.model.removeRows(styleData.row, 1, styleData.index)
                               }

                               states: [
                                    State {
                                        when: rectL.Drag.active

                                        ParentChange {
                                            target: rectL
                                            parent: windows
                                        }
                                        AnchorChanges {
                                            target: rectL
                                            anchors.left: undefined
                                            anchors.verticalCenter: undefined
                                        }
                                    }
                                ]
                           }
                       }
                }
            }

        }

        ListModel {
             id: packetModel
        }

        ListView {
            id: packetList
            anchors.fill: parent

            model: packetModel
            delegate: packet
            spacing: 10
        }


    }

}
