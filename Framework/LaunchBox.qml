import QtQuick 2.0
import "content"
import "content/calculator.js" as CalcEngine
import QtQuick.Controls 2.13

Rectangle {
    id: window
    width: 1100
    height: 700
    focus: true
    color: "#1e1e1e"

    onWidthChanged: controller.reload()
    onHeightChanged: controller.reload()

    function operatorPressed(operator) {
        CalcEngine.operatorPressed(operator)
        numPad.buttonPressed()
    }
    function digitPressed(digit) {
        CalcEngine.digitPressed(digit)
        numPad.buttonPressed()
    }
    function isButtonDisabled(op) {
        return CalcEngine.disabled(op)
    }

    Item {
        id: pad
        width: 180
    }

    AnimationController {
        id: controller
        animation: ParallelAnimation {
            id: anim
            NumberAnimation { target: display; property: "x"; duration: 400; from: -16; to: window.width - display.width; easing.type: Easing.InOutQuad }
            NumberAnimation { target: pad; property: "x"; duration: 400; from: window.width - pad.width; to: 0; easing.type: Easing.InOutQuad }
            SequentialAnimation {
                NumberAnimation { target: pad; property: "scale"; duration: 200; from: 1; to: 0.97; easing.type: Easing.InOutQuad }
                NumberAnimation { target: pad; property: "scale"; duration: 200; from: 0.97; to: 1; easing.type: Easing.InOutQuad }
            }
        }
    }

    Button {
        id: button
        x: 80
        y: 481
        width: 80
        height: 40
        text: qsTr("PLAY")
        font.pointSize: 13
    }

    Button {
        id: button1
        x: 272
        y: 481
        text: qsTr("Button")
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_0)
            digitPressed("0")
        else if (event.key == Qt.Key_1)
            digitPressed("1")
        else if (event.key == Qt.Key_2)
            digitPressed("2")
        else if (event.key == Qt.Key_3)
            digitPressed("3")
        else if (event.key == Qt.Key_4)
            digitPressed("4")
        else if (event.key == Qt.Key_5)
            digitPressed("5")
        else if (event.key == Qt.Key_6)
            digitPressed("6")
        else if (event.key == Qt.Key_7)
            digitPressed("7")
        else if (event.key == Qt.Key_8)
            digitPressed("8")
        else if (event.key == Qt.Key_9)
            digitPressed("9")
        else if (event.key == Qt.Key_Plus)
            operatorPressed("+")
        else if (event.key == Qt.Key_Minus)
            operatorPressed("−")
        else if (event.key == Qt.Key_Asterisk)
            operatorPressed("×")
        else if (event.key == Qt.Key_Slash)
            operatorPressed("÷")
        else if (event.key == Qt.Key_Enter || event.key == Qt.Key_Return)
            operatorPressed("=")
        else if (event.key == Qt.Key_Comma || event.key == Qt.Key_Period)
            digitPressed(".")
        else if (event.key == Qt.Key_Backspace)
            operatorPressed("backspace")
    }

}

/*##^##
Designer {
    D{i:12;flowX:-338;flowY:146}
}
##^##*/
