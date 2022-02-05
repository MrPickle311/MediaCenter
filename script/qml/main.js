function resetResizeBorders(bool){
    resizeLeft.visible = bool
    resizeRight.visible = bool
    resizeBottom.visible = bool
    resizeRightCorner.visible = bool
}

//sets window to non maximized state
function setNormalState(){
    windowStatus = 0
    windowMargin = 10
    resetResizeBorders(true)
}

//sets window to maximized state
function setMaximizedState(){
    windowStatus = 1
    windowMargin = 0
    resetResizeBorders(false)
}

function maximizeRestore(){
    if(windowStatus == 0){
        mainWindow.showMaximized()
        setMaximizedState()
    }
    else{
        mainWindow.showNormal()
        setNormalState()
    }
}

function ifMaximizedWindowRestore(){
    if(windowStatus == 1){
        mainWindow.showNormal()
        setNormalState()
    }
}

function restoreMargins(){
    setNormalState()
}

function showMinimized(){
    restoreMargins()
    mainWindow.showMinimized()
}

function moveSystem(isActive){
    if(isActive){
        mainWindow.startSystemMove()
        Logic.ifMaximizedWindowRestore()
    }
}

function trySetNormalMainWindowState(){
    if(mainWindow.visibility !== Window.Maximized ){
        topBar.setMaximizeIcon()
        Logic.setNormalState()
    }
}
