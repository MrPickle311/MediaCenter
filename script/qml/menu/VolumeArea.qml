import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia
import "../controls"

import "VolumeArea.js" as Logic

Item{
    id: volumeArea
    property real volume: 0.5
    property real realVolume: volume //QtMultimedia.convertVolume(volumeArea.volume,
    //                         QtMultimedia.LinearVolumeScale,
    //                       QtMultimedia.LogarithmicVolumeScale)

    //public methods
    function increaseLinearVolume(){
        Logic.increaseLinearVolume()
    }

    function decreaseLinearVolume(){
        Logic.decreaseLinearVolume()
    }

    function showAndSet(){
        Logic.showAndSet()
    }

    function increaseVolume(){
        Logic.increaseVolume()
    }

    function decraseVolumne(){
        Logic.decraseVolumne()
    }

    function hide(){
        Logic.hide()
    }

    function show(){
        Logic.show()
    }

    CircularProgressBar{
        id: progressBar
        Component.onCompleted: value = volumeArea.volume * 100
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        z:2
    }

    AudioOutput{
        id: audioOutput
    }

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
