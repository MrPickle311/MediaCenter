import QtQuick 2.15
import QtQuick.Controls 2.15

import "../delegates"

ItemList {
    id: videoList
    radius: 7
    border.width: 1

    signal playVideoRequested(url src)

    externalDelegate: SongDelegate {
        height: 60
        width: videoList.width
        mediaSource: source

        onPlayRequest: playVideoRequested(src)
    }
}
