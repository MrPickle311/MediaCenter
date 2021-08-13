import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    id: title
    color: "#dcdfe2"
    text: qsTr("Title")
    wrapMode: Text.NoWrap
    textFormat: Text.AutoText
    font.pointSize: 12

    function setTitle(source){
        text = source.split('/').pop()
    }
}
