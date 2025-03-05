import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 360
    height: 640
    title: "WiFi Setup"

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
                //model: ["Option 1", "Option 2", "Option 3"]
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
                source: "qrc:/resources/icons/user/user_64.png"
                x: connectButton.x - width-10
                width: 35
                height: 35
                fillMode: Image.PreserveAspectFit
                anchors.verticalCenter: parent.verticalCenter
            }

            Button {
                id: connectButton
                text: "Connect"
                width: 80
                height: parent.height-10
                x: parent.width-width-10
                anchors.verticalCenter: parent.verticalCenter
            }
        } // Row

    } // footer

} // ApplicationWindow
