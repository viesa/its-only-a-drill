function frontendUpdate() {
    var container = $("#layers-container");
    container.empty();
    for (i = 0; i < db.list.length; i++) {
        var str1 = "<div class='layer-child' chnr='" + i + "'>";

        var str2 = "<div class='layer-child-box'><label>Layer [" + i + "]</label></div>";
        var str3 = "<div class='layer-child-box'><label>Type: " + db.list[i].type + "</label></div>";
        var str4 = "<div class='layer-child-box'><label>X</label><input type='number' class='lrval' lrType='lrvx' value='" + db.list[i].x + "'></div>";
        var str5 = "<div class='layer-child-box'><label>Y</label><input type='number' class='lrval' lrType='lrvy' value='" + db.list[i].y + "'></div>";
        var str6 = "<div class='layer-child-box'><label>Width</label><input type='number' class='lrval' lrType='lrvw' value='" + db.list[i].w + "'></div>";
        var str7 = "<div class='layer-child-box'><label>Height</label><input type='number' class='lrval' lrType='lrvh' value='" + db.list[i].h + "'></div>";
        var str8 = "<div class='layer-child-box'><label>Mass</label><input type='number' class='lrval' lrType='lrvm' value='" + db.list[i].m + "'></div>";
        var str9 = "<div class='layer-child-box'><label>Collider</label><input type='number' class='lrval' lrType='lrvc' value='" + db.list[i].c + "'></div>";
        var str10 = "<div class='layer-child-box'><label>Rotation</label><input type='number' class='lrval' lrType='lrvr' value='" + db.list[i].r + "'></div>";
        var str11 = "<div class='layer-child-box'><button class='lrbutton lrdel' onclick='lrdel($(this))'>Delete</button><button class='lrbutton lrhigh' onclick='highLight($(this))'>Highlight</button></div>";
        var str12 = "<div class='layer-child-box'><button class='lrbutton lrup' onclick='lrup($(this))'>Up</button><button class='lrbutton lrdown' onclick='lrdown($(this))'>Down</button></div>";
        var str13 = "</div>";

        container.append(str1 + str2 + str3 + str4 + str5 + str6 + str7 + str8 + str9 + str10 + str11 + str12 + str13);
    }
}

$(document).on("change", '.lrval', function () {
    switch ($(this).attr("lrType")) {
        case "lrvx":
            db.list[$(this).parent().parent().attr("chnr")].x = parseInt($(this).val());
            break;

        case "lrvy":
            db.list[$(this).parent().parent().attr("chnr")].y = parseInt($(this).val());
            break;

        case "lrvw":
            db.list[$(this).parent().parent().attr("chnr")].w = parseInt($(this).val());
            break;

        case "lrvh":
            db.list[$(this).parent().parent().attr("chnr")].h = parseInt($(this).val());
            break;

        case "lrvm":
            db.list[$(this).parent().parent().attr("chnr")].m = parseInt($(this).val());
            break;

        case "lrvc":
            db.list[$(this).parent().parent().attr("chnr")].c = parseInt($(this).val());
            break;

        case "lrvr":
            db.list[$(this).parent().parent().attr("chnr")].r = parseInt($(this).val());
            break;
    }
    update();
});

function lrdel(element) {
    db.list.splice(element.parent().parent().attr("chnr"), 1);
    update();
}

function lrup(element) {
    var index = parseInt(element.parent().parent().attr("chnr"));
    var a = db.list[index - 1];
    db.list[index - 1] = db.list[index];
    db.list[index] = a;
    update();
}

function lrdown(element) {
    var index = parseInt(element.parent().parent().attr("chnr"));
    var a = db.list[index];
    db.list[index] = db.list[index + 1];
    db.list[index + 1] = a;
    update();
}

function getMousePos(canvas, evt) {
    var rect = canvas.getBoundingClientRect();
    return {
        x: evt.clientX - rect.left,
        y: evt.clientY - rect.top
    };
}

$("#canvas").on("click", function () {
    if (selectedLayer) {

        var pos = getMousePos(c, window.event);
        var mouseX;
        var mouseY;
        if ($("#snapCheckBox").is(":checked")) {
            mouseX = Math.ceil((parseInt(pos.x)) / 32) * 32 - 32;
            mouseY = Math.ceil((parseInt(pos.y)) / 32) * 32 - 32;
        } else {
            mouseX = pos.x;
            mouseY = pos.y;
        }
        var cvX = parseInt(selectedLayer.attr("sx"));
        var cvY = parseInt(selectedLayer.attr("sy"));
        var cvW = parseInt(selectedLayer.attr("sw"));
        var cvH = parseInt(selectedLayer.attr("sh"));
        //center of mouse parseInt(pos.x - cvW / 2), parseInt(pos.y - cvH / 2)

        db.list.push(objectGenerator(selectedLayer.attr("type"), document.getElementById(selectedLayer.attr("id")), parseInt(mouseX), parseInt(mouseY), cvW * 2, cvH * 2, parseInt(selectedLayer.attr("mass")), parseInt(selectedLayer.attr("collider")), parseInt(0), cvX, cvY, cvW, cvH));

        update();
    }
    else {
        alert("You need to select a type first.");
    }
});

function highLight(object) {
    update();
    var hoverObj = db.list[object.parent().parent().attr("chnr")];
    drawHighlightRectangle(hoverObj.x, hoverObj.y, hoverObj.w, hoverObj.h, hoverObj.r);
};