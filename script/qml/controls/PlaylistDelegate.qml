import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Rectangle {
        id: playlistItem

        property string songSource: ""

        color: "#282c34"
        border.color: "#202225"
        border.width: 2
        height: 70
        width: 350

        signal playSongRequest(string src)

        SquareButton{
            id: playButton
            width: 50
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            defaultColor: "#00000000"
            buttonIconSource: "qrc:/data/play.svg"
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.rightMargin: 10
            onClicked: playSongRequest(text)
        }

        Image {
            id: songIcon
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
            anchors.fill: songIcon
            source: songIcon
            color: "white"
            antialiasing: true

            width: songIcon.width
            height: songIcon.height
        }

        TextArea {
            id: textArea
            anchors.left: songIcon.right
            anchors.right: playButton.left
            anchors.top: playlistItem.top
            anchors.bottom: playlistItem.bottom
            horizontalAlignment: Text.AlignLeft
            wrapMode: Text.WordWrap
            placeholderTextColor: "#7fffffff"
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            placeholderText: songSource.split('/').pop()
            readOnly: true
        }
}
