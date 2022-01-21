import QtQuick 2.15
import QtQuick.Shapes 1.15
import Qt5Compat.GraphicalEffects

Item {
    id: progressBar

    implicitHeight: 250
    implicitWidth: 250
    visible: false

    function hide(){
        progressBar.visible = false
    }

    function show(){
        progressBar.visible = true
    }

    function increaseVolume(){
        progressBar.show()
        if(progressBar.value < 100)
            ++progressBar.value
    }

    function decreaseVolume(){
        progressBar.show()
        if(progressBar.value > 0)
            --progressBar.value
    }

    property bool roundCap: true
    property int startAngle: -90
    property real maxValue: 100
    property real value: 80
    property int samples: 12

    property color backgroundColor: "transparent"
    property color backgroundStrokeColor: "#7e7e7e"
    property int strokeBackgroundWidth: 5

    property color progressColor: "#ff007f"
    property int progressWidth: 5

    property string text: " % Volume"
    property bool textShowValue: true
    property string textFontFamily: "Segoe UI"
    property int textSize: 12
    property color textColor: "#7c7c7c"

    Shape{
        id: shape
        anchors.fill:  parent
        layer.enabled: true
        layer.samples: progressBar.samples

        ShapePath{
            id: backgroundPath

            strokeColor: progressBar.backgroundStrokeColor
            fillColor: progressBar.backgroundColor
            strokeWidth: progressBar.strokeBackgroundWidth
            capStyle: progressBar.roundCap ? ShapePath.RoundCap : ShapePath.FlatCap

            PathAngleArc{
                radiusX: progressBar.width / 2 - progressBar.progressWidth / 2
                radiusY: progressBar.height / 2 - progressBar.progressWidth / 2
                centerX: progressBar.width / 2
                centerY: progressBar.height / 2
                startAngle: progressBar.startAngle
                sweepAngle: 360
            }
        }

        ShapePath{
            id: path
            strokeColor: progressBar.progressColor
            fillColor: "transparent"
            strokeWidth: progressBar.progressWidth
            capStyle: progressBar.roundCap ? ShapePath.RoundCap : ShapePath.FlatCap

            PathAngleArc{
                radiusX: progressBar.width / 2 - progressBar.progressWidth / 2
                radiusY: progressBar.height / 2 - progressBar.progressWidth / 2
                centerX: progressBar.width / 2
                centerY: progressBar.height / 2
                startAngle: progressBar.startAngle
                sweepAngle: 360 / progressBar.maxValue * progressBar.value
            }
        }

        Text{
            id: barText
            text:  progressBar.textShowValue ? parseInt(progressBar.value) + progressBar.text : progressBar.text
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: progressBar.textColor
            font.pointSize: progressBar.textSize
            font.family: progressBar.textFontFamily
        }
    }
}
