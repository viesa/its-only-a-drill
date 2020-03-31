var imageList = [];

var c = document.getElementById("canvas");
var ctx = c.getContext("2d");
var selectedLayer;

$(window).on("load", function () {


});

$(".tile-selector").on("click", function () {
    if (selectedLayer) {
        selectedLayer.removeClass("selected-tile");
    }
    selectedLayer = $(this);
    $(this).addClass("selected-tile");
});

$("#canvas").on("click", function () {
    if (selectedLayer) {

        var pos = getMousePos(c, window.event);
        var width = selectedLayer.width();
        var height = selectedLayer.height();

        imageList.push(objectGenerator(selectedLayer.attr("type"), document.getElementById(selectedLayer.attr("id")), pos.x - width / 2, pos.y - height / 2, width, height));

        update();
    }
    else {
        alert("You need to select a type first.");
    }
});

function update() {
    $("#layers-title").text("layers (" + imageList.length + ")");

    var container = $("#layers-container");
    container.empty();
    for (i = 0; i < imageList.length; i++) {
        container.append("<p class='layer-child'> Layer [" + i + "], Type: " + imageList[i].type + " X<input type='number' class='lrval lrvx' value='" + imageList[i].x + "'> Y<input type='number' class='lrval lrvy' value='" + imageList[i].y + "'> Width<input type='number' class='lrval lrvw' value='" + imageList[i].w + "'> Height<input type='number' class='lrval lrvh' value='" + imageList[i].h + "'></p>");
    }

    clearCanvas();
    for (i = 0; i < imageList.length; i++) {
        ctx.drawImage(imageList[i].img, imageList[i].x, imageList[i].y, imageList[i].w, imageList[i].h);
    }
}

function clearCanvas() {
    setCanvasSize();
    const context = canvas.getContext('2d');
    context.clearRect(0, 0, canvas.width, canvas.height);
}

function objectGenerator(imgType, image, xPos, yPos, width, height) {
    return obj = {
        type: imgType,
        img: image,
        x: xPos,
        y: yPos,
        w: width,
        h: height
    }
}

function getMousePos(canvas, evt) {
    var rect = canvas.getBoundingClientRect();
    return {
        x: evt.clientX - rect.left,
        y: evt.clientY - rect.top
    };
}

var setCanvasSize = function () {
    canvas.width = window.innerHeight;
    canvas.height = window.innerHeight;
}
