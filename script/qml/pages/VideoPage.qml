import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15

import "../controls"
import "../delegates"

Rectangle {
    id: videoPage

    function makePlayButtonConnections(){
        playButton.clicked.connect(videoPlayer.updateState)
    }

    function makeSearchBarConnections(){
        searchBar.searchRequested.connect(searchVideo)
        searchBar.focusModified.connect(foundMoviesList.showHide)
    }

    function makeFoundMoviesListConnections(){
        foundMoviesList.playVideoRequested.connect(searchBar.hide)
        foundMoviesList.playVideoRequested.connect(videoPlayer.playVideo)
        foundMoviesList.playVideoRequested.connect(playButton.updateIconSource)
        foundMoviesList.playVideoRequested.connect(videoTitle.setTitle)
    }

    function makeVideoSliderConnections(){
        videoSlider.positionMoved.connect(videoPlayer.changeVideoPosition)
    }

    function makePlayerConnections(){
        videoPlayer.millisChanged.connect(videoTimeLabel.setTime)
        videoPlayer.relativePositionChanged.connect(videoSlider.changePosition)
    }

    function makeInternalConnections(){
        makePlayButtonConnections()
        makeVideoSliderConnections()
        makePlayerConnections()
        makeSearchBarConnections()
        makeFoundMoviesListConnections()
    }

    function setVolume(vol){
            videoPlayer.volume = vol
    }

    signal searchVideo(url src)

    Component.onCompleted: {
        makeInternalConnections()
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
        id: videoPlayer
        visible: true
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: videoTitle.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.bottomMargin: 20
        fillMode: VideoOutput.Stretch

        focus: true

        signal millisChanged(int millis)
        signal relativePositionChanged(real position)

        onPositionChanged: {
            millisChanged(position)
            relativePositionChanged(position / duration)
        }

        function playVideo(src){
            source = src
            play()
        }

        function changeVideoPosition(position){
            seek(position * duration)
        }

        function updateState(){
            if(playbackState === MediaPlayer.PlayingState) pause()
            else play()
        }
    }

    VideoList{
        id: foundMoviesList
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchBar.bottom
        anchors.bottom: videoTitle.top
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10

        onPlayVideoRequested: showHide(false)

        //to C++
        externalModel: ListModel{
            ListElement{source: "file:///home/damiano/Projects/MediaCenter/data/video.mp4"}
        }
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

    MediaSlider {
        id: videoSlider
        y: 426
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        value: 0.5
    }

    TitleLabel {
        id: videoTitle
        y: 395
        width: 52
        height: 19
        anchors.left: videoSlider.left
        anchors.bottom: videoSlider.top
        anchors.leftMargin: 0
        anchors.bottomMargin: 10
    }

    TimeLabel{
        id: videoTimeLabel
        width: 40
        anchors.right: videoSlider.right
        anchors.bottom: videoSlider.top
        anchors.bottomMargin: 10
        anchors.rightMargin: 0
    }

    PlayButton{
        id: playButton
        x: 302
        y: 364
        width: 40
        height: 40
        anchors.bottom: videoSlider.top
        anchors.bottomMargin: 10
        anchors.horizontalCenter: videoSlider.horizontalCenter
    }
}
