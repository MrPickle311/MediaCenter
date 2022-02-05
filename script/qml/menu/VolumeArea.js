function increaseLinearVolume(){
    if(volumeArea.volume < 1.0)
    {
        volumeArea.volume += 0.01
    }
}

function decreaseLinearVolume(){
    if(volumeArea.volume > 0.0)
    {
        volumeArea.volume -= 0.01
    }
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
