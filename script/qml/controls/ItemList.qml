import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.12

Rectangle {
    id: results
    visible: false

    property alias externalDelegate: delegateModel.delegate
    property alias externalModel: delegateModel.model

    function showHide(predicate){
        if(predicate) visible = true
        else visible = false
    }

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
                    model: delegateModel
            }
    }

    DelegateModel {
            id: delegateModel
    }

}
