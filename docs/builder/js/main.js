var c = document.getElementById("canvas");
var ctx = c.getContext("2d");
var selectedLayer;
var fileLoading;

var db = {
    mapInfo: {
        uid: 0,
        name: "Test map",
        difficulty: 0,
        maxPlayers: 0,
        maxBots: 0
    },
    layers: 0,
    list: []
}

$(window).on("load", function () {
    $("#tile-container").children().first().trigger("click");
    $.getJSON("./js/types.json", function (json) {

        for (i = 0; i < json.types.length; i++) {
            var str1 = '<div class="tiles-child-box"><span>' + json.types[i].name + '</span>';
            var str2 = '<img src="../spritesheets/background-tiles.png" id="img-' + json.types[i].name + '" title="' + json.types[i].name + ': 16x16px" type="' + json.types[i].name + '" sx="' + json.types[i].x + '" sy="' + json.types[i].y + '" sw="' + json.types[i].w + '" sh="' + json.types[i].h + '" mass="1" collider="0" class="tile-selector" style="display:none">';
            var str3 = '</div>';
            $("#tile-container").append(str1 + str2 + str3);
        };
    });

    $('#canvas-container').scrollTop(1028 - $(window).height() / 2);
    $('#canvas-container').scrollLeft(1028 - $(window).height() / 2);

    db.mapInfo.uid = Math.round(Math.random() * 1000000);
});

$(document).on("click", '.tiles-child-box', function () {
    if (selectedLayer) {
        selectedLayer.parent().removeClass("selected-tile");
    }
    $(this).addClass("selected-tile");
    selectedLayer = $(this).find(".tile-selector");
});

function update() {
    db.layers = db.list.length;

    frontendUpdate();
    canvasUpdate();
}

function objectGenerator(imgType, image, xPos, yPos, width, height, mass, collider, rotation, sx, sy, sw, sh) {
    return obj = {
        type: imgType,
        img: image,
        x: xPos,
        y: yPos,
        w: width,
        h: height,
        m: mass,
        c: collider,
        r: rotation,
        src: {
            x: sx,
            y: sy,
            w: sw,
            h: sh
        }
    }
}

function showMapInfo() {
    $("#mapNameInput").val(db.mapInfo.name);
    $("#maxPlayersInput").val(db.mapInfo.maxPlayers);
    $("#maxBotsInput").val(db.mapInfo.maxBots);
    $("#difficultyInput").val(db.mapInfo.difficulty);
    $("#uidInput").val(db.mapInfo.uid);
    $('#mapInfoContainer').fadeIn();
}

function saveMapInfo() {
    db.mapInfo.name = $("#mapNameInput").val();
    db.mapInfo.difficulty = $("#difficultyInput").val();
    db.mapInfo.uid = $("#uidInput").val();
    $('#mapInfoContainer').fadeOut();
}