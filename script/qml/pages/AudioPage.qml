import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15
import QtQml.Models 2.12

import "../delegates"
import "../controls"

Rectangle {
    id: audioPage
    color: "#191919"

    //make Connections

    Component.onCompleted: {
        playButton.clicked.connect(player.updateState)

        musicSlider.positionMoved.connect(player.changeSongPosition)

        player.millisChanged.connect(musicTimeLabel.setTime)
        player.relativePositionChanged.connect(musicSlider.changePosition)

        searchBar.searchRequested.connect(searchInPlaylist)

        nextButton.clicked.connect(player.goNextSong)
        prevButton.clicked.connect(player.goPreviousSong)
    }

    //public signals

    //after emisission of this signal -> model update
    signal searchInPlaylist(string src)


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

        function changePosition(newPos){
            value = newPos
        }
        signal positionMoved(real position)
        onMoved: positionMoved(position)
    }

    SearchBar{
        id: searchBar
        anchors.left: playListArea.left
        anchors.top: playListArea.top
        anchors.rightMargin: 40
        anchors.leftMargin: 10
        anchors.topMargin: 10
        currentWidth: playListArea.width - 20
        z: 2

        onFocusChanged: {
            playListArea.showHide(!focus)
            searchResults.showHide(focus)
        }
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

        function setTitle(source){
            text = source.split('/').pop()
        }
    }

    //a playlist view
    SearchResults {
        id: playListArea
        visible: true
        radius: 7
        border.width: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: musicTitle.top
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        anchors.bottomMargin: 30
        anchors.topMargin: 30

        externalDelegate: PlaylistDelegate {
            height: 60
            width: playListArea.width
            songSource: source

            //C++ service
            onPlaySongRequest: {
                player.stop()
                //Bridge.getPosition(songSource)
                player.play()
            }
        }

        externalModel: player.playlist
    }

    SearchResults{
        id: searchResults

        radius: 7
        border.width: 1
        anchors.left: playListArea.left
        anchors.right: playListArea.right
        anchors.top: searchBar.bottom
        anchors.bottom: playListArea.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 10

        //To C++ bindings
        externalDelegate: PlaylistDelegate {
            height: 60
            width: playListArea.width
            songSource: source

            //C++ service
            onPlaySongRequest: {
                player.stop()
                //Bridge.getPosition(songSource)
                player.play()
            }
        }

        ListModel{
            id: temp
            ListElement{
                source: "file:///home/damiano/Projects/MediaCenter/data/song2.mp3"
            }
            ListElement{
                source: "song2"
            }
        }

        externalModel: temp
    }

    Audio {
            id: player;
            playlist: Playlist {
                id: playlist
                //MAKE A WRAPPER FOR A LIST ELEMENT IN C++
                PlaylistItem { source: "file:///home/damiano/Projects/MediaCenter/data/song2.mp3"; }
                PlaylistItem { source: "file:///home/damiano/Projects/MediaCenter/data/song.mp3"; }
                PlaylistItem { source: "song3.ogg"; }
                PlaylistItem { source: "song1.ogg"; }
                PlaylistItem { source: "song2.ogg"; }
                PlaylistItem { source: "song3.ogg"; }
                PlaylistItem { source: "song1.ogg"; }
                PlaylistItem { source: "song2.ogg"; }
                PlaylistItem { source: "song3.ogg"; }
            }

            signal millisChanged(int millis)
            signal relativePositionChanged(real position)

            onPositionChanged: {
                millisChanged(position)
                relativePositionChanged(position / duration)
            }

            function updateState(){
                if(playbackState === Audio.PlayingState) pause()
                else play()
            }

            function changeSongPosition(position){
                seek(position * duration)
            }

            function goNextSong(){
                playlist.next()
            }

            function goPreviousSong(){
                playlist.previous()
            }
    }

    PlayButton{
        id: playButton

        x: 295
        width: 56
        anchors.top: playListArea.bottom
        anchors.bottom: musicSlider.top
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.horizontalCenter: musicSlider.horizontalCenter
    }

    SquareButton {
        id: nextButton
        width: 56
        anchors.verticalCenter: playButton.verticalCenter
        anchors.left: playButton.right
        anchors.leftMargin: 10
        buttonIconSource: "qrc:/data/right-arrow.svg"
        defaultColor: "#00000000"
    }

    SquareButton {
        id: prevButton
        width: 56
        anchors.verticalCenter: playButton.verticalCenter
        anchors.right: playButton.left
        anchors.rightMargin: 10
        buttonIconSource: "qrc:/data/left-arrow.svg"
        defaultColor: "#00000000"
    }

    TimeLabel {
        id: musicTimeLabel
        x: 567
        y: 398
        width: 53
        height: 16
        anchors.right: musicSlider.right
        anchors.bottom: musicSlider.top
        anchors.bottomMargin: 10
        anchors.rightMargin: 0
    }
}
