import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQml.Models 2.12
import Qt.labs.folderlistmodel 2.11

Rectangle {
    id: windows
    color: "transparent"

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
            model: _TreeModel
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
                               treeview.model.gee(styleData.index)
                               console.log(treeview.currentIndex)
                               rect.visible = false
                               rect.Drag.drop();
                           }
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

        DropArea {
           id:set

           anchors{
            verticalCenter: treeview.verticalCenter
            left: treeview.right
            rightMargin: 50
           }
            width: 50; height: 50

            Rectangle {
                id: nyg
                anchors.fill: parent
                color: "green"

            }
            onDropped: {
               nyg.color = "yellow"
            }
        }

       // DropArea {
       //     id: dragTarget
       //
       //     property string colorKey
       //     property alias dropProxy: dragTarget
       //
       //     width: 64; height: 64
       //     keys: [ colorKey ]
       //
       //     Rectangle {
       //         id: dropRectangle
       //
       //         anchors.fill: parent
       //         color: dragTarget.colorKey
       //
       //         states: [
       //             State {
       //                 when: dragTarget.containsDrag
       //                 PropertyChanges {
       //                     target: dropRectangle
       //                     color: "grey"
       //                 }
       //             }
       //         ]
       //     }
       // }
    }
}
