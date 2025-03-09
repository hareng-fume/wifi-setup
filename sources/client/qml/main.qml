import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 360
    height: 480
    title: "WiFi Setup"

    property bool passwordValid: false
    property bool networkSelected: false
    property bool readyToConnect: passwordValid && networkSelected

    Connections {
        target: wifiClient
        function onPasswordValidatedWithResult(success, message) {
            wifiImage.source = success ? "qrc:/resources/icons/wifi/wifi_green_64.png" :
                                         "qrc:/resources/icons/wifi/wifi_red_64.png"
        }
    }

    Connections {
        target: passwordField
        function onPasswordValidityChanged(valid) {
            passwordValid = valid
        }
    }

    header: Rectangle {
        width: parent.width
        height: 50
        color: "#f0f0f0"
        border.color: "#ccc"

        Row {
            width: parent.width
            height: parent.height
            anchors.centerIn: parent

            Text {
                text: "WiFi Setup"
                font.pointSize: 20
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Image {
                id: wifiImage
                source: "qrc:/resources/icons/wifi/wifi_default_64.png"
                width: 40
                height: 40
                x: parent.width-2*width
                anchors.verticalCenter: parent.verticalCenter
            }
        }

    } // header

    Column {
        width: parent.width
        spacing: 10
        padding: 10

        // WiFi network ComboBox Section
        Column {
            width: parent.width * 0.8
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "WiFi Network"
                font.bold: true
            }

            ComboBox {
                id: networkSelector
                width: parent.width
                model: wifiClient.networkModel
                textRole: "display"
                font.pointSize: 14

                contentItem: Text {
                    text: parent.displayText
                    leftPadding: 2  // Padding only for the selected item
                    verticalAlignment: Text.AlignVCenter
                }

                onCurrentIndexChanged: {
                    if (networkSelector.currentIndex !== -1 && !networkSelected)
                        networkSelected = true
                }
            }

            Component.onCompleted: {
                wifiClient.requestNetworkList(); // automatically request networks on start
            }
        }

        // Password TextField Section
        Column {
            width: parent.width * 0.8
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: "Enter text:"
                font.bold: true
            }

            PasswordField {
                id: passwordField
                width: parent.width
            }


        }
    }

    footer: Rectangle {
        width: parent.width
        height: 50
        color: "#e0e0e0"
        border.color: "#bbb"

        Row {
            width: parent.width
            height: parent.height

            Image {
                id: userImage
                source: "qrc:/resources/icons/user/user_64.png"
                x: connectButton.x - width-10
                width: 35
                height: 35
                fillMode: Image.PreserveAspectFit
                anchors.verticalCenter: parent.verticalCenter
                opacity: readyToConnect ? 1.0 : 0.5
            }

            Button {
                id: connectButton
                enabled: readyToConnect
                text: "Connect"
                width: 80
                height: userImage.height
                x: parent.width-width-10
                anchors.verticalCenter: parent.verticalCenter

                background: Rectangle {
                    color: "white"
                    //border.color: "#0078D7"  // Blue border
                    border.color: "grey"
                    border.width: 1
                    radius: 8  // Rounded corners
                    layer.enabled: true
                }

                onClicked: wifiClient.connectToNetwork(networkSelector.currentText
                                                       , passwordField.password)
            }
        } // Row

    } // footer



} // ApplicationWindow
