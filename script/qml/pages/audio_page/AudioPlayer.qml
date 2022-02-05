import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia

import "AudioPlayer.js" as Logic

MediaPlayer{
    id: audioPlayer
    source: "song1.ogg"

    signal millisChanged(int millis)
    signal relativePositionChanged(real position)

    Component.onCompleted: {
        Logic.makeConnections()
    }

    // public methods

    function propagateAudioCurrentPosition(position){
        Logic.propagateAudioCurrentPosition(position)
    }

    function setAudioOutput(audioOutput){
        Logic.setAudioOutput(audioOutput)
    }

    function setSource(newSource){
        Logic.setSource(newSource)
    }

    function pausePlaying(){
        Logic.pausePlaying()
    }

    function startPlaying(){
        Logic.startPlaying()
    }

    function goNextState(){
        Logic.goNextState()
    }

    function changeSongPosition(position){
        Logic.changeSongPosition()
    }

}
