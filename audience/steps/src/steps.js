$(document).ready(function() {
    // AUDIO
    console.log("ready");
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

    var sections = [
        {
            "id": "chant",
            "function": playChant,
            "notes": [43, 43, 55, 55, 60, 62, 64, 67, 67]
        },
        {
            "id": "verse",
            "function": playVerse,
            "notes": [55, 67]
        },
        {
            "id": "chorus",
            "function": playChorus,
            "notes": [55, 60, 62, 62, 62, 62, 64, 67, 67]
        },
        {
            "id": "solo",
            "function": playSolo,
            "notes": [55, 60, 62, 64, 67]
        },
        {
            "id": "outro",
            "function": playOutro,
            "notes": [55, 62, 67]
        }
    ];

    // add listeners to each display element
    for (var i=0; i<sections.length; i++) {
        var section = sections[i];
        (function(s) {
            var id = "#" + s["id"];
            var func = s["function"];
            var notes = s["notes"];
            $(id).on("mousedown touchstart", function(e) {
                $(".sectionDisp").removeClass("selected");
                $(".sectionDisp").removeClass("currentlyTouched");
                $(id).addClass("currentlyTouched");
                console.log("selected " + id);
                if (!oscsStarted) {
                    startOscs(oscs);
                    oscsStarted = true;
                }
                func(oscs, notes);
            });

            $(id).on("mouseup touchend", function(e) {
                $(id).removeClass("currentlyTouched");
                $(id).addClass("selected");
            })
        })(section);
    }

});

// SECTION FUNCTIONS
function playChant(oscs, notes) {
    pickNotesAndAdjustOscs(oscs, notes, "polyphonic");
}

function playVerse(oscs, notes) {
    pickNotesAndAdjustOscs(oscs, notes, "octaves");
}

function playChorus(oscs, notes) {
    pickNotesAndAdjustOscs(oscs, notes, "polyphonic");
}

function playSolo(oscs, notes) {
    pickNotesAndAdjustOscs(oscs, notes, "polyphonic");
}

function playOutro(oscs, notes) {
    pickNotesAndAdjustOscs(oscs, notes, "octaves");
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

// UTILITIES
function midiToHz(midiNote)
{
    var freq = (440 / 32) * (Math.pow(2,((midiNote - 9) / 12)));
    return freq;
}
