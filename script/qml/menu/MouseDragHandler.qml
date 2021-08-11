import QtQuick 2.15
import QtQuick.Controls 2.15

MouseArea {
    id: mouseDragHandler
    width: 10

    cursorShape: Qt.SizeHorCursor

    property int edge: 0
    property int shape: Qt.SizeHorCursor

    DragHandler{
        target: null
        onActiveChanged: if(active) { mainWindow.startSystemResize(edge) }
    }
}
