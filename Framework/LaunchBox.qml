import QtQuick 2.0

Item {

    width: 300;
    height: 200;

    Rectangle{
        x:100;
        y: 50;
        height: 100;
        width: height*2;
        color: "blue";
        TextInput {
            id: elem
            text: qsTr("Hello Nygger");
            font.family: "Arial";
            font.pixelSize: 50;
        }

    }
    Rectangle{
        x:100;
        y: 100;
        z: -1;
        height: 100;
        width: elem.width;
        color: "green";
    }

}
