import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{
    id: popupTextInput

    width: 60
    height: 20
    color: "#00a1f1"
    visible: false
    opacity: 0.5

    signal textAccepted(string txt)

    function show(){
        popupTextInput.visible = true
        textEngine.focus = true
    }

    function hide(){
        popupTextInput.visible = false
        textEngine.focus = false
        textEngine.clear()
    }

    function tryHide(){
        if(textEngine.focus)
            popupTextInput.hide()
    }

    Action{
        shortcut: "Esc"
        onTriggered: popupTextInput.tryHide()
    }

    TextInput {
        id: textEngine
        color: "white"
        z: 2
        onAccepted: {
            popupTextInput.textAccepted(textEngine.text)
            popupTextInput.hide()
        }
    }
}
