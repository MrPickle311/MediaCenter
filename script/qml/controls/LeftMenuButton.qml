import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

CustomButton {

    id : leftMenuButton

    implicitWidth: 250
    implicitHeight: 60

    buttonIconSource: "../../../data/home_icon.svg"

    iconWidth:  18
    iconHeight: 18

    property color activeMenuColor: "#55aaff"
    property bool isActiveMenu: false
    property color activeMenuColorRight: "#2c313c"

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
