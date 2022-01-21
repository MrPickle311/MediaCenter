import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

import "../controls"
import "../logic/SvgGetter.js" as Logic

Rectangle {
    id: leftMenu
    width: 65
    color: "#1c1d20"
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    clip: true
    anchors.topMargin: 0
    anchors.bottomMargin: 0
    anchors.leftMargin: 0

    function runAnimation(){
        animationMenu.running = true
    }

    signal buttonClicked(int id)

    signal settingsButtonClicked()

    property var currentlySelectedButton: musicButton

    function selectButton(button){
        currentlySelectedButton.isActiveMenu = false
        currentlySelectedButton = button
        button.isActiveMenu = true
    }

    PropertyAnimation{
        id: animationMenu
        target: leftMenu
        property: "width"
        to: leftMenu.width === 65 ? 250 : 65
        duration: 500
        easing.type: Easing.InOutQuint
    }

    Column {
        id: columnMenus
        width: leftMenu.border.width
        anchors.fill: parent
        spacing: 0
        anchors.bottomMargin: 70

        //LeftMenuButton {
        //    id: homeButton
        //    text: qsTr("Home page")
        //
        //    isActiveMenu: true
        //
        //    onClicked: {
        //        selectButton(homeButton)
        //        buttonClicked(0)
        //    }
        //}

        LeftMenuButton {
            id: musicButton
            text: qsTr("Play music")
            buttonIconSource: Logic.getControl("music.svg")

            onClicked: {
                selectButton(musicButton)
                buttonClicked(0)
            }
        }

        LeftMenuButton {
            id: videoButton
            text: qsTr("Play video")
            buttonIconSource: Logic.getControl("video.svg")

            onClicked: {
                selectButton(videoButton)
                buttonClicked(1)
            }
        }

        LeftMenuButton {
            id: imagesButton
            text: qsTr("Browse photos")
            checkable: false
            checked: false
            buttonIconSource: Logic.getControl("image.svg")

            onClicked: {
                selectButton(imagesButton)
                buttonClicked(2)
            }
        }
    }

    LeftMenuButton {
        id: settingsButton
        x: 0
        y: 240
        text: qsTr("Open settings")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        padding: 6
        buttonIconSource: Logic.getControl("settings_icon.svg")

        onClicked: {
            selectButton(settingsButton)
            settingsButtonClicked()
        }
    }
}
