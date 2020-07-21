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
        property int fontSize: 22
        property int home_: 1
        property int settings_: 2
        property int account_: 3
        // actual state
        property int state: home_
        // buttons opacity/timers
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
                contentLoader.source = "Home.qml"
                settings.loc = false
                account.loc = false
                break
            case settings_:
                //load new stage
                contentLoader.source = "Settings.qml"
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
                source: "Icons/house.png"
                height: 30
                width: 30
                opacity: parent.op
            }
            Text {
                id: homeText
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: homeImage.right
                    leftMargin: 20
                }
                text: qsTr("Home")
                font.pixelSize: accessButtons.fontSize
                font.family: latoFont.name
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
                topMargin: 30
            }
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
                source: "Icons/person.png"
                height: 30
                width: 30
                opacity: parent.op
            }
            Text {
                id: accountText
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: accountImage.right
                    leftMargin: 20
                }
                text: qsTr("Account")
                font.family: latoFont.name
                font.pixelSize: accessButtons.fontSize
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
                topMargin: 30
            }
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
                source: "Icons/cog.png"
                height: 30
                width: 30
                opacity: parent.op
            }
            Text {
                id: settingsText
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: settingsImage.right
                    leftMargin: 20
                }
                text: qsTr("Settings")
                font.pixelSize: accessButtons.fontSize
                font.family: latoFont.name
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

