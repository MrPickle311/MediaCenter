import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Button {

    id : leftMenuButton

    implicitWidth: 250
    implicitHeight: 60

    text: "Left Menu Text"

    property url buttonIconSource: "../../../data/home_icon.svg"

    property color defaultColor: "#1c1d20"
    property color mouseOverColor: "#23273E"
    property color pressedColor: "#00a1f1"
    antialiasing: true

    property int iconWidth:  18
    property int iconHeight: 18

    property color activeMenuColor: "#55aaff"

    property bool isActiveMenu: false

    property color activeMenuColorRight: "#2c313c"

    QtObject{
        id: internal

        property color dynamicColor: if(leftMenuButton.down){
                                       leftMenuButton.down ? pressedColor : defaultColor
                                   } else {
                                       leftMenuButton.hovered ? mouseOverColor : defaultColor
                                   }
    }

    background: Rectangle{
        id: background

        color: internal.dynamicColor

        Rectangle{
            anchors{
                top: parent.top
                left: parent.left
                bottom: parent.bottom
            }
            color: activeMenuColor
            width: 3
            visible: isActiveMenu
        }

        Rectangle{
            anchors{
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }
            color: activeMenuColorRight
            width: 5
            visible: isActiveMenu
        }

    }

    contentItem: Item{

        anchors.fill: parent
        id: content

        Image {
            id: buttonIcon
            source: buttonIconSource
            anchors.leftMargin: 26

            antialiasing: true

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left

            sourceSize.width: iconWidth
            sourceSize.height:  iconHeight
            height: iconHeight
            width: iconWidth

            fillMode: Image.PreserveAspectFit
        }

        ColorOverlay{
            anchors.fill:  buttonIcon
            source: buttonIcon
            color: "white"
            antialiasing: true

            width: iconWidth
            height: iconHeight
        }

        Text {
            id: buttonText
            color: "white"
            text: leftMenuButton.text
            font: leftMenuButton.font

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left

            anchors.leftMargin: 75


        }
    }



}
