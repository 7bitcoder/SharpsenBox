import QtQuick 2.12

Rectangle {
    id: gameChoser
    property int size
    // game state id
    property int sylioId: 0
    property int restId: 1
    //state
    property int globSelected: sylioId
    function resetAll(id) {
        globSelected = id
        rest.selected = false
        sylio.selected = false
    }
    //buttons settings/color/opavity etc
    property color normal: "#808080"
    property real clicked: 0.3
    property real hovered: 1
    property real timeHover: 200
    property real timeClicked: 120

    Component{
        id: gameBLock
        GameChoserBlock{ gameId: gId }
    }

    ListModel {
         id: gameBarModel
    }

    ListView {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        clip: true
        height: parent.height
        model: gameBarModel
        id: gameBar
        delegate: gameBLock
        orientation: ListView.Horizontal
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

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
