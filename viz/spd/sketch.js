// Adapted from p5js-osc/p5-basic sketch: https://github.com/genekogan/p5js-osc/tree/master/p5-basic

//====
// Globs
// var x, y;
var numDivisionsX = 3;
var numDivisionsY = 6;
var numFramesToAnimate = 30;
var pads;
// var midiMapping = "BATDUB"; // BAT_DUB, SPD_DEFAULT


// ====
// p5
function setup() {
    createCanvas(800, 600);
    setupOsc(9001, 3334);
    setupPads();
}

function draw() {
    background(0, 0, 0);

    // fill(0, 255, 0);
    // ellipse(x, y, 100, 100);
    // fill(0);
    // text("I'm p5.js", x-25, y);

    // Draw the SPD
    stroke(255, 0, 0);
    for (var i=0; i<numDivisionsY; i++) {
        if (i == 2 || i == 4) {
            continue;
        }

        var lineY = i * (height/numDivisionsY);
        line(0, lineY, width, lineY);
    }

    for (var i=0; i<numDivisionsY; i++) {
        var lineX = i * (width/numDivisionsX);
        var h = height;

        if (i == 1 || i == 2) { // To get whole width on bottom kick pad
            h = height * ((numDivisionsY-1)/numDivisionsY);
        }

        line(lineX, 0, lineX, h);
    }    

    line(width-1, 0, width-1, height);
    line(0, height-1, width, height-1);

    // Animate the SPD
    for (var i=0; i<pads.length; i++) {
        var pad = pads[i];
        if (pad["framesLeftToAnimate"] > 0) {
            // console.log("asdf");

            var px = pad["rectArgs"][0];
            var py = pad["rectArgs"][1];
            var pw = pad["rectArgs"][2];
            var ph = pad["rectArgs"][3];
            var pa = (pad["framesLeftToAnimate"] / numFramesToAnimate) * 255;
            // console.log("pa: " + pa);

            fill(255, 0, 0, pa);
            // fill(255, 0, 0);
            rect(px, py, pw, ph);
            // rect(0, 0, width, height);

            pad["framesLeftToAnimate"]--;
        }
    }

}

// ====
// Custom

function setupPads() {
    var padWidth = width / numDivisionsX;
    var topPadHeight = height / numDivisionsY;
    var otherPadHeight = topPadHeight * 2;

    pads = [
        {
            "framesLeftToAnimate": 0,
            "midiNote": 60,
            "rectArgs": [0, 0, padWidth, topPadHeight]
        },
        {
            "framesLeftToAnimate": 0,
            "midiNote": 61,
            "rectArgs": [padWidth, 0, padWidth, topPadHeight]
        },
        {
            "framesLeftToAnimate": 0,
            "midiNote": 62,
            "rectArgs": [padWidth*2, 0, padWidth, topPadHeight]
        },
        {
            "framesLeftToAnimate": 0,
            "midiNote": 63,
            "rectArgs": [0, topPadHeight, padWidth, otherPadHeight]
        },
        {
            "framesLeftToAnimate": 0,
            "midiNote": 64,
            "rectArgs": [padWidth, topPadHeight, padWidth, otherPadHeight]
        },
        {
            "framesLeftToAnimate": 0,
            "midiNote": 65,
            "rectArgs": [padWidth*2, topPadHeight, padWidth, otherPadHeight]
        },
        {
            "framesLeftToAnimate": 0,
            "midiNote": 66,
            "rectArgs": [0, topPadHeight + otherPadHeight, padWidth, otherPadHeight]
        },
        {
            "framesLeftToAnimate": 0,
            "midiNote": 67,
            "rectArgs": [padWidth, topPadHeight + otherPadHeight, padWidth, otherPadHeight]
        },
        {
            "framesLeftToAnimate": 0,
            "midiNote": 68,
            "rectArgs": [padWidth*2, topPadHeight + otherPadHeight, padWidth, otherPadHeight]
        },
        {
            "framesLeftToAnimate": 0,
            "midiNote": 69,
            "rectArgs": [0, topPadHeight + otherPadHeight*2, padWidth*3, topPadHeight]
            // "rectArgs": [padWidth*2, topPadHeight + otherPadHeight, padWidth, otherPadHeight]
        },
    ];
}

function handleOscFromSpd(value){
    console.log("handling");

    var note = value[0];
    var vel = value[1];

    var padIdx = note - 60;
    console.log(padIdx);
    if (padIdx >=0 && padIdx < pads.length) {
        var pad = pads[padIdx];
        pad["framesLeftToAnimate"] = numFramesToAnimate;
        console.log(pads[padIdx]["framesLeftToAnimate"]);
    }
}



// ====
// OSC
function receiveOsc(address, value) {
    console.log("received OSC: " + address + ", " + value);
    
    // if (address == '/test') {
    //     x = value[0];
    //     y = value[1];
    // }

    if (address == "/spd/in") {
        handleOscFromSpd(value);
    }
}

function sendOsc(address, value) {
    socket.emit('message', [address].concat(value));
}

function setupOsc(oscPortIn, oscPortOut) {
    var socket = io.connect('http://127.0.0.1:8081', { port: 8081, rememberTransport: false });
    socket.on('connect', function() {
        socket.emit('config', { 
            server: { port: oscPortIn,  host: '127.0.0.1'},
            client: { port: oscPortOut, host: '127.0.0.1'}
        });
    });
    socket.on('message', function(msg) {
        if (msg[0] == '#bundle') {
            for (var i=2; i<msg.length; i++) {
                receiveOsc(msg[i][0], msg[i].splice(1));
            }
        } else {
            receiveOsc(msg[0], msg.splice(1));
        }
    });
}