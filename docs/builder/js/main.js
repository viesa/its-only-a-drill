var imageList = [];

var c = document.getElementById("canvas");
var ctx = c.getContext("2d");
var selectedLayer;
var fileLoading;

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

        imageList.push(objectGenerator(selectedLayer.attr("type"), document.getElementById(selectedLayer.attr("id")), pos.x - width / 2, pos.y - height / 2, width, height, selectedLayer.attr("mass"), selectedLayer.attr("collider"), 0));

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

        case "lrvm":
            imageList[$(this).parent().parent().attr("chnr")].m = $(this).val();
            break;

        case "lrvc":
            imageList[$(this).parent().parent().attr("chnr")].c = $(this).val();
            break;

        case "lrvr":
            imageList[$(this).parent().parent().attr("chnr")].r = $(this).val();
            break;
    }
    update();
});

function lrdel(element) {
    imageList.splice(element.parent().parent().attr("chnr"), 1);
    update();
};

function update() {

    var container = $("#layers-container");
    container.empty();
    for (i = 0; i < imageList.length; i++) {
        var str1 = "<div class='layer-child' chnr='" + i + "'>";

        var str2 = "<div class='layer-child-box'><label>Layer [" + i + "]</label></div>";
        var str3 = "<div class='layer-child-box'><label>Type: " + imageList[i].type + "</label></div>";
        var str4 = "<div class='layer-child-box'><label>X</label><input type='number' class='lrval' lrType='lrvx' value='" + imageList[i].x + "'></div>";
        var str5 = "<div class='layer-child-box'><label>Y</label><input type='number' class='lrval' lrType='lrvy' value='" + imageList[i].y + "'></div>";
        var str6 = "<div class='layer-child-box'><label>Width</label><input type='number' class='lrval' lrType='lrvw' value='" + imageList[i].w + "'></div>";
        var str7 = "<div class='layer-child-box'><label>Height</label><input type='number' class='lrval' lrType='lrvh' value='" + imageList[i].h + "'></div>";
        var str8 = "<div class='layer-child-box'><label>Mass</label><input type='number' class='lrval' lrType='lrvm' value='" + imageList[i].m + "'></div>";
        var str9 = "<div class='layer-child-box'><label>Collider</label><input type='number' class='lrval' lrType='lrvc' value='" + imageList[i].c + "'></div>";
        var str10 = "<div class='layer-child-box'><label>Rotation</label><input type='number' class='lrval' lrType='lrvr' value='" + imageList[i].r + "'></div>";
        var str11 = "<div class='layer-child-box'><button class='lrdel' onclick='lrdel($(this))'>Delete</button></div>";
        var str12 = "</div>";

        container.append(str1 + str2 + str3 + str4 + str5 + str6 + str7 + str8 + str9 + str10 + str11 + str12);
    }

    clearCanvas();
    for (i = 0; i < imageList.length; i++) {
        drawRotatedImage(imageList[i].img, imageList[i].x, imageList[i].y, imageList[i].w, imageList[i].h, imageList[i].r);
    }
}

// Code from https://www.html5rocks.com/en/tutorials/file/dndfiles/
function readBlob() {

    var files = document.getElementById('files').files;
    if (!files.length) {
        alert('Please select a file!');
        return;
    }

    var file = files[0];
    var reader = new FileReader();

    // If we use onloadend, we need to check the readyState.
    reader.onloadend = function (evt) {
        if (evt.target.readyState == FileReader.DONE) { // DONE == 2
            document.getElementById('byte_content').textContent = evt.target.result;
        }
    };

    var blob = file.slice(0, file.length);
    reader.readAsBinaryString(blob);
}

function outputJSON() {
    var jsonString = JSON.stringify(imageList);
    download(jsonString, "level.json", "json");
}

// Code from https://gamedev.stackexchange.com/questions/67274/is-it-possible-to-rotate-an-image-on-an-html5-canvas-without-rotating-the-whole
var TO_RADIANS = Math.PI / 180;
function drawRotatedImage(image, x, y, w, h, angle) {
    // save the current co-ordinate system 
    // before we screw with it
    ctx.save();

    // move to the middle of where we want to draw our image
    ctx.translate(x, y);

    // rotate around that point, converting our 
    // angle from degrees to radians 
    ctx.rotate(angle * TO_RADIANS);

    // draw it up and to the left by half the width
    // and height of the image 
    ctx.drawImage(image, -(w / 2), -(h / 2));

    // and restore the co-ords to how they were when we began
    ctx.restore();
}

// Code from https://stackoverflow.com/questions/13405129/javascript-create-and-save-file
function download(data, filename, type) {
    var file = new Blob([data], { type: type });
    if (window.navigator.msSaveOrOpenBlob) // IE10+
        window.navigator.msSaveOrOpenBlob(file, filename);
    else { // Others
        var a = document.createElement("a"),
            url = URL.createObjectURL(file);
        a.href = url;
        a.download = filename;
        document.body.appendChild(a);
        a.click();
        setTimeout(function () {
            document.body.removeChild(a);
            window.URL.revokeObjectURL(url);
        }, 0);
    }
}

function clearCanvas() {
    setCanvasSize();
    const context = canvas.getContext('2d');
    context.clearRect(0, 0, canvas.width, canvas.height);
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
