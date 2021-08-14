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
        imageBrowser.showImageRequested.connect(imageViewer.setImage)
        imageBrowser.showImageRequested.connect(imageViewer.show)
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

    ScrollView{
        id: imageBrowser

        signal showImageRequested(url src)

        GridView {
            id: gridView
            anchors.fill: parent
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            cellWidth: 70
            cellHeight: 70
            delegate: ImageDelegate{
                visible: true
                id: imageDelegate
                source: imageSource

                onShowImageRequest: imageBrowser.showImageRequested(src)
            }

            model: ListModel {
                ListElement {
                    imageSource: "file:///home/damiano/Projects/MediaCenter/data/imageVERYVERYVERYVERYLONG.jpeg"
                }
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

        Shortcut {
                sequence: "Esc"
                onActivated: imageViewer.hide()
        }
    }
}
