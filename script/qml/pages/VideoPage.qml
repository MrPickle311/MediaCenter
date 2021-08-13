import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15

import "../controls"
import QtQuick.Extras 1.4

Rectangle {
    id: videoPage

    Component.onCompleted: {

    }

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

    Video{
        id: video
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: videoTitle.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 20
        fillMode: VideoOutput.Stretch
    }

    SearchBar{
        id: searchBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 10
        anchors.topMargin: 10
        anchors.rightMargin: 20
        currentWidth: videoPage.width - 20
    }

    Slider {
        id: slider
        y: 426
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        value: 0.5
    }

    Label {
        id: videoTitle
        y: 395
        width: 52
        height: 19
        color: "#dcdfe2"
        text: qsTr("Video Title")
        anchors.left: slider.left
        anchors.bottom: slider.top
        wrapMode: Text.NoWrap
        anchors.leftMargin: 0
        anchors.bottomMargin: 10
        textFormat: Text.AutoText
        font.pointSize: 12

        function setTitle(source){
            text = source.split('/').pop()
        }
    }

    TimeLabel{
        id: videoTIme
        width: 40
        anchors.right: slider.right
        anchors.bottom: slider.top
        anchors.bottomMargin: 10
        anchors.rightMargin: 0

    }

    PlayButton{
        id: playButton
        x: 302
        y: 364
        width: 40
        height: 40
        anchors.bottom: slider.top
        anchors.bottomMargin: 10
        anchors.horizontalCenter: slider.horizontalCenter

    }
}
