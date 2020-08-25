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
    property color background: "#292929"
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

    Rectangle {
        color: gameChoser.background
        opacity: 0.85
        height: parent.height
        anchors{
            top: parent.top
            left: parent.left
            right: leftTerm.right
        }
    }

    Component {
        id: leftTerm
        Rectangle {
            color: gameChoser.background
            opacity: 0.85
            height: parent.height
            width: 20
        }
    }

    Component {
        id: rightTerm
        Rectangle {
            color: gameChoser.background
            opacity: 0.85
            height: parent.height
            width: 20
        }
    }

    ListView {
        id: gameBar
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        clip: true
        height: parent.height
        model: gameBarModel
        delegate: gameBLock
        orientation: ListView.Horizontal
        //header: leftTerm
        //footer: rightTerm
    }

    Component.onCompleted: {
        while (true) {
            var gameId = _Config.getGameId()
            //end
            if (gameId === 0){
                break
            }
            gameBarModel.append({ gId: gameId })
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
