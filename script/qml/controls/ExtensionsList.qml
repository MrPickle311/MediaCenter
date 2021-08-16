import QtQuick 2.15
import QtQuick.Controls 2.15

FlatStringList {
    id: flatStringList

    signal extensionAdded(string extension)

    onAddRequested: popupTextInput.show()

    PopupTextInput{
        id: popupTextInput
        anchors.top: flatStringList.top
        anchors.topMargin: 20
        anchors.left: flatStringList.left
        anchors.right: flatStringList.right
        height: 20
        onTextAccepted: flatStringList.extensionAdded(txt)
        z: 2
    }
}
