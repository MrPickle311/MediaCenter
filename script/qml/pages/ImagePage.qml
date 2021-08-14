import QtQuick 2.15
import QtQuick.Controls 2.15

import "../controls"
import "../delegates"

Rectangle {
    id: imagePage
    color: "#191919"
    visible: true

    ScrollView{
        id: scrollVIew

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
            }

            model: ListModel {
                ListElement {
                    imageSource: "file:///home/damiano/Projects/MediaCenter/data/imageVERYVERYVERYVERYLONG.jpeg"
                }
                //
                //ListElement {
                //    name: "Red"
                //    colorCode: "red"
                //}
                //
                //ListElement {
                //    name: "Blue"
                //    colorCode: "blue"
                //}
                //
                //ListElement {
                //    name: "Green"
                //    colorCode: "green"
                //}
            }
        }
    }



}
