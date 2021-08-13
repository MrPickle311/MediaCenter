import QtQuick 2.15

SquareButton{
    id: playButton

    property url pauseIcon: "qrc:/data/pause.svg"
    property url playIcon: "qrc:/data/play.svg"

    defaultColor: "#00000000"
    buttonIconSource: playIcon

    function updateIconSource(){
        buttonIconSource = buttonIconSource === playIcon ? pauseIcon : playIcon
    }

    onClicked: updateIconSource()
}
