import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Button {

    id : toggleButton

    implicitWidth: 70
    implicitHeight: 60

    property url buttonIconSource: "../../../data/menu_icon.svg"

    property color defaultColor: "#1c1d20"
    property color mouseOverColor: "#23273E"
    property color pressedColor: "#00a1f1"

    QtObject{
        id: internal

        property color dynamicColor: if(toggleButton.down){
                                       toggleButton.down ? pressedColor : defaultColor
                                   } else {
                                       toggleButton.hovered ? mouseOverColor : defaultColor
                                   }
    }

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
