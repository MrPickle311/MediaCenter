import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

import "../controls"
import "../logic/SvgGetter.js" as Logic

Rectangle {
    id: topBar
    height: 60
    color: "#1c1d20"
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.rightMargin: 0
    anchors.leftMargin: 0
    anchors.topMargin: 0

    signal closing()
    signal maximizeRestore()
    signal minimize()
    signal dragging(bool isActive)
    signal toggleButtonClicked()

    property string restoreIcon:  Logic.getControl("restore_icon.svg")
    property string maximizeIcon: Logic.getControl("maximize_icon.svg")

    //theses 2 functions set top-left bar maximize icon
    function setRestoreIcon(){
        maximizeRestoreButton.buttonIconSource = restoreIcon
    }

    function setMaximizeIcon(){
        maximizeRestoreButton.buttonIconSource = maximizeIcon
    }

    function changeMaximizeRestoreIcon(){
        if(maximizeRestoreButton.buttonIconSource === maximizeIcon)
            setRestoreIcon()
        else setMaximizeIcon()
    }

    SquareButton {
        id: toggleBtn
        onClicked: toggleButtonClicked()
        buttonIconSource: Logic.getControl("menu_icon.svg")
    }

    Rectangle {
        id: titleBar
        height: 35
        color: "#00000000"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 105
        anchors.leftMargin: 70
        anchors.topMargin: 0

        DragHandler{
            id: dragHandler
            onActiveChanged: dragging(dragHandler.active)
        }

        Image {
            id: iconApp
            width: 22
            height: 22
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            source: Logic.getControl("media-center.svg")
            anchors.leftMargin: 0
            anchors.bottomMargin: 0
            anchors.topMargin: 0
            fillMode: Image.PreserveAspectFit
        }

        ColorOverlay{
            anchors.fill:  iconApp
            source: iconApp
            color: "white"
            antialiasing: true

            width: iconApp.width
            height: iconApp.height
        }

        Label {
            id: label
            x: 38
            width: 765
            color: "#c3cbdd"
            text: qsTr("Media Center v0.1")
            anchors.left: iconApp.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 10
            anchors.leftMargin: 5
        }
    }

    Row {
        id: rowButtons
        x: 863
        width: 105
        height: 35
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.rightMargin: 0

        SquareButton{
            iconSize: 16
            implicitWidth: 35
            implicitHeight: 35
            id: minimizeButton
            onClicked: minimize()

            buttonIconSource: Logic.getControl("minimize_icon.svg")
        }

        SquareButton{
            iconSize: 16
            implicitWidth: 35
            implicitHeight: 35
            id: maximizeRestoreButton
            buttonIconSource: maximizeIcon

            onClicked: {
                changeMaximizeRestoreIcon()
                maximizeRestore()
            }
        }

        SquareButton{
            iconSize: 16
            implicitWidth: 35
            implicitHeight: 35
            id: closeButton
            pressedColor: "#ff007f"
            buttonIconSource: Logic.getControl("close_icon.svg")
            onClicked: closing()
        }
    }
}
