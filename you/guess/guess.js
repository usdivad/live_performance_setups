// --------------------------------
// GUESS
// --------------------------------

console.log("---- GUESS ----");

// --------------------------------
// Setup

// ----
// Global

let bpm = 180;

// ----
// Melody

let melodyNoteRoot = 60;
let melodyNotesToGuess = [0, 2, 4, 5, 7, 9, 11, 12];
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
    // Play input note
    let note = inputNoteRoot + noteGuess;
    let noteFreq = new Tone.Frequency(note, "midi");
    inputSynth.triggerAttackRelease(noteFreq, "4n");
    console.log("Input playing note " + note + " for key " + key.toUpperCase());

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
            console.log("Next already-guessed note correctly played; incrementing input guessed note index to " + inputGuessedNoteIndex);
        } else {
            inputGuessedNoteIndex = -1;
            console.log("Resetting input guessed note index to -1");
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
    }

    // Reset regardless of correct/incorrect guess
    inputGuessedNoteIndex = -1;
    // console.log("Resetting input guessed note index to -1");    
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
    // ----
    // Setup melody
    shuffle(melodyNotesToGuess);
    console.log(melodyNotesToGuess);

    // ----
    // Start playback
    Tone.getTransport().start();
    Tone.getTransport().bpm.value = bpm;

    melodyLoop.start(0);

    let droneNoteFreq = new Tone.Frequency(melodyNoteRoot - 24, "midi");
    droneSynth.triggerAttack(droneNoteFreq);
}

window.onload = (event) => {
    // Start audio on button press
    document.querySelector("button")?.addEventListener("click", async () => {
        await Tone.start();
        console.log("Starting audio");

        start();

        document.querySelector("button")?.remove();
    });

    // Input on key down
    document.addEventListener("keydown", (event) => {
        handleKeyDown(event);
    });
};
