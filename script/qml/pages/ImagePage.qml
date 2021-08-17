import QtQuick 2.15
import QtQuick.Controls 2.15

import "../controls"
import "../delegates"

Rectangle {
    id: imagePage
    color: "#191919"
    visible: true

    signal loadNextImageRequested(url current_image_src)
    signal loadPreviousImageRequested(url current_image_src)

    function makeImageBrowserConnections(){
        imageBrowser.itemClicked.connect(imageViewer.setImage)
        imageBrowser.itemClicked.connect(imageViewer.show)
    }

    function makeImageViewerConnections(){
        imageViewer.nextImageRequested.connect(loadNextImageRequested)
        imageViewer.previousImageRequested.connect(loadPreviousImageRequested)
    }

    function makeInternalConnections(){
        makeImageBrowserConnections()
    }

    Component.onCompleted: {
        makeInternalConnections()
    }

    function setImageToViewer(src){
        imageViewer.setImage(src)
    }

    ItemsGrid{
        id: imageBrowser
        anchors.fill: parent

        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10

        externalDelegate: ImageDelegate{
            visible: true
            id: imageDelegate
            source: imageSource

            onShowImageRequest: imageBrowser.itemClicked(src)
        }

        externalModel: ListModel {
            ListElement {
                imageSource: "file:///home/damiano/Projects/MediaCenter/data/imageVERYVERYVERYVERYLONG.jpeg"
            }
        }
    }

    ImageViewer{
        id: imageViewer
        visible: false
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 10
        anchors.topMargin: 10

        Action {
           shortcut: "Esc"
           onTriggered: imageViewer.hide()
        }
    }
}
