import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

CustomButton {

    id : toggleButton

    implicitWidth: 70
    implicitHeight: 60

    property int iconSize: 25

    background: Rectangle{
        id: background

        color: internal.dynamicColor

        Image {
            id: buttonIcon
            source: buttonIconSource

            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            height: iconSize
            width: iconSize

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
