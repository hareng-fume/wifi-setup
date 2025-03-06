import QtQuick
import QtQuick.Controls

Rectangle {
    id: container
    width: 250
    height: 40
    radius: 5
    border.color: "#ccc"
    color: "white"

    property alias password: passwordField.text
    property alias placeholderText: passwordField.placeholderText

    property bool isValid: true
    signal passwordValidityChanged(bool valid)

    Row {
        anchors.fill: parent
        spacing: 5

        TextField {
            id: passwordField
            width: parent.width - eyeButton.width - 10
            height: parent.height
            color: "black"

            placeholderText: "16 charachter minimum"
            placeholderTextColor: "gray"

            validator: RegularExpressionValidator {
                regularExpression: /^[A-Za-z_\W][A-Za-z0-9_\W]{15,}$/
            }

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
                var _isValid = !(password.length < 16 || !validator.regularExpression.test(password))
                if (isValid !== _isValid) {
                    isValid = _isValid
                    passwordField.color = isValid ? "black" : "red"
                    passwordValidityChanged(isValid)
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
                    passwordField.echoMode = eyeButton.state === "visible" ? TextInput.Normal : TextInput.Password;
                }
            }

        } // Rectangle
    } // Row
} // Rectangle
