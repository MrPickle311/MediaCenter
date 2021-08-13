import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

import "../controls"

Rectangle {
        id: playlistItem

        color: "#282c34"
        border.color: "#202225"
        border.width: 2
        height: 70
        width: 350

        property url mediaSource: ""
        property alias mediaIcon: ""

        signal playRequest(url src)

        Image {
            id: mediaIcon
            width: 50
            anchors.left: playlistItem.left
            anchors.top: playlistItem.top
            anchors.bottom: playlistItem.bottom
            source: "qrc:/data/song.svg"
            anchors.bottomMargin: 10
            anchors.leftMargin: 10
            anchors.topMargin: 10
            fillMode: Image.PreserveAspectFit
        }

        ColorOverlay{
            id: clearIconOverlay
            anchors.fill: mediaIcon
            source: mediaIcon
            color: "white"
            antialiasing: true

            width: mediaIcon.width
            height: mediaIcon.height
        }

        TextArea {
            id: textArea
            anchors.left: mediaIcon.right
            //anchors.right: playButton.left
            anchors.right: playlistItem.right
            anchors.top: playlistItem.top
            anchors.bottom: playlistItem.bottom
            horizontalAlignment: Text.AlignLeft
            wrapMode: Text.WordWrap
            placeholderTextColor: "#7fffffff"
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            placeholderText: mediaSource.toString().split('/').pop()
            readOnly: true
        }

        MouseArea{
            id: mouseArea
            anchors.fill: parent
            onClicked: playSongRequest(mediaSource)
            hoverEnabled: true
            onHoveredChanged: if(containsMouse) playlistItem.border.color = "#55aaff"
                              else playlistItem.border.color = "#202225"
        }
}
