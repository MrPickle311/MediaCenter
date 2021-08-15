import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

import "../controls"
import "../delegates"

Item {

    Rectangle {
        id: rectangle
        color: "#191919"
        anchors.fill: parent
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        FlatStringList{
            id: directoryList
            width: rectangle.width / 2
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            title: "Directories"

            onAddRequested: emplacer.open()
        }

        DirectoryEmplacer{
            id: emplacer
            //C++ bind
            //onDirectoryAdded: console.log(dir)
        }

        RowLayout {
            id: gridLayout
            anchors.left: directoryList.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: joystickSensibility.top
            anchors.bottomMargin: 80
            clip: true
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.topMargin: 10
            visible: true
            spacing: 5

            FlatStringList {
                id: imagesExtensions
                height: 189
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: "Images Extensions"
            }

            FlatStringList {
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: videoExtensions
                height: 184
                width: gridLayout.recWidth
                title: "Video Extensions"
            }

            FlatStringList {
                Layout.fillWidth: true
                Layout.fillHeight: true
                id: audioExtensions
                height: 194
                width: gridLayout.recWidth
                title: "Audio Extensions"
            }
        }

        Slider {
            id: joystickSensibility
            y: 264
            anchors.left: directoryList.right
            anchors.right: parent.right
            anchors.bottom: volumeSensibility.top
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            anchors.bottomMargin: 20
            value: 0.5
        }

        Slider {
            id: volumeSensibility
            y: 352
            anchors.left: directoryList.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.leftMargin: 20
            anchors.bottomMargin: 30
            anchors.rightMargin: 20
            value: 0.5
        }

        Label {
            id: label
            y: 328
            color: "#f2f2f2"
            text: qsTr("Joystick sensibility")
            anchors.left: joystickSensibility.left
            anchors.bottom: joystickSensibility.top
            anchors.bottomMargin: 10
            anchors.leftMargin: 0
        }

        Label {
            id: label1
            y: 396
            color: "#f2f2f2"
            text: qsTr("Volume sensibility")
            anchors.left: volumeSensibility.left
            anchors.bottom: volumeSensibility.top
            anchors.bottomMargin: 10
            anchors.leftMargin: 0
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:9}D{i:10}
}
##^##*/
