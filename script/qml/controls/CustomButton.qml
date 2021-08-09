import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: customButton
    text: qsTr("Custom button")

    implicitHeight: 40
    implicitWidth: 200

    property color defaultColor: "#55aaff"
    property color mouseOverColor: "#cccccc"
    property color pressedColor: "#333333"

    QtObject{
        id: internal

        property var dynamicColor: if(customButton.down){
                                       customButton.down ? pressedColor : defaultColor
                                   } else {
                                       customButton.hovered ? mouseOverColor : defaultColor
                                   }
    }


    background: Rectangle{
        color: internal.dynamicColor
        radius: 10
    }

    contentItem: Item{
        Text {
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            id: buttonText
            color: "white"
            text: customButton.text
        }
    }
}
