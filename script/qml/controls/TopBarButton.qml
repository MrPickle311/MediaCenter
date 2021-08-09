import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Button {

    id : topBarButton

    implicitWidth: 35
    implicitHeight: 35

    property url buttonIconSource: "../../../data/minimize_icon.svg"

    property color defaultColor: "#1c1d20"
    property color mouseOverColor: "#23273E"
    property color pressedColor: "#00a1f1"

    QtObject{
        id: internal

        property color dynamicColor: if(topBarButton.down){
                                       topBarButton.down ? pressedColor : defaultColor
                                   } else {
                                       topBarButton.hovered ? mouseOverColor : defaultColor
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
