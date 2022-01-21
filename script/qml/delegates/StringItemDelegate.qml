import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects

import "../controls"

Rectangle {
    id: stringDelegate
    color: "transparent"
    border.width: 2
    border.color: "transparent"
    height: 35
    visible: true

    property string str: ""
    property bool isSelected: false

    signal selected(string txt)
    signal deselected(string txt)

    function select(){
        stringDelegate.border.color = "#55aaff"
        stringDelegate.isSelected = true
        stringDelegate.selected(str)
    }

    function deselect(){
        stringDelegate.isSelected = false
        stringDelegate.border.color = "transparent"
        stringDelegate.deselected(str)
    }

    function selectDeselectThisEntity(){
        if(stringDelegate.isSelected) stringDelegate.deselect()
        else stringDelegate.select()
    }

    TextArea {
        visible: true
        id: textArea
        height: 40
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.NoWrap
        placeholderTextColor: "#7fffffff"
        readOnly: true
        placeholderText: str
        anchors.fill: parent
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onClicked:selectDeselectThisEntity()
    }
}
