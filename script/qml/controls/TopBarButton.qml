import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

CustomButton {

    id : topBarButton

    implicitWidth: 35
    implicitHeight: 35

    buttonIconSource: "../../../data/minimize_icon.svg"

    background: Rectangle{
        id: background

        color: internal.dynamicColor

        Image {
            id: buttonIcon
            source: buttonIconSource

            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            height: 16
            width: 16

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
