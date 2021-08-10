import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

import "controls"
import "pages"

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

    property url restoreIcon:  "../../data/restore_icon.svg"
    property url maximizeIcon: "../../data/maximize_icon.svg"

    QtObject{
        id: internal

        //theses 2 functions set top-left bar maximize icon
        function setRestoreIcon(){
            maximizeRestoreButton.buttonIconSource = restoreIcon
        }

        function setMaximizeIcon(){
            maximizeRestoreButton.buttonIconSource = maximizeIcon
        }

        //it sets the following content to true or false at once
        function resetResizeBorders(bool){
            resizeLeft.visible = bool
            resizeRight.visible = bool
            resizeBottom.visible = bool
            resizeRightCorner.visible = bool
        }

        //sets window to non maximized state
        function setNormalState(){
            windowStatus = 0
            windowMargin = 10
            resetResizeBorders(true)
            setMaximizeIcon()
        }

        //sets window to maximized state
        function setMaximizedState(){
            windowStatus = 1
            windowMargin = 0
            resetResizeBorders(false)
            setRestoreIcon()
        }

        function maximizeRestore(){
            if(windowStatus == 0){
                mainWindow.showMaximized()
                setMaximizedState()
            }
            else{
                mainWindow.showNormal()
                setNormalState()
            }
        }

        function ifMaximizedWindowRestore(){
            if(windowStatus == 1){
                mainWindow.showNormal()
                setNormalState()
            }
        }

        function restoreMargins(){
            setNormalState()
        }
    }

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

                ToggleButton {
                    id: toggleBtn
                    width: 65
                    onClicked: animationMenu.running = true
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

                    PropertyAnimation{
                        id: animationMenu
                        target: leftMenu
                        property: "width"
                        to: leftMenu.width == 65 ? 250 : 65
                        duration: 500
                        easing.type: Easing.InOutQuint
                    }

                    DragHandler{
                        onActiveChanged: if(active){
                                             mainWindow.startSystemMove()
                                             internal.ifMaximizedWindowRestore()
                                         }
                    }

                    Image {
                        id: iconApp
                        width: 22
                        height: 22
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        source: "../../data/icon_app_top.svg"
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        anchors.topMargin: 0
                        fillMode: Image.PreserveAspectFit
                    }

                    Label {
                        id: label
                        x: 38
                        width: 765
                        color: "#c3cbdd"
                        text: qsTr("My App Title")
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

                    TopBarButton{
                        id: minimizeButton
                        onClicked: {
                            internal.restoreMargins()
                            mainWindow.showMinimized()
                        }
                    }

                    TopBarButton{
                        id: maximizeRestoreButton
                        buttonIconSource: "../../data/maximize_icon.svg"

                        onClicked: internal.maximizeRestore()
                    }

                    TopBarButton{
                        id: closeButton
                        pressedColor: "#ff007f"
                        buttonIconSource: "../../data/close_icon.svg"

                        onClicked: mainWindow.close()
                    }
                }
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

                    Column {
                        id: columnMenus
                        width: leftMenu.border.width
                        anchors.fill: parent
                        anchors.bottomMargin: 90

                        LeftMenuButton {
                            id: homeButton
                            text: qsTr("Home")

                            isActiveMenu: true

                            onClicked: {
                                homeButton.isActiveMenu = true
                                settingsButton.isActiveMenu = false

                                swipeView.setCurrentIndex(0)
                            }
                        }

                        LeftMenuButton {
                            id: openButton
                            text: qsTr("Open")
                            buttonIconSource: "../../data/open_icon.svg"
                        }

                        LeftMenuButton {
                            id: saveButton
                            text: qsTr("Save")
                            buttonIconSource: "../../data/save_icon.svg"
                        }
                    }

                    LeftMenuButton {
                        id: settingsButton
                        text: qsTr("Save")
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 25
                        buttonIconSource: "../../data/settings_icon.svg"

                        onClicked: {
                            homeButton.isActiveMenu = false
                            settingsButton.isActiveMenu = true

                            swipeView.setCurrentIndex(1)
                        }
                    }
                }

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

                    SwipeView {
                        id: swipeView
                        anchors.fill: parent

                        orientation: Qt.Vertical

                        HomePage{

                        }

                        VideoPage{

                        }
                    }
                }
            }
        }
    }

    DropShadow{
        anchors.fill: background

        horizontalOffset: 0
        verticalOffset: 0
        radius: 10
        samples: 16
        color: "#80000000"
        source: background
        z: 0
    }

    MouseArea {
        id: resizeLeft
        width: 10
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        cursorShape: Qt.SizeHorCursor
        anchors.leftMargin: 0
        anchors.bottomMargin: 10
        anchors.topMargin: 10

        DragHandler{
            target: null
            onActiveChanged: if(active) { mainWindow.startSystemResize(Qt.LeftEdge) }
        }
    }

    MouseArea {
        id: resizeRight
        width: 10
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        cursorShape: Qt.SizeHorCursor
        anchors.bottomMargin: 10
        anchors.topMargin: 10

        DragHandler{
            target: null
            onActiveChanged: if(active) { mainWindow.startSystemResize(Qt.RightEdge) }
        }
    }

    MouseArea {
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

        DragHandler{
            target: null
            onActiveChanged: if(active) { mainWindow.startSystemResize(Qt.RightEdge | Qt.BottomEdge) }
        }
    }

    MouseArea {
        id: resizeBottom
        height: 10
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        cursorShape: Qt.SizeVerCursor
        anchors.bottomMargin: 0

        DragHandler{
            target: null
            onActiveChanged: if(active) { mainWindow.startSystemResize(Qt.BottomEdge) }
        }

    }

}
