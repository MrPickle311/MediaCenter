import QtQuick 2.15
import QtQuick.Controls 2.15

import "../controls"

Rectangle {
    id: audioPage
    color: "#191919"

    Slider {
        id: slider
        y: 413
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        stepSize: 0.01
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        anchors.bottomMargin: 20
        value: 0.5
    }

    SearchBar{
        x: 570
        y: 391

    }

    Label {
        id: label
        y: 395
        width: 52
        height: 19
        color: "#dcdfe2"
        text: qsTr("Music Title")
        anchors.left: slider.left
        anchors.bottom: slider.top
        wrapMode: Text.NoWrap
        anchors.leftMargin: 0
        anchors.bottomMargin: 10
        textFormat: Text.AutoText
        font.pointSize: 12
    }

    Rectangle {
        id: rectangle
        radius: 7
        border.width: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: label.top
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        anchors.bottomMargin: 20
        anchors.topMargin: 30
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#434343"
            }

            GradientStop {
                position: 1
                color: "#000000"
            }
        }
    }


}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1}D{i:2}D{i:3}
}
##^##*/
