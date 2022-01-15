import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15
import QtMultimedia 5.15

import "../controls"
import "../pages"

Rectangle {
    id: contentPages
    color: "#2c313c"
    anchors.left: leftMenu.right
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 0
    anchors.leftMargin: 0
    clip: true

    function setCurrentPage(page_id){
        swipeView.setCurrentIndex(page_id)
    }

    function setSettingsPage(){
        swipeView.setCurrentIndex(swipeView.count - 1)
    }

    function increaseVolume(){
        videoPage.increaseVolume()
        audioPage.increaseVolume()
    }

    function decraseVolumne(){
        videoPage.decreaseVolume()
        audioPage.decreaseVolume()
    }

    Item{
        id: volumeArea
        anchors.fill: contentPages
        z: 1
        property real volume: 0.5
        property real realVolume: QtMultimedia.convertVolume(volumeArea.volume,
                                                             QtMultimedia.LinearVolumeScale,
                                                             QtMultimedia.LogarithmicVolumeScale)

        function increaseLinearVolume(){
            if(volumeArea.volume < 1.0)
                volumeArea.volume += 0.01
        }

        function decreaseLinearVolume(){
            if(volumeArea.volume > 0.0)
                volumeArea.volume -= 0.01
        }

        function showAndSet(){
            timer.start()
            volumeArea.show()
            audioPage.setVolume(realVolume)
        }

        function increaseVolume(){
            progressBar.increaseVolume()
            volumeArea.increaseLinearVolume()
            volumeArea.showAndSet()
        }

        function decraseVolumne(){
            progressBar.decreaseVolume()
            volumeArea.decreaseLinearVolume()
            volumeArea.showAndSet()
        }

        function hide(){
            volumeArea.visible = false
        }

        function show(){
            volumeArea.visible = true
        }

        CircularProgressBar{
            id: progressBar
            Component.onCompleted: value = volumeArea.volume * 100
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            z:2
        }

//242424
        Timer{
            id: timer
            repeat: false
            interval: 800
            onTriggered: {
                volumeArea.hide()
                progressBar.hide()
            }
        }

        Shortcut {
            sequence: "Ctrl+Shift+M"
            onActivated: volumeArea.decraseVolumne()
        }

        Shortcut {
            sequence: "Ctrl+Shift+L"
            onActivated: volumeArea.increaseVolume()
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        interactive: false

        orientation: Qt.Vertical

        //HomePage{
        //
        //}

        AudioPage{
            id: audioPage
        }

        VideoPage{
            id: videoPage
        }

        ImagePage{

        }

        SettingsPage{

        }
    }
}
