import QtQuick 2.15

import "../logic/SvgGetter.js" as Logic

SquareButton{
    id: playButton

    property string pauseIcon: Logic.getControl("pause.svg")
    property string playIcon: Logic.getControl("play.svg")

    defaultColor: "#00000000"
    buttonIconSource: playIcon

    function updateIconSource(){
        buttonIconSource = buttonIconSource === playIcon ? pauseIcon : playIcon
    }

    onClicked: updateIconSource()
}
