import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

CustomButton {

    id : toggleButton

    implicitWidth: 70
    implicitHeight: 60

    buttonIconSource: "../../../data/menu_icon.svg"

    background: Rectangle{
        id: background

        color: internal.dynamicColor

        Image {
            id: buttonIcon
            source: buttonIconSource

            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            height: 25
            width: 25

            fillMode: Image.PreserveAspectFit
        }

        ColorOverlay{
            anchors.fill:  buttonIcon
            source: buttonIcon
            color: "white"
            antialiasing: true
        }
    }
}
