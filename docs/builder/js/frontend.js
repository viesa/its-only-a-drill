function frontendUpdate() {
    var container = $("#layers-container");
    container.empty();
    db.mapInfo.maxPlayers = 0;
    db.mapInfo.maxBots = 0;
    for (i = 0; i < db.list.length; i++) {

        if (db.list[i].type == "player_spawn") {
            db.mapInfo.maxPlayers += 1;
            $("#maxPlayersInput").val(db.mapInfo.maxPlayers);
        }
        if (db.list[i].type == "enemy_spawn") {
            db.mapInfo.maxBots += 1;
            $("#maxBotsInput").val(db.mapInfo.maxBots);
        }

        var str1 = "<div class='layer-child' chnr='" + i + "'>";

        var str2 = "<div class='layer-child-first'><div class='layer-child-info'><label>Z-Index: " + i + "</label>";
        var str3 = "<label>Type: " + db.list[i].type + "</label></div><div class='layer-child-options'>";
        var str4 = "";
        //"<button class='lrbutton lrexpand'>Options</button>";
        var str5 = "<button class='lrbutton lrdel' onclick='lrdel($(this))'><i class='fas fa-trash-alt'></i></button>";
        var str6 = "";
        //var str6 = "<button class='lrbutton lrhigh' onclick='highLight($(this).parent().parent().parent())'>Highlight</button>";

        if (i > 0) {
            var str7 = "<button class='lrbutton lrup lrarrow' onclick='lrup($(this))'><i class='fas fa-arrow-up'></i></button>";
        } else {
            var str7 = "<button class='lrbutton lrup lrarrow lrdisabled' disabled onclick='lrup($(this))'><i class='fas fa-arrow-up'></i></button>";
        }

        if (i < db.list.length - 1) {
            var str8 = "<button class='lrbutton lrdown lrarrow' onclick='lrdown($(this))'><i class='fas fa-arrow-down'></i></button>";
        } else {
            var str8 = "<button class='lrbutton lrdown lrarrow lrdisabled' disabled onclick='lrdown($(this))'><i class='fas fa-arrow-down'></i></button>";
        }
        var str9 = "</div></div><div class='layer-child-expanded' style='display:none'>";
        var str10 = "<div class='layer-child-box'><label>X</label><input type='number' class='lrval' lrType='lrvx' value='" + db.list[i].x + "'></div>";
        var str11 = "<div class='layer-child-box'><label>Y</label><input type='number' class='lrval' lrType='lrvy' value='" + db.list[i].y + "'></div>";
        var str12 = "<div class='layer-child-box'><label>Width</label><input type='number' class='lrval' lrType='lrvw' value='" + db.list[i].w + "'></div>";
        var str13 = "<div class='layer-child-box'><label>Height</label><input type='number' class='lrval' lrType='lrvh' value='" + db.list[i].h + "'></div>";
        var str14 = "<div class='layer-child-box'><label>Mass</label><input type='number' class='lrval' lrType='lrvm' value='" + db.list[i].m + "'></div>";
        var str15 = "<div class='layer-child-box'><label>Collider</label><input type='number' class='lrval' lrType='lrvc' value='" + db.list[i].c + "'></div>";
        var str16 = "<div class='layer-child-box'><label>Rotation</label><input type='number' class='lrval' lrType='lrvr' value='" + db.list[i].r + "'></div>";
        var str17 = "</div></div>";

        container.append(str1 + str2 + str3 + str4 + str5 + str6 + str7 + str8 + str9 + str10 + str11 + str12 + str13 + str14 + str15 + str16 + str17);
    }
}

$(document).on("change", '.lrval', function () {
    switch ($(this).attr("lrType")) {
        case "lrvx":
            db.list[$(this).parent().parent().parent().attr("chnr")].x = parseInt($(this).val());
            break;

        case "lrvy":
            db.list[$(this).parent().parent().parent().attr("chnr")].y = parseInt($(this).val());
            break;

        case "lrvw":
            db.list[$(this).parent().parent().parent().attr("chnr")].w = parseInt($(this).val());
            break;

        case "lrvh":
            db.list[$(this).parent().parent().parent().attr("chnr")].h = parseInt($(this).val());
            break;

        case "lrvm":
            db.list[$(this).parent().parent().parent().attr("chnr")].m = parseInt($(this).val());
            break;

        case "lrvc":
            db.list[$(this).parent().parent().parent().attr("chnr")].c = parseInt($(this).val());
            break;

        case "lrvr":
            db.list[$(this).parent().parent().parent().attr("chnr")].r = parseInt($(this).val());
            break;
    }
    canvasUpdate();
});

function lrdel(element) {
    db.list.splice(element.parent().parent().parent().attr("chnr"), 1);
    update();
}

function lrup(element) {
    var index = parseInt(element.parent().parent().parent().attr("chnr"));
    var a = db.list[index - 1];
    db.list[index - 1] = db.list[index];
    db.list[index] = a;
    update();
}

function lrdown(element) {
    var index = parseInt(element.parent().parent().parent().attr("chnr"));
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

        db.list.push(objectGenerator(selectedLayer.attr("type"), document.getElementById(selectedLayer.attr("id")), parseInt(mouseX), parseInt(mouseY), cvW * 2, cvH * 2, parseInt($("#defaultMassInput").val()), parseInt($("#defaultColliderInput").val()), parseInt(0), cvX, cvY, cvW, cvH));

        update();
        $('#layers-container').scrollTop($('#layers-container')[0].scrollHeight);
    }
    else {
        alert("You need to select a type first.");
    }
});

function highLight(object) {
    canvasUpdate();
    var hoverObj = db.list[object.attr("chnr")];
    drawHighlightRectangle(hoverObj.x, hoverObj.y, hoverObj.w, hoverObj.h, hoverObj.r);
};

$(document).on("click", '.layer-child-first', function () {
    $(this).parent().find(".layer-child-expanded").slideToggle("fast");
});

$(document).on('mouseenter', '.layer-child', function (event) {
    highLight($(this));
}).on('mouseleave', '.layer-child', function () {
    canvasUpdate();
});

function centerCanvas() {
    $('#canvas-container').scrollTop(1028 - $(window).height() / 2);
    $('#canvas-container').scrollLeft(1028 - $(window).height() / 2);
}