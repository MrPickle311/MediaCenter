import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

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

    function selectDeselectThisEntity(){
        if(!isSelected) stringDelegate.border.color = "#55aaff"
        else stringDelegate.border.color = "transparent"
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
