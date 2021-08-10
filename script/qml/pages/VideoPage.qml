import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15

Item {
    Video{
        id: video
        anchors.fill: parent

        property url vidSrc: "file:///C:/Users/Damian/Desktop/Projects/C++ Projects/Qt/MediaCenter/script/qml/pages/video.mp4"

        source: vidSrc

        autoLoad: true

        focus: true

        MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("played  "  + video.availability + "  " + video.vidSrc)
                    video.play()
                }
        }

        fillMode: VideoOutput.Stretch

    }
}
