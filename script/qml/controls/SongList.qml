import QtQuick 2.15
import QtQuick.Controls 2.15

import "../delegates"

ItemList {
    id: songList
    visible: true
    radius: 7
    border.width: 1


    signal playSongRequested(url src)

    externalDelegate: SongDelegate {
        id: songDelegate
        Component.onCompleted: {
            songDelegate.playRequest.connect(songList.playSongRequested)
        }
        height: 60
        width: songList.width
        mediaSource: source

//        onPlayRequest: playSongRequested(src)
    }

    function updateSongs(newSongsList){
        songList.externalModel = newSongsList
    }
}
