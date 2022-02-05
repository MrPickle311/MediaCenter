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
