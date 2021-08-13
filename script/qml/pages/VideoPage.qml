import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15

import "../controls"
import "../delegates"

Rectangle {
    id: videoPage

    function makeSearchBarConnections(){
        searchBar.searchRequested.connect(searchVideo)
        searchBar.focusModified.connect(playListArea.hideShow)
    }

    function makeInternalConnections(){
        //makePlayButtonConnections()
        //makeMusicSliderConnections()
        //makePlayerConnections()
        makeSearchBarConnections()
        //makeSongsSearchResultsConnections()
        //makeNextButtonConnections()
        //makePrevButtonConnections()
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

    ItemList{
        visible: true
        id: foundMoviesList
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchBar.bottom
        anchors.bottom: videoTitle.top
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        externalModel: null//C++
        externalDelegate: VideoDelegate{
            height: 60
            width: foundMoviesList.width
            mediaSource: source

            onPlayRequest: searchVideo(src)
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

    TitleLabel {
        id: videoTitle
        y: 395
        width: 52
        height: 19
        anchors.left: slider.left
        anchors.bottom: slider.top
        anchors.leftMargin: 0
        anchors.bottomMargin: 10
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
