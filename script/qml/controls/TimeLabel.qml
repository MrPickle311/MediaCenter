import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    id: musicTimeLabel
    color: "#f2f2f2"

    function setTime(millis){
        var minutes = Math.floor(millis / 60000)
        var seconds = ((millis % 60000) / 1000).toFixed(0)
        text =  minutes + ":" + (seconds < 10 ? '0' : '') + seconds
    }
}
