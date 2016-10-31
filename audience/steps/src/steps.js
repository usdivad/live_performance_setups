$(document).ready(function() {
    // AUDIO

    // web audio setup
    var ctx;

    window.AudioContext = window.AudioContext || window.webkitAudioContext;
    try {
        ctx = new AudioContext();
    }
    catch(e) {
        alert("Sorry, your browser doesn't support the Web Audio API. You can still enjoy the song though :)");
    }

    var oscs = [];
    var gains = [];
    var startNotes = [55, 67, 43];
    var oscTypes = [/*"sine",*/ "square", "sawtooth", "triangle"];
    var numOscs = 3;
    var oscsStarted = false;
    
    for (var i=0; i<numOscs; i++) {
        var osc = ctx.createOscillator();
        var gain = ctx.createGain();
        
        osc.type = oscTypes[Math.floor(Math.random()*oscTypes.length)];
        osc.frequency.value = midiToHz(startNotes[i]);
        
        gain.gain.value = 1/numOscs;

        osc.connect(gain);
        gain.connect(ctx.destination);
        
        oscs.push(osc);
        gains.push(gain);
    }

    // musical data
    var sections = [
        {
            "lyric": "touch here to begin",
            "function": playChant,
            "notes": [55]
        },
        {
            "lyric": "ahh ahh ahh ahh",
            "function": playChant,
            "notes": [55, 55, 60, 62, 64, 67, 67]
        },
        {
            "lyric": "falling back i think we're falling back",
            "function": playVerse,
            "notes": [55]
        },
        {
            "lyric": "feel afraid to face the changes",
            "function": playChorus,
            "notes": [55, 60, 62, 62, 62, 62, 64, 67, 67]
        },
        {
            "lyric": "ahh ahh ahh ahh",
            "function": playChant,
            "notes": [43, 43, 55, 55, 60, 62, 64, 67, 67]
        },
        {
            "lyric": "arrowheads these broken arrowheads",
            "function": playVerse,
            "notes": [55, 67]
        },
        {
            "lyric": "i feel afraid",
            "function": playChant, // technically "prechorus"
            "notes": [55, 62, 67]
        },
        {
            "lyric": "feel afraid to face the changes",
            "function": playChorus,
            "notes": [55, 60, 62, 62, 62, 62, 64, 67, 67]
        },
        {
            "lyric": "*sax solo*",
            "function": playSolo,
            "notes": [55, 60, 62, 64, 67]
        },
        {
            "lyric": "shadows come and shadows go",
            "function": playOutro,
            "notes": [55, 62, 67]
        }
    ];

    // USER TOUCH

    // add listeners to each display element
    // for (var i=0; i<sections.length; i++) {
    //     var section = sections[i];
    //     (function(s) {
    //         var id = "#" + s["id"];
    //         var func = s["function"];
    //         var notes = s["notes"];
    //         $(id).on("mousedown touchstart", function(e) {
    //             $(".sectionDisp").removeClass("selected");
    //             $(".sectionDisp").removeClass("currentlyTouched");
    //             $(id).addClass("currentlyTouched");
    //             console.log("selected " + id);
    //             if (!oscsStarted) {
    //                 startOscs(oscs);
    //                 oscsStarted = true;
    //             }
    //             func(oscs, gains, notes);
    //         });

    //         $(id).on("mouseup touchend", function(e) {
    //             $(id).removeClass("currentlyTouched");
    //             $(id).addClass("selected");
    //         });
    //     })(section);
    // }


    // cycle through sections with the same UI element
    var currentSection = 0;

    $(".sectionDisp").on("touchstart mousedown", function(e) {
        e.preventDefault();
        $(".sectionDisp").addClass("currentlyTouched");
        console.log("current section: " + currentSection);

        if (currentSection >= sections.length) {
            return;
        }

        var notes = sections[currentSection]["notes"];
        var func = sections[currentSection]["function"];

        if (!oscsStarted) {
            startOscs(oscs);
            oscsStarted = true;
        }

        func(oscs, gains, notes);
        
        currentSection++;
    });

    $(".sectionDisp").on("touchend mouseup", function(e) {
        e.preventDefault();
        $(".sectionDisp").removeClass("currentlyTouched");

        if (currentSection < sections.length) {
            var lyric = sections[currentSection]["lyric"]; // this will be the upcoming section because currentSection has already been incremented by the touchstart
            $(".sectionDisp").text(lyric);
        }
        else {
            $(".sectionDisp").css("visibility", "hidden");
        }
    });


    // USER DEVICE ORIENTATION
    // if (window.DeviceOrientationEvent) {
    //     window.addEventListener("deviceorientation", function(e) {
    //         var a = event.alpha; // 0 to 360
    //         var b = event.beta + 180; // -180 to 180 -> 0 to 360
    //         var c = (event.gamma * 2) + 180; // -90 to 90 -> 0 to 360
    //         $("#orientationInfo").text("a: " + a + ", b: " + b + ", " + "c: " + c);

    //         var orientations = [a, b, c];
    //         for (var i=0; i<orientations.length; i++) {
    //             var orientation = orientations[i];
    //             if (orientation && i < gains.length) {
    //                 var val = orientation / 360;
    //                 setGain(gains[i], val);
    //             }
    //         }
    //     }, false);
    // }
});

