import QtQuick 2.15

SquareButton{
    id: playButton

    property string pauseIcon: "pause.svg"
    property string playIcon: "play.svg"

    defaultColor: "#00000000"
    buttonIconSource: playIcon

    function updateIconSource(){
        buttonIconSource = buttonIconSource === playIcon ? pauseIcon : playIcon
    }

    onClicked: updateIconSource()
}
