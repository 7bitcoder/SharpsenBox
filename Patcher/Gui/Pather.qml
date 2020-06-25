import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Window 2.12
import QtWebEngine 1.8
import QtQuick.Layouts 1.11
//import Qt.labs.platform 1.1

ApplicationWindow {
    id: window
    visible: true
    width: 1000
    height: 500
    color: "#202020"
    minimumWidth: 1000
    minimumHeight: 600
    // for window movement
    property bool maximalized: false
    property point startMousePos
    property point startWindowPos
    property size startWindowSize
    property int stage: 0

    property string gameDir: ""

    menuBar: MenuBar {
        Menu {
            title: "Project"
            MenuItem { text: "New"
            onTriggered: gameChoserLoader.source = "InstallPopOut.qml"
            }
            MenuItem { text: "Open..."

            onTriggered: gameChoserLoader.source = "OpenProject.qml"
            }
            MenuItem {
                text: "Close"
                onTriggered: Qt.quit()
            }
        }
    }

   //FolderDialog {
   //    id: folderDialog
   //    title: "Chose installation folder"
   //    options: FolderDialog.ShowDirsOnly
   //    onAccepted: {
   //        gameDir = folderDialog.folder.toString().substring(8)
   //    }
   //}
    Loader {
        id: gameChoserLoader
        anchors.fill: parent
        source: "Empty.qml"
    }
}

