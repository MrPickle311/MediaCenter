import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia
import QtQml.Models 2.12

import "../delegates"
import "../controls"
import "audio_page"
import "AudioPage.js" as Logic

Rectangle {
    id: audioPage
    color: "#191919"

    // public signals

    //after emisission of this signal -> model update
    signal searchInPlaylist(string src)
    signal nextSongRequested()
    signal previousSongRequested()

    // public methods

    function setVolume(vol){
        Logic.setVolume(vol)
    }

    function setAudioSource(audioSource){

    }

    function pullSongsPlaylist(){
        playListArea.updateSongs(AudioBackend.requestSongsPlaylist())
    }

    Component.onCompleted: {
        Logic.makeInternalConnections()
        audioPage.pullSongsPlaylist()
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

        //only revert the boolean argument and delegate to showHide ()
        function hideShow(predicate){
            showHide(!predicate)
        }

    }

    SongList{
        id: audioSearchResults
        anchors.left: playListArea.left
        anchors.right: playListArea.right
        anchors.top: searchBar.bottom
        anchors.bottom: playListArea.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 10
        visible: false

    }

    AudioPlayer{
        id: audioPlayer
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
