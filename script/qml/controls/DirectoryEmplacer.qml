import QtQuick 2.15
//import QtQuick.Dialogs 1.3
import Qt.labs.platform 1.1

FolderDialog {
    id: emplacer

    title: "Choose a directory to add"

    signal directoryAdded(url dir)

    onAccepted: directoryAdded(folder)
}
