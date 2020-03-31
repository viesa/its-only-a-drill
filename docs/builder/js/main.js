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

$(document).on("change", '.lrval', function () {
    switch ($(this).attr("lrType")) {
        case "lrvx":
            imageList[$(this).parent().parent().attr("chnr")].x = $(this).val();
            break;

        case "lrvy":
            imageList[$(this).parent().parent().attr("chnr")].y = $(this).val();
            break;

        case "lrvw":
            imageList[$(this).parent().parent().attr("chnr")].w = $(this).val();
            break;

        case "lrvh":
            imageList[$(this).parent().parent().attr("chnr")].h = $(this).val();
            break;
    }
    update();
});

function lrdel(element) {
    imageList.splice(element.parent().parent().attr("chnr"), 1);
    update();
};

function update() {
    $("#layers-title").text("layers (" + imageList.length + ")");

    var container = $("#layers-container");
    container.empty();
    for (i = 0; i < imageList.length; i++) {
        var str1 = "<div class='layer-child' chnr='" + i + "'>";

        var str2 = "<div class='layer-child-box'><label>Layer [" + i + "]</label></div>";
        var str3 = "<div class='layer-child-box'><label>Type: " + imageList[i].type + "</label></div>";
        var str4 = "<div class='layer-child-box'><label>X</label><input type='number' class='lrval' lrType='lrvx' value='" + imageList[i].x + "'></div>";
        var str5 = "<div class='layer-child-box'><label>Y</label><input type='number' class='lrval' lrType='lrvy' value='" + imageList[i].y + "'></div>";
        var str6 = "<div class='layer-child-box'><label>W</label><input type='number' class='lrval' lrType='lrvw' value='" + imageList[i].w + "'></div>";
        var str7 = "<div class='layer-child-box'><label>H</label><input type='number' class='lrval' lrType='lrvh' value='" + imageList[i].h + "'></div>";
        var str8 = "<div class='layer-child-box'><button class='lrdel' onclick='lrdel($(this))'>Delete</button></div>";
        var str9 = "</div>";

        container.append(str1 + str2 + str3 + str4 + str5 + str6 + str7 + str8 + str9);
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
