import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.12

Rectangle {
    id: results
    visible: false

    property var externalDelegate: null
    property var externalModel: null


    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#434343"
        }

        GradientStop {
            position: 1
            color: "#000000"
        }
    }

    ScrollView {
            anchors.fill: results
            clip: true
            ListView {
                    anchors.fill: parent
                    model: externalModel
            }
    }

    DelegateModel {
            id: delegateModel
            model: externalModel
            delegate: externalDelegate
    }

    PropertyAnimation{
        id: onTextFocus
        target: results
        properties: "height"
        //to: currentWidth
        duration: 300
        easing.type: Easing.OutQuint
        //onFinished: textField.anchors.right = parent.right
    }


}
