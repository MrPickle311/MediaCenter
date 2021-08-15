import QtQuick 2.15
import QtQuick.Controls 2.15

import "../delegates"

ModalItemList{
    id: flatStringList

     signal selected(string str)
     signal deselected(string str)

    externalDelegate: StringItemDelegate{
        width: flatStringList.width
        str: text
        onSelected: flatStringList.selected(str)
        onDeselected: flatStringList.deselected(str)
    }
}
