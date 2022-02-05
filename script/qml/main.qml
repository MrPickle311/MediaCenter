import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import "controls"
import "pages"
import "menu"
import "main.js" as Logic

Window {

    id: mainWindow

    width: 1000
    height: 580
    visible: true
    color: "#00000000"
    title: qsTr("Media Center")

    minimumWidth: 800
    minimumHeight: 500

    flags: Qt.Window | Qt.FramelessWindowHint

    property int windowStatus: 0
    property int windowMargin: 10

    onHeightChanged: Logic.trySetNormalMainWindowState()


    Rectangle {
        id: background
        color: "#2c313c"
        border.color: "#383e4c"
        border.width: 1
        anchors.fill: parent
        anchors.rightMargin: windowMargin
        anchors.leftMargin: windowMargin
        anchors.bottomMargin: windowMargin
        anchors.topMargin: windowMargin
        z:1

        Rectangle {
            id: appContainer
            color: "#00000000"
            anchors.fill: parent
            anchors.rightMargin: 1
            anchors.leftMargin: 1
            anchors.bottomMargin: 1
            anchors.topMargin: 1

            TopBar{
                id: topBar

                Component.onCompleted: {
                    topBar.dragging.connect(Logic.moveSystem)
                }

                onToggleButtonClicked: leftMenu.expandMenu()
                onMinimize: Logic.showMinimized()
                onMaximizeRestore: Logic.maximizeRestore()
                onClosing: Qt.quit()
            }

            Rectangle {
                id: content
                color: "#00000000"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: topBar.bottom
                anchors.bottom: parent.bottom
                clip: true
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0

                LeftMenu{
                    id: leftMenu

                    Component.onCompleted: {
                        leftMenu.buttonClicked.connect(contentPages.setCurrentPage)
                    }

                    onSettingsButtonClicked: contentPages.setSettingsPage()
                }

                ContentPages{
                    id: contentPages
                }

            }
        }
    }


    MouseDragHandler {
        id: resizeLeft
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        cursorShape: Qt.SizeHorCursor
        anchors.leftMargin: 0
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        edge: Qt.LeftEdge
    }

    MouseDragHandler {
        id: resizeRight
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        cursorShape: Qt.SizeHorCursor
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        edge: Qt.RightEdge
    }

    MouseDragHandler {
        id: resizeRightCorner
        x: 950
        y: 527
        width: 40
        height: 43
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        cursorShape: Qt.SizeFDiagCursor
        edge: Qt.RightEdge | Qt.BottomEdge
    }

    MouseDragHandler {
        id: resizeBottom
        height: 10
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        cursorShape: Qt.SizeVerCursor
        anchors.bottomMargin: 0
        edge: Qt.BottomEdge
    }

}


