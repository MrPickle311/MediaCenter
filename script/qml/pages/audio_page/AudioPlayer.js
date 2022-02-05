function propagateAudioCurrentPosition(position){
    millisChanged(position)
    relativePositionChanged(position / audioPlayer.duration)
}

function setAudioOutput(audioOutput){
    audioPlayer.audioOutput = audioOutput
}

function setSource(newSource){
    audioPlayer.source = newSource
}

function pausePlaying(){
    audioPlayer.pause()
}

function startPlaying(){
    audioPlayer.play()
}

function goNextState(){
    if(audioPlayer.playbackState === MediaPlayer.PlayingState){
        audioPlayer.stopPlaying()
    }
    else{
        audioPlayer.startPlaying()
    }
}

function changeSongPosition(position){
    audioPlayer.setPosition(position * audioPlayer.duration)
}

function makeConnections(){
    audioPlayer.positionChanged.connect(audioPlayer.propagateAudioCurrentPosition)
}
