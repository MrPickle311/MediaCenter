import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

import "../controls"
import "../pages"

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

    function setCurrentPage(page_id){
        swipeView.setCurrentIndex(page_id)
    }

    function setSettingsPage(){
        swipeView.setCurrentIndex(swipeView.count - 1)
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent

        orientation: Qt.Vertical

        HomePage{

        }

        AudioPage{

        }

        VideoPage{

        }

        ImagePage{

        }

        SettingsPage{

        }
    }
}
