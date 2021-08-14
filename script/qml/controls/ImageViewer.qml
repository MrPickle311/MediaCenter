import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: imageViewer
    color: "#00000000"

    function show(){
        focus = true
        visible = true
    }

    function hide(){
        focus = false
        visible = false
    }

    function setImage(src){
        image.source = src
    }

    signal nextImageRequested(url current_image)
    signal previousImageRequested(url current_image)

    NextButton {
        id: nextButton
        x: 474
        y: 241
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        onClicked: nextImageRequested(image.source)
    }

    PreviousButton {
        id: previousButton
        y: 190
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        onClicked: previousImageRequested(image.source)
    }

    Image {
        id: image
        anchors.left: previousButton.right
        anchors.right: nextButton.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 10
        fillMode: Image.PreserveAspectFit
    }



}
