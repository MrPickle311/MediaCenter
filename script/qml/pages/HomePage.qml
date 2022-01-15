import QtQuick 2.15
import QtQuick.Controls 2.15

import "../controls"

Item {
    Rectangle {
        id: rectangle
        color: "lightblue"//"#55aaff"
        anchors.fill: parent

        ItemsGrid{
            id: recently

            externalModel: ListModel{

            }
        }

    }
}
