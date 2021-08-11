import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

TextField{
    id: textField

    width: 40
    implicitHeight: 40

    property color colorDefault: "#282c34"
    property color colorOnFocus: "#202225"
    property color colorMouseOver: "#2B2F38"
    property color colorBorderOnFocus: "#ff007f"

    property int currentWidth: width

    //sending signal when user wants to search something
    signal searchRequested(string what)
    Keys.onReturnPressed: searchRequested(textField.text)

    placeholderText: "Search by"

    color: "#ffffff"
    rightPadding: 35
    leftPadding: 42
    clip: true
    font.family: "Segoe UI"

    font.pointSize: 10

    QtObject{
        id: internal

        property color dynamicColor: if(textField.focus)
                                         textField.focus ? colorOnFocus : colorDefault
                                     else textField.hovered ? colorMouseOver : colorDefault
    }

    background: Rectangle{
        id: backgroundColor
        color: internal.dynamicColor
        radius: 8
        border.color: colorDefault
        border.width: 2

        Image{
            id: searchIcon
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            source: "qrc:/data/search.svg"
            fillMode: Image.PreserveAspectFit
            sourceSize.height: 20

            opacity: 0.5
        }

        ColorOverlay{
            anchors.fill: searchIcon
            source: searchIcon
            color: "white"
            antialiasing: true

            width: searchIcon.width
            height: searchIcon.height
        }

        Image{
            id: clearIcon
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
            source: "qrc:/data/clear.svg"
            fillMode: Image.PreserveAspectFit
            sourceSize.height: 20
            opacity: 0.5
            visible: false
        }

        ColorOverlay{
            id: clearIconOverlay
            anchors.fill: clearIcon
            source: clearIcon
            color: "white"
            antialiasing: true

            width: clearIcon.width
            height: clearIcon.height
            visible: false
        }

        MouseArea{
            cursorShape: Qt.PointingHandCursor
            anchors.fill: clearIcon
            hoverEnabled: true
            onEntered: clearIcon.opacity = 0.75
            onExited: clearIcon.opacity = 0.5
            onClicked: textField.text = ""
            visible: clearIcon.visible
        }
    }

    function setClearIconVisibility(is_visible){
        clearIcon.visible = is_visible
        clearIconOverlay.visible = is_visible
    }

    onFocusChanged: {
        if(textField.focus){
            onTextFocus.running = true
            setClearIconVisibility(true)
            backgroundColor.border.color = colorBorderOnFocus
        }
        else {
            onLostFocus.running = true
            setClearIconVisibility(false)
            textField.text = ""
            backgroundColor.border.color = colorDefault
        }
    }

    selectByMouse: true
    selectedTextColor: "#FFFFFF"
    selectionColor: "#81848c"

    PropertyAnimation{
        id: onTextFocus
        target: textField
        properties: "width"
        to: currentWidth
        duration: 500
        easing.type: Easing.OutQuint

        onFinished: textField.anchors.right = parent.right
    }

    PropertyAnimation{
        id: onLostFocus
        target: textField
        properties: "width"
        to: 40
        duration: 500
        easing.type: Easing.OutQuint

        onStarted: textField.anchors.right = undefined
    }

}
