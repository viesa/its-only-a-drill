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
        var str11 = "<div class='layer-child-box'><button class='lrdel' onclick='lrdel($(this))'>Delete</button></div>";
        var str12 = "</div>";

        container.append(str1 + str2 + str3 + str4 + str5 + str6 + str7 + str8 + str9 + str10 + str11 + str12);
    }
}

$(document).on("change", '.lrval', function () {
    switch ($(this).attr("lrType")) {
        case "lrvx":
            db.list[$(this).parent().parent().attr("chnr")].x = $(this).val();
            break;

        case "lrvy":
            db.list[$(this).parent().parent().attr("chnr")].y = $(this).val();
            break;

        case "lrvw":
            db.list[$(this).parent().parent().attr("chnr")].w = $(this).val();
            break;

        case "lrvh":
            db.list[$(this).parent().parent().attr("chnr")].h = $(this).val();
            break;

        case "lrvm":
            db.list[$(this).parent().parent().attr("chnr")].m = $(this).val();
            break;

        case "lrvc":
            db.list[$(this).parent().parent().attr("chnr")].c = $(this).val();
            break;

        case "lrvr":
            db.list[$(this).parent().parent().attr("chnr")].r = $(this).val();
            break;
    }
    update();
});

function lrdel(element) {
    db.list.splice(element.parent().parent().attr("chnr"), 1);
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
        var width = selectedLayer.width();
        var height = selectedLayer.height();

        db.list.push(objectGenerator(selectedLayer.attr("type"), document.getElementById(selectedLayer.attr("id")), pos.x, pos.y, width, height, selectedLayer.attr("mass"), selectedLayer.attr("collider"), 0));

        update();
    }
    else {
        alert("You need to select a type first.");
    }
});