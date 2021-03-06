import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.12

Item {
    id: modalItemList

    property string title: "Title"
    property alias externalDelegate: itemList.externalDelegate
    property alias externalModel: itemList.externalModel

    signal addRequested()
    signal removeRequested()

    TitleLabel{
        id: titleLabel
        anchors.horizontalCenter: parent.horizontalCenter
        text: modalItemList.title
    }

    ItemList{
        id: itemList
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: addButton.top
        anchors.bottomMargin: 10
        anchors.topMargin: 20

        visible: true
        border{
            width: 1
            color: "#00a1f1"
        }
    }

    SquareButton{
        id: addButton
        y: 375
        width: 50
        height: 50
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        defaultColor: "#00000000"
        anchors.bottomMargin: 10
        buttonIconSource: "add.svg"
        onClicked: modalItemList.addRequested()
    }

    SquareButton{
        id: removeButton
        y: 366
        width: 50
        height: 50
        anchors.left: addButton.right
        anchors.bottom: parent.bottom
        defaultColor: "#00000000"
        anchors.bottomMargin: 10
        buttonIconSource: "minus.svg"
        onClicked: modalItemList.removeRequested()
    }
}
