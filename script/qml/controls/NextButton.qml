import QtQuick 2.15

import "../logic/SvgGetter.js" as Logic

SquareButton {
    id: nextButton
    buttonIconSource: Logic.getControl("right-arrow.svg")
    defaultColor: "#00000000"
}
