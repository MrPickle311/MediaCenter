import QtQuick 2.15
import QtQuick.Controls 2.15

import "../delegates"

ItemList {
    id: songList
    visible: true
    radius: 7
    border.width: 1

    signal playSongRequested(url src)

    externalDelegate: PlaylistDelegate {
        height: 60
        width: songList.width
        songSource: source

        onPlaySongRequest: playSongRequested(src)
    }
}
