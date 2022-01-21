import QtQuick 2.15

import "../logic/SvgGetter.js" as Logic

SquareButton {
    id: prevButton
    buttonIconSource: Logic.getControl("left-arrow.svg")
    defaultColor: "#00000000"
}
