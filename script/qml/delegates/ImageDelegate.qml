import QtQuick 2.15
import QtQuick.Controls 2.15

import "../controls"

Rectangle{
    id: imageDelegate

    property url source: ""

    signal showImageRequest(url src)
    color: "#00000000"
    border.color: "#00000000"

    width: 150
    height: 150

    Component.onCompleted: imageTitle.setTitle(source)

    Image {
        id: image
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: titleScroll.top
        source: imageDelegate.source
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 5
        anchors.topMargin: 5
        asynchronous: true
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked: showImageRequest(source)
        hoverEnabled: true
        onHoveredChanged: if(containsMouse) imageDelegate.border.color = "#55aaff"
                          else imageDelegate.border.color = "#00000000"
    }

   ScrollView{
       id: titleScroll
       anchors.left: parent.left
       anchors.right: parent.right
       anchors.bottom: parent.bottom
       anchors.rightMargin: 5
       anchors.bottomMargin: 0
       anchors.leftMargin: 5
       height: 30
       TextArea{
           id: imageTitle
           wrapMode: Text.NoWrap
           placeholderTextColor: "white"
           function setTitle(source){
               text = source.toString().split('/').pop()
           }
           readOnly: true
           background: Rectangle{
               color: "#00000000"
           }
       }
   }
}
