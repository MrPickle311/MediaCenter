import QtQuick 2.15
import QtQuick.Controls 2.15

Slider {
    id: mediaSlider
    stepSize: 0.01

    function changePosition(newPos){
        value = newPos
    }
    signal positionMoved(real position)
    onMoved: positionMoved(position)
}
