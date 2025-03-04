import QtQuick
import QtQuick.Controls

Rectangle {
    id: container
    width: 250
    height: 40
    radius: 5
    border.color: "#ccc"
    color: "white"

    property alias text: inputField.text
    property alias placeholderText: inputField.placeholderText

    Row {
        anchors.fill: parent
        spacing: 5

        TextField {
            id: inputField
            width: parent.width - eyeButton.width - 10
            height: parent.height
            placeholderText: "16 charachter minimum"
            color: "black"
            placeholderTextColor: "gray"
            echoMode: TextInput.Password // initially hidden
            clip: true

            // align placeholder text
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            background: Rectangle {
                color: "transparent"  // remove background
                border.width: 0       // remove border
            }

            // ensure minimum 16 characters
            onTextChanged: {
                if (text.length < 16) {
                    color = "red"
                } else {
                    color = "black"
                }
            }
        } // TextField

        Rectangle {
            id: eyeButton
            width: 40
            height: parent.height
            color: "transparent"

            Image {
                id: eyeIcon
                height: parent.height
                anchors.centerIn: parent
                source: eyeButton.state === "visible" ?
                            "qrc:/resources/icons/visible/visible_64.png" :
                            "qrc:/resources/icons/visible/invisible_64.png"
                fillMode: Image.PreserveAspectFit
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    eyeButton.state = eyeButton.state === "visible" ? "hidden" : "visible";
                    inputField.echoMode = eyeButton.state === "visible" ? TextInput.Normal : TextInput.Password;
                }
            }

        } // Rectangle
    } // Row
} // Rectangle
