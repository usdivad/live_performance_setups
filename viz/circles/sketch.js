// Adapted from the SPD viz

//====
// Globs
var numFramesToAnimate = 30;
var framesLeftToAnimate = 0;

// ====
// p5
function setup() {
    createCanvas(800, 600);
}

function draw() {
    background(0, 0, 0);

    if (framesLeftToAnimate > 0) {
        var alpha = (framesLeftToAnimate / numFramesToAnimate) * 255;
        var size = ((framesLeftToAnimate / numFramesToAnimate) * 300) + 100;
        // alpha = 255;
        fill(255, 255, 0, alpha);
        ellipse(width/2, height/2, size, size);

        framesLeftToAnimate--;
    }
}

function keyPressed() {
    framesLeftToAnimate = numFramesToAnimate;
}