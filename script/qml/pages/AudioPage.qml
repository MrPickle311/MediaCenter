import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15
import QtQml.Models 2.12

import "../delegates"
import "../controls"

Rectangle {
    id: audioPage
    color: "#191919"

    //public signals

    //after emisission of this signal -> model update
    signal searchInPlaylist(string src)

    //make Connections

    function makePlayButtonConnections(){
        playButton.clicked.connect(player.updateState)
    }

    function makeMusicSliderConnections(){
        musicSlider.positionMoved.connect(player.changeSongPosition)
    }

    function makePlayerConnections(){
        player.millisChanged.connect(musicTimeLabel.setTime)
        player.relativePositionChanged.connect(musicSlider.changePosition)
    }

    function makeSearchBarConnections(){
        searchBar.searchRequested.connect(searchInPlaylist)
        searchBar.focusModified.connect(playListArea.hideShow)
        searchBar.focusModified.connect(songsSearchResults.showHide)
    }

    function makeSongsSearchResultsConnections(){
        songsSearchResults.playSongRequested.connect(searchBar.hide)
    }

    function makeNextButtonConnections(){
        nextButton.clicked.connect(player.goNextSong)
    }

    function makePrevButtonConnections(){
        prevButton.clicked.connect(player.goPreviousSong)
    }

    function makeInternalConnections(){
        makePlayButtonConnections()
        makeMusicSliderConnections()
        makePlayerConnections()
        makeSearchBarConnections()
        makeSongsSearchResultsConnections()
        makeNextButtonConnections()
        makePrevButtonConnections()
    }

    Component.onCompleted: {
        makeInternalConnections()
    }

    MediaSlider {
        id: musicSlider
        y: 413
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        anchors.bottomMargin: 20
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
    }

    TitleLabel {
        id: musicTitle
        y: 395
        width: 52
        height: 19
        anchors.left: musicSlider.left
        anchors.bottom: musicSlider.top
        anchors.leftMargin: 0
        anchors.bottomMargin: 10
    }

    //a playlist view
    SongList {
        id: playListArea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: musicTitle.top
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        anchors.bottomMargin: 30
        anchors.topMargin: 30
        externalModel: player.playlist

        //only revert the boolean argument and delegate to showHide ()
        function hideShow(predicate){
            showHide(!predicate)
        }
    }

    SongList{
        id: songsSearchResults
        anchors.left: playListArea.left
        anchors.right: playListArea.right
        anchors.top: searchBar.bottom
        anchors.bottom: playListArea.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 10
        visible: false
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

    NextButton {
        id: nextButton
        width: 56
        anchors.verticalCenter: playButton.verticalCenter
        anchors.left: playButton.right
        anchors.leftMargin: 10
    }

    PreviousButton {
        id: prevButton
        width: 56
        anchors.verticalCenter: playButton.verticalCenter
        anchors.right: playButton.left
        anchors.rightMargin: 10
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
