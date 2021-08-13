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
            if(focus)
                results.visible = true
            else results.visible = false
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
        anchors.bottomMargin: 30
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

        ScrollView {
                anchors.fill: playListArea
                clip: true
                ListView {
                        anchors.fill: parent
                        model: playlistModel
                }
        }

        DelegateModel {
                id: playlistModel
                model: player.playlist
                delegate: PlaylistDelegate {
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
        }
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

    SearchResults{
        id: results
        width: 100
        height: 100

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
