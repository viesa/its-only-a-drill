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

function drawHighlightRectangle(x, y, w, h, deg) {
    ctx.beginPath();
    ctx.rect(x, y, w, h);
    ctx.rotate(deg * TO_RADIANS);
    ctx.lineWidth = "10";
    ctx.strokeStyle = "#1a73d9";
    ctx.stroke();
}

var setCanvasSize = function () {
    canvas.width = 2048; //window.innerHeight;
    canvas.height = 2048; //window.innerHeight;
}

function clearCanvas() {
    setCanvasSize();
    const context = canvas.getContext('2d');
    context.clearRect(0, 0, canvas.width, canvas.height);
}

function canvasUpdate() {
    clearCanvas();
    //ctx.drawImage(document.getElementById("background-tiles-sheet"), 0, 48, 16, 16, 0, 0, 16, 16);
    for (i = 0; i < db.list.length; i++) {
        ctx.drawImage(db.list[i].img, db.list[i].src.x, db.list[i].src.y, db.list[i].src.w, db.list[i].src.h, db.list[i].x, db.list[i].y, db.list[i].w, db.list[i].h);
    }
}