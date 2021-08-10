import QtQuick 2.15
import QtQuick.Controls 2.15


Item {
    Rectangle {
        id: rectangle
        color: "#191919"
        anchors.fill: parent

        Label {
            id: label
            x: 196
            y: 141
            color: "white"
            text: qsTr("Settings page")
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 16
        }
    }

}
