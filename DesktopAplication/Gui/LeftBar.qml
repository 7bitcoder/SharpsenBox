import QtQuick 2.12

Rectangle {
    Item {
        id: accessButtons
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
            topMargin: 30
            leftMargin: 30
        }
        // readonly enum values
        property int home_: 1
        property int settings_: 2
        property int account_: 3
        // actual state
        property int state: home_
        // buttons opacity
        property real normal: 0.6
        property real clicked: 0.4
        property real hovered: 1
        property real timeHover: 200
        property real timeClicked: 80

        function changeMenu(where) {
            state = where
            switch (state) {
            case home_:
                //load new stage
                settings.loc = false
                account.loc = false
                break
            case settings_:
                //load new stage
                home.loc = false
                account.loc = false
                break
            case account_:
                //load new stage
                home.loc = false
                settings.loc = false
                break
            }
        }

        Item {
            id: home
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: homeImage.height
            property real op: accessButtons.normal
            property bool timeType: false
            property bool loc: false
            onLocChanged: {
                if (loc === true) {
                    home.timeType = false
                    home.op = accessButtons.hovered
                } else {
                    home.timeType = true
                    home.op = accessButtons.normal
                }
            }
            Image {
                id: homeImage
                source: "content/house.png"
                height: 30
                width: 30
                opacity: parent.op
            }
            Text {
                id: homeText
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: homeImage.right
                    leftMargin: 8
                }
                text: qsTr("Home")
                font.pointSize: 15
                color: "white"
                opacity: parent.op
            }
            MouseArea {
                id: homeArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    home.loc = true
                    home.timeType = false
                    home.op = accessButtons.hovered
                    accessButtons.changeMenu(accessButtons.home_)
                }
                onPressed: {
                    if (home.loc === false) {
                        home.timeType = false
                        home.op = accessButtons.clicked
                    }
                }
                onReleased: {
                    if (home.loc === false) {
                        home.timeType = false
                        home.op = containsMouse ? accessButtons.hovered : accessButtons.normal
                    }
                }
                onEntered: {
                    if (home.loc === false) {
                        home.timeType = true
                        home.op = pressed ? parent.op : accessButtons.hovered
                    }
                }
                onExited: {
                    if (home.loc === false) {
                        home.timeType = true
                        home.op = pressed ? parent.op : accessButtons.normal
                    }
                }
            }

            Behavior on op {
                NumberAnimation {
                    duration: home.timeType
                              === true ? accessButtons.timeHover : accessButtons.timeClicked
                }
            }
        }
        Item {
            anchors {
                top: home.bottom
                left: parent.left
                right: parent.right
            }
            anchors.topMargin: 7
            id: account
            height: accountImage.height
            property real op: accessButtons.normal
            property bool timeType: false
            property bool loc: false
            onLocChanged: {
                if (loc === true) {
                    account.timeType = false
                    account.op = accessButtons.hovered
                } else {
                    account.timeType = true
                    account.op = accessButtons.normal
                }
            }

            Image {
                id: accountImage
                source: "content/person.png"
                height: 30
                width: 30
                opacity: parent.op
            }
            Text {
                id: accountText
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: accountImage.right
                    leftMargin: 8
                }
                text: qsTr("Account")
                font.pointSize: 15
                color: "white"
                opacity: parent.op
            }
            MouseArea {
                id: accountArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    account.loc = true
                    accessButtons.changeMenu(accessButtons.account_)
                }
                onPressed: {
                    if (account.loc === false) {
                        account.timeType = false
                        account.op = accessButtons.clicked
                    }
                }
                onReleased: {
                    if (account.loc === false) {
                        account.timeType = false
                        account.op = containsMouse ? accessButtons.hovered : accessButtons.normal
                    }
                }
                onEntered: {
                    if (account.loc === false) {
                        account.timeType = true
                        account.op = pressed ? parent.op : accessButtons.hovered
                    }
                }
                onExited: {
                    if (account.loc === false) {
                        account.timeType = true
                        account.op = pressed ? parent.op : accessButtons.normal
                    }
                }
            }

            Behavior on op {
                NumberAnimation {
                    duration: account.timeType
                              === true ? accessButtons.timeHover : accessButtons.timeClicked
                }
            }
        }
        Item {
            anchors {
                top: account.bottom
                left: parent.left
                right: parent.right
            }
            anchors.topMargin: 7
            id: settings
            height: settingsImage.height
            property real op: accessButtons.normal
            property bool timeType: false
            property bool loc: false
            onLocChanged: {
                if (loc === true) {
                    settings.timeType = false
                    settings.op = accessButtons.hovered
                } else {
                    settings.timeType = true
                    settings.op = accessButtons.normal
                }
            }
            Image {
                id: settingsImage
                source: "content/cog.png"
                height: 30
                width: 30
                opacity: parent.op
            }
            Text {
                id: settingsText
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: settingsImage.right
                    leftMargin: 8
                }
                text: qsTr("Settings")
                font.pointSize: 15
                color: "white"
                opacity: parent.op
            }
            MouseArea {
                id: settingsArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    settings.loc = true
                    settings.timeType = false
                    settings.op = accessButtons.hovered
                    accessButtons.changeMenu(accessButtons.settings_)
                }
                onPressed: {
                    if (settings.loc === false) {
                        settings.timeType = false
                        settings.op = accessButtons.clicked
                    }
                }
                onReleased: {
                    if (settings.loc === false) {
                        settings.timeType = false
                        settings.op = containsMouse ? accessButtons.hovered : accessButtons.normal
                    }
                }
                onEntered: {
                    if (settings.loc === false) {
                        settings.timeType = true
                        settings.op = pressed ? parent.op : accessButtons.hovered
                    }
                }
                onExited: {
                    if (settings.loc === false) {
                        settings.timeType = true
                        settings.op = pressed ? parent.op : accessButtons.normal
                    }
                }
            }

            Behavior on op {
                NumberAnimation {
                    duration: settings.timeType
                              === true ? accessButtons.timeHover : accessButtons.timeClicked
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

