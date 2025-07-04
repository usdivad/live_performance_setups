// --------------------------------
// GUESS
// --------------------------------

console.log("---- GUESS ----");

// --------------------------------
// Setup

// ----
// Global

let hasStarted = false;
let bpm = 180;

// ----
// Melody

let melodyNoteRoot = 60;
let melodyNotesToGuess = [2, 4, 5, 7, 9, 11, 12];
let melodyNotesGuessed = [];

let melodyNotePlaybackIndex = 0;
let melodyNotePlaybackProbability = 0.7;
let melodyNotePlaybackDuration = "16n";

// TESTING
// melodyNotesGuessed = melodyNotesToGuess;

let melodySynth = new Tone.FMSynth().toDestination();
let melodyLoop = new Tone.Loop((time) => {

    if (melodyNotesGuessed.length < 1) {
        return;
    }

    let dice = Math.random();
    if (dice > melodyNotePlaybackProbability) {
        return;
    }

    let melodyNote = melodyNoteRoot + melodyNotesGuessed[melodyNotePlaybackIndex];
    let melodyNoteFreq = new Tone.Frequency(melodyNote, "midi");
    melodySynth.triggerAttackRelease(melodyNoteFreq, melodyNotePlaybackDuration, time);
    // console.log("Melody playing note " + melodyNote);

    melodyNotePlaybackIndex = (melodyNotePlaybackIndex + 1) % melodyNotesGuessed.length;
}, "8n");

// ----
// Drone

let droneSynth = new Tone.PolySynth(Tone.FMSynth).toDestination();

// --------------------------------
// Input

let inputSynth = new Tone.FMSynth().toDestination();
let inputFeedbackSynth = new Tone.PolySynth(Tone.FMSynth).toDestination();

let inputKeys = ["a", "s", "d", "f", "g", "h", "j", "k"];
let inputNotes = [0, 2, 4, 5, 7, 9, 11, 12];
let inputNoteRoot = melodyNoteRoot + 12;

let inputGuessedNotes = [];
let inputGuessedNoteIndex = -1;

// ----
// Enable WebMIDI input

WebMidi
    .enable()
    .then(onWebMidiEnabled)
    .catch(error => console.log(error));

// ----
// Input handlers

function handleKeyDown(event) {

    // ----
    // Get note

    const key = event.key;
    // console.log(key);

    let noteIndex = inputKeys.indexOf(key);
    if (noteIndex < 0 || noteIndex >= inputNotes.length) {
        return;
    }

    let noteGuess = inputNotes[noteIndex];

    // ----

    console.log("Input key down: " + key.toUpperCase() + " -- " + noteGuess);

    handleNoteInput(noteGuess);
}

async function handleMidiNoteOn(event) {
    // ----
    // First time: start

    if (!hasStarted) {
        await Tone.start();
        start();
        return;
    }

    // ----
    // Get note
    // TODO: Make this octave-agnostic -- currently we have to start on inputNoteRoot (72)

    const noteNum = event.note.number;
    let noteGuess = noteNum - inputNoteRoot; // TODO: Pass note num in directly to avoid extra calc

    // ----

    console.log("Input MIDI note on: " + noteNum + " -- " + noteGuess);

    handleNoteInput(noteGuess);
}