// SECTION FUNCTIONS
function playChant(oscs, gains, notes) {
    pickNotesAndAdjustOscs(oscs, notes, "polyphonic");
}

function playVerse(oscs, gains, notes) {
    setGains(gains, 0.5, 3);
    pickNotesAndAdjustOscs(oscs, notes, "octaves");
}

function playChorus(oscs, gains, notes) {
    setGains(gains, 1, 3);
    pickNotesAndAdjustOscs(oscs, notes, "polyphonic");

    var currentVerseLabel = $("#verse").text();
    if (currentVerseLabel.indexOf("falling") >= 0) {
        $("#verse").text("arrowheads");
    }
}

function playSolo(oscs, gains, notes) {
    pickNotesAndAdjustOscs(oscs, notes, "polyphonic");
    pointillize(oscs, gains, notes);
}

function playOutro(oscs, gains, notes) {
    for (var i=0; i<9999; i++) { // clear the pointillism
        window.clearInterval(i);
    }
    pickNotesAndAdjustOscs(oscs, notes, "octaves");
    setGains(gains, 1, 0.01);
    window.setTimeout(function() {
        // setGains(gains, 0.01, 40);
        fadeOutGains(gains, 30);
        fadeDisplay(20);
    }, 1000); // has to wait a bit longer than the rampup time, otherwise sometimes it doesn't trigger
}

// OSC ADJUSTERS
function pickNotesAndAdjustOscs(oscs, notes, mode) {
    var baseNote = notes[Math.floor(Math.random() * notes.length)];

    for (var i=0; i<oscs.length; i++) {
        var note;
        if (mode == "polyphonic") { // pick a new random note
            note = notes[Math.floor(Math.random() * notes.length)];
        }
        else if (mode == "octaves") { // ocatves above & below
            switch (i%3) {
                case 0:
                    note = baseNote;
                    break;
                case 1:
                    note = baseNote + 12;
                case 2:
                    note = baseNote - 12;
            }
        }
        else { // monophonic
            note = baseNote;
        }
        oscs[i].frequency.value = midiToHz(note);
    }
}

function startOscs(oscs) {
    for (var i=0; i<oscs.length; i++) {
        oscs[i].start(0);
    }
}

function setGains(gains, val, dur) {
    for (var i=0; i<gains.length; i++) {
        setGain(gains[i], val, dur); // divide val by gains.length?
    }
}

function setGain(gain, val, dur) {
    // gain.gain.exponentialRampToValueAtTime(val, dur);
    gain.gain.value = val;
}

function fadeOutGains(gains, dur) {
    var step = 0.01;
    var interval = dur * (1/step) / 10;
    for (var i=0; i<gains.length; i++) {
        var g = gains[i];
        (function(gain) {
            var intervalId = window.setInterval(function() {
                if (gain.gain.value <= 0) {
                    window.clearInterval(intervalId);
                }
                else {
                    var val = gain.gain.value - step;
                    gain.gain.value = Math.max(val, 0);
                }
                console.log(gain.gain.value);
            }, interval);
        })(g);
    }
}


function pointillize(oscs, gains, notes) {
    window.setInterval(function() {
        var takeAction = Math.random() < 0.75;
        if (takeAction) {
            var i = Math.floor(Math.random() * oscs.length);
            var currentGainVal = gains[i].gain.value;
            console.log(i + ", " + currentGainVal);
            if (currentGainVal > 0) {
                gains[i].gain.value = 0;
                pickNotesAndAdjustOscs([oscs[i]], notes, "polyphonic");
                console.log("setset");
            }
            else {
                gains[i].gain.value = 1;
            }
        }
    }, 100);
}

function fadeDisplay(dur) {
    var step = 0.01;
    var interval = dur * (1/step) / 10;
    
    var intervalId = window.setInterval(function() {
        var currentOpacity = $("body").css("opacity");
        if (currentOpacity > 0) {
            $("body").css("opacity", Math.max(currentOpacity - step, 0));
        }
        else {
            window.clearInterval(intervalId);
        }
    }, interval);
}

// UTILITIES
function midiToHz(midiNote)
{
    var freq = (440 / 32) * (Math.pow(2,((midiNote - 9) / 12)));
    return freq;
}
