import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import QtMultimedia

import "../controls"
import "../pages"
import "ContentPages.js" as Logic

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

    //public methods

    function setCurrentPage(page_id){
        Logic.setCurrentPage(page_id)
    }

    function setSettingsPage(){
        Logic.setSettingsPage()
    }

    function increaseVolume(){
        Logic.increaseVolume()
    }

    function decraseVolumne(){
        Logic.decraseVolumne()
    }

    VolumeArea{
        id: volumeArea
        anchors.fill: contentPages
        z: 1

    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        interactive: false

        orientation: Qt.Vertical

//        HomePage{

//        }

        AudioPage{
            id: audioPage
        }

//        VideoPage{
//            id: videoPage
//        }

//        ImagePage{

//        }

        SettingsPage{
            id: settingsPage

        }
    }
}
