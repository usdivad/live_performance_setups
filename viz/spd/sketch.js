// Adapted from p5js-osc/p5-basic sketch: https://github.com/genekogan/p5js-osc/tree/master/p5-basic

// var x, y;
var numDivisionsX = 3;
var numDivisionsY = 5;

function setup() {
    createCanvas(800, 600);
    setupOsc(9001, 3334);
}

function draw() {
    background(0, 0, 0);

    // fill(0, 255, 0);
    // ellipse(x, y, 100, 100);
    // fill(0);
    // text("I'm p5.js", x-25, y);

    // Draw the SPD
    stroke(255, 0, 0);
    for (var i=1; i<numDivisionsY; i++) {
        if (i == 2 || i == 4) {
            continue;
        }

        var lineY = i * (height/numDivisionsY);
        line(0, lineY, width, lineY);
    }

    for (var i=1; i<numDivisionsY; i++) {
        var lineX = i * (width/numDivisionsX);
        line(lineX, 0, lineX, height);
    }    

    // line(0, 100, width, 100);
    

    // fill(255, 0, 0);

}


function receiveOsc(address, value) {
    console.log("received OSC: " + address + ", " + value);
    
    // if (address == '/test') {
    //     x = value[0];
    //     y = value[1];
    // }

    if (address == "/spd/in") {
        var note = value[0];
        var vel = value[1];
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