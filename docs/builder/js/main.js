var c = document.getElementById("canvas");
var ctx = c.getContext("2d");
var selectedLayer;
var fileLoading;

var db = {
    layers: 0,
    list: []
}

$(".tile-selector").on("click", function () {
    if (selectedLayer) {
        selectedLayer.removeClass("selected-tile");
    }
    selectedLayer = $(this);
    $(this).addClass("selected-tile");
});

function update() {
    db.layers = db.list.length;

    frontendUpdate();
    canvasUpdate();
}

function objectGenerator(imgType, image, xPos, yPos, width, height, mass, collider, rotation) {
    return obj = {
        type: imgType,
        img: image,
        x: xPos,
        y: yPos,
        w: width,
        h: height,
        m: mass,
        c: collider,
        r: rotation
    }
}