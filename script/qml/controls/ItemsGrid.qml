import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.12

ScrollView{
    id: itemsGrid

    property alias externalDelegate: gridView.delegate
    property alias externalModel: gridView.model
    property int itemSize: 70

    signal itemClicked(url src)

    GridView {
        id: gridView
        anchors.fill: parent
        cellWidth: itemsGrid.itemSize
        cellHeight: itemsGrid.itemSize
    }
}
