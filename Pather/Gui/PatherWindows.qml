import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQml.Models 2.15
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
            TableViewColumn {
                role: "name_role"
                title: "Name"
                width: 160
            }
            TableViewColumn {
                role: "type_role"
                title: "Type"
                width: 75
            }
            itemDelegate: Item {
                   Rectangle {
                       id: rect
                       anchors.fill: parent
                       color: styleData.selected ? "blue" : "transparent"
                       Text {
                           anchors.verticalCenter: parent.verticalCenter
                           color: styleData.selected ? "white" : "black"
                           text: styleData.value
                       }
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
            id: dragTarget

            property string colorKey
            property alias dropProxy: dragTarget

            width: 64; height: 64
            keys: [ colorKey ]

            Rectangle {
                id: dropRectangle

                anchors.fill: parent
                color: dragTarget.colorKey

                states: [
                    State {
                        when: dragTarget.containsDrag
                        PropertyChanges {
                            target: dropRectangle
                            color: "grey"
                        }
                    }
                ]
            }
        }
    }
}
