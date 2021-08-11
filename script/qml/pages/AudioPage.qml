import QtQuick 2.15
import QtQuick.Controls 2.15

import "../controls"

Rectangle {
    id: audioPage
    color: "#191919"

    Slider {
        id: musicSlider
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
        id: searchBar
        anchors.left: playListArea.left
        anchors.top: playListArea.top
        anchors.rightMargin: 40
        anchors.leftMargin: 10
        anchors.topMargin: 10

        currentWidth: playListArea.width - 20

        onSearchRequested: console.log(what)

        z: 2

    }

    Label {
        id: musicTitle
        y: 395
        width: 52
        height: 19
        color: "#dcdfe2"
        text: qsTr("Music Title")
        anchors.left: musicSlider.left
        anchors.bottom: musicSlider.top
        wrapMode: Text.NoWrap
        anchors.leftMargin: 0
        anchors.bottomMargin: 10
        textFormat: Text.AutoText
        font.pointSize: 12
    }

    Rectangle {
        id: playListArea
        radius: 7
        border.width: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: musicTitle.top
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
