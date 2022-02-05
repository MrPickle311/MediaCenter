function makePlayButtonConnections(){
    playButton.clicked.connect(audioPlayer.goNextState)
}

function makeMusicSliderConnections(){
    musicSlider.positionMoved.connect(audioPlayer.changeSongPosition)
}

function makePlayerConnections(){
    audioPlayer.millisChanged.connect(musicTimeLabel.setTime)
    audioPlayer.relativePositionChanged.connect(musicSlider.changePosition)
}

function makeSearchBarConnections(){
    searchBar.searchRequested.connect(searchInPlaylist)
    searchBar.focusModified.connect(playListArea.hideShow)
    searchBar.focusModified.connect(audioSearchResults.showHide)
}

function makeSongsSearchResultsConnections(){
    audioSearchResults.playSongRequested.connect(searchBar.hide)
    audioSearchResults.playSongRequested.connect(playButton.updateIconSource)
    audioSearchResults.playSongRequested.connect(musicTitle.setTitle)
}

function makeNextButtonConnections(){
    nextButton.clicked.connect(audioPage.nextSongRequested)
//    nextButton.clicked.connect(audioPlayer.goNextSong)
}

function makePrevButtonConnections(){
    nextButton.clicked.connect(audioPage.previousSongRequested)
//    prevButton.clicked.connect(audioPlayer.goPreviousSong)
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

function setVolume(vol){
    audioPlayer.volume = vol
}


