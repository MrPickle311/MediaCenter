import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: customButton
    text: qsTr("")

    implicitHeight: 40
    implicitWidth: 200

    //properties
    property color defaultColor: "#1c1d20"
    property color mouseOverColor: "#23273E"
    property color pressedColor: "#00a1f1"
    property url   buttonIconSource: ""

    antialiasing: true

    property int iconWidth:  18
    property int iconHeight: 18

    //expose internal implementation
    property alias internal: internal

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
