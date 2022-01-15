var Iterator = function (items) {
    this.index = 0;
    this.items = items;
}


class HardItem extends Object{
    constructor(parent){
        super(parent)
    }
    show(){
        var ty = new Promise()
        console.log("ok")
    }
}
