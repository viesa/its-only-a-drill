/// UPLOAD
var readObj = {};

//Interface for readBlob();
function loadJSON() {
    readBlob();
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
            readObj = JSON.parse(evt.target.result);
            for (i = 0; i < readObj.list.length; i++) {
                readObj.list[i].img = document.getElementById("img-" + readObj.list[i].type);
            }
        }
        db = readObj;
        update();
    }

    var blob = file.slice(0, file.length);
    return reader.readAsBinaryString(blob);
}


///------------------------------------

/// DOWNLOAD

/// Intercept ctrl + s
$(document).keydown(function (e) {

    var key = undefined;
    var possible = [e.key, e.keyIdentifier, e.keyCode, e.which];

    while (key === undefined && possible.length > 0) {
        key = possible.pop();
    }

    if (key && (key == '115' || key == '83') && (e.ctrlKey || e.metaKey) && !(e.altKey)) {
        e.preventDefault();
        outputJSON();
        return false;
    }
    return true;
});

//Interface for download();
function outputJSON() {
    var tempOutObj = db;
    for (i = 0; i < tempOutObj.list.length; i++) {
        tempOutObj.list[i].img = 0;
    }

    var jsonString = JSON.stringify(tempOutObj);
    var fileOutputName = db.mapInfo.uid.toString() + ".json";
    download(jsonString, fileOutputName, "json");
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