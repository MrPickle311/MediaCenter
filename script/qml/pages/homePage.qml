import QtQuick 2.15
import QtQuick.Controls 2.15


Item {
    Rectangle {
        id: rectangle
        color: "#55aaff"
        anchors.fill: parent

        Label {
            id: label
            x: 196
            y: 141
            text: qsTr("Home page")
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 16
        }
    }

}