function handleNoteInput(noteGuess) {

    // ----
    // Play input note
    let note = inputNoteRoot + noteGuess;
    let noteFreq = new Tone.Frequency(note, "midi");
    inputSynth.triggerAttackRelease(noteFreq, "4n");
    // console.log("Input playing note " + note);

    // ----
    // Make sure we're playing the guessed melody

    // No need for further guessing if we've guessed all the melody notes
    if (melodyNotesToGuess.length < 1) {
        return;
    }

    let nextGuessedNoteIndex = inputGuessedNoteIndex + 1;
    // console.log(nextGuessedNoteIndex);

    if (nextGuessedNoteIndex <= melodyNotesGuessed.length - 1) {
        let nextGuessedNote = melodyNotesGuessed[nextGuessedNoteIndex];
        // console.log (noteGuess + " vs. " + nextGuessedNote);

        if (noteGuess == nextGuessedNote) {
            inputGuessedNoteIndex++;
            console.log("Next already-guessed note correctly played at index " + inputGuessedNoteIndex);
        } else {
            inputGuessedNoteIndex = -1;
            console.log("Resetting input already-guessed note index to -1");
        }

        return;
    }

    // ----
    // Handle correct guess

    // let melodyGuessIndex = melodyNotesToGuess.indexOf(noteGuess);
    // if (melodyGuessIndex < 0) {
    //     return;;
    // }

    let nextMelodyNote = melodyNotesToGuess[0];
    if (noteGuess == nextMelodyNote) {
        console.log("Next melody note " + nextMelodyNote + " guessed correctly!");
        melodyNotesGuessed.push(melodyNotesToGuess.shift());

        // Play the note an octave below as additional feedback
        inputFeedbackSynth.triggerAttackRelease(new Tone.Frequency(note - 12, "midi"), "1n");
        inputFeedbackSynth.triggerAttackRelease(new Tone.Frequency(melodyNoteRoot - 12, "midi"), "1n");

        // "Win state"
        if (melodyNotesToGuess.length < 1) {
            droneSynth.triggerAttack(new Tone.Frequency(melodyNoteRoot - 12, "midi"));
            melodyNotePlaybackDuration = "8n";
            // melodyNotePlaybackProbability = 0.9;
            console.log("You win!!");
        }
    } else {
        console.log("Incorrect guess for next melody note");
    }

    // Reset regardless of correct/incorrect guess
    inputGuessedNoteIndex = -1;
    // console.log("Resetting input guessed note index to -1");    
}

function onWebMidiEnabled() {
    console.log("MIDI enabled");

    console.log(WebMidi.inputs);

    WebMidi.inputs.forEach((device, index) => {
        console.log("Listening to MIDI device " + index + ": " + device.name);
        
        device.addListener("noteon", e => {
            // console.log(e.note.number);
            handleMidiNoteOn(e);
        }, {channels: [1,2,3,4]});
    })
}

// --------------------------------
// Utilities

function shuffle(arr) {
    // Fisher-Yates method via Knuth
    for (var i=arr.length-1; i>0; i--) {
        var j = Math.floor(Math.random()*i);
        var arr_i = arr[i];
        arr[i] = arr[j];
        arr[j] = arr_i;
    }
}


// --------------------------------
// Main

function start() {
    
    console.log("Starting audio");

    // ----
    // Setup melody
    shuffle(melodyNotesToGuess);
    melodyNotesToGuess.unshift(0); // Force melody to start on tonic
    console.log(melodyNotesToGuess);

    // ----
    // Start playback
    Tone.getTransport().start();
    Tone.getTransport().bpm.value = bpm;

    melodyLoop.start(0);

    let droneNoteFreq = new Tone.Frequency(melodyNoteRoot - 24, "midi");
    droneSynth.triggerAttack(droneNoteFreq);

    // ----
    // Update DOM
    document.getElementById("instructionsStart")?.remove();
    document.getElementById("instructionsNotes")?.classList.remove("hidden");

    // ----
    // Update book-keeping
    hasStarted = true;
}

window.onload = (event) => {
    // Start audio on button press
    document.querySelector("button")?.addEventListener("click", async () => {
        if (!hasStarted) {
            await Tone.start();
            start();
        }
    });
    // document.querySelector("button")?.addEventListener("click", () => {
    //     start();
    // });

    // Input on key down
    document.addEventListener("keydown", (event) => {
        if (hasStarted) {
            handleKeyDown(event);
        }
    });
    document.addEventListener("keydown", async (event) => {
        if (event.key == " " && !hasStarted) {
            await Tone.start();
            start();
        }
    });
};
